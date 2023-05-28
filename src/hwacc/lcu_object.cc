#include "hwacc/lcu_object.hh"

#include "base/trace.hh"
#include "debug/LcuDebugFlag.hh"

namespace gem5
{
    LcuObject::LcuObject(const Params& params) :
        SimObject(params),
        
        event([this]{processEvent();}, name()),
        myName(params.name),
        latency(params.time_to_wait),
        timesLeft(params.number_of_fires),
        result(params.result),
        instPort(name() + ".inst_port", this),
        dataPort(name() + ".data_port", this),
        memPort(name() + ".mem_side", this),
        blocked(false)
        {
            DPRINTF(LcuDebugFlag,"%s: Welcome to my LCU SimObject\n", __func__);
        }
    Port&
    LcuObject::getPort(const std::string& if_name, PortID idx)
    {
        if(if_name == "inst_port"){
            return instPort;
        }else if(if_name == "data_port"){
            return dataPort;
        }else if(if_name == "mem_side"){
            return memPort;
        }else{
            DPRINTF(LcuDebugFlag,"%s: Stuck at getting the Port\n", __func__);
            return LcuObject::getPort(if_name, idx);
        }
    }

    void
    LcuObject::CPUSidePort::recvFunctional(PacketPtr pkt)
    {
        owner->handleFunctional(pkt);
    }

    AddrRangeList
    LcuObject::CPUSidePort::getAddrRanges() const
    {
        return owner->getAddrRanges();
    }

    void 
    LcuObject::handleFunctional(PacketPtr pkt)
    {
        memPort.sendFunctional(pkt);
    }

    AddrRangeList
    LcuObject::getAddrRanges() const
    {
        DPRINTF(LcuDebugFlag, "%s: sending new ranges.\n", __func__);
        return memPort.getAddrRanges();
    }

    void
    LcuObject::MemSidePort::recvRangeChange()
    {
        owner->sendRangeChange();

    }

    void
    LcuObject::sendRangeChange()
    {
        instPort.sendRangeChange();
        dataPort.sendRangeChange();
    }

    bool 
    LcuObject::CPUSidePort::recvTimingReq(PacketPtr pkt)
    {
        if(!owner->handleRequest(pkt)){
            needRetry = true;
            return false;
        }else{
            return true;
        }
    }

    bool
    LcuObject::handleRequest(PacketPtr pkt)
    {
        if(blocked || memPort.blocked() || instPort.blocked() || dataPort.blocked()){
            return false;
        }
        DPRINTF(LcuDebugFlag, "%s: recieved request for addr %#x.\n", __func__, pkt->getAddr());
        blocked = true;
        memPort.sendPacket(pkt);
        return true;
    }

    void
    LcuObject::MemSidePort::sendPacket(PacketPtr pkt)
    {
        panic_if(blockedPacket != nullptr, "should not try to send if blocked!.");
        if(!sendTimingReq(pkt)){
            blockedPacket=pkt;
        }
    }

    void
    LcuObject::MemSidePort::recvReqRetry()
    {
        assert(blockedPacket != nullptr);
        DPRINTF(LcuDebugFlag, "%s: recieved request retry.\n", __func__);
        PacketPtr pkt = blockedPacket;
        blockedPacket = nullptr;
        sendPacket(pkt);
    }

    bool 
    LcuObject::MemSidePort::recvTimingResp(PacketPtr pkt)
    {
        return owner->handleResponse(pkt);
    }

    bool 
    LcuObject::handleResponse(PacketPtr pkt)
    {
        assert(blocked);
        DPRINTF(LcuDebugFlag, "%s: Recieved response for Addr %#x", __func__, pkt->getAddr());
        blocked= false;
        if(pkt->req->isInstFetch()){
            instPort.sendPacket(pkt);
        }else{
            dataPort.sendPacket(pkt);
        }
        instPort.trySendRetry();
        dataPort.trySendRetry();
        return true;
    }

    void
    LcuObject::CPUSidePort::sendPacket(PacketPtr pkt){
        panic_if(blockedPacket != nullptr, "should never try to send when blocked");
        if(!sendTimingResp(pkt)){
            blockedPacket = pkt;
        }
    }

    void
    LcuObject::CPUSidePort::recvRespRetry()
    {
        assert(blockedPacket != nullptr);
        PacketPtr pkt = blockedPacket;
        blockedPacket = nullptr;
        sendPacket(pkt);
    }

    void
    LcuObject::CPUSidePort::trySendRetry()
    {
        if(needRetry && blockedPacket == nullptr){
            needRetry = false;           
            DPRINTF(LcuDebugFlag, "%s: Sending a retry request.\n", __func__);
            sendRetryReq();
        }

    }

    void
    LcuObject::processEvent()
    {
        lcu_blob[3 - timesLeft] = timesLeft;
        timesLeft--;
        // params.result = 300; 
        DPRINTF(LcuDebugFlag, "LCU is processing an event!\n");
        if(timesLeft <= 0){
            for(int i = 0; i < 3; i++)
                DPRINTF(LcuDebugFlag, "%d\n", lcu_blob); 
            // *(int*)0x55901be42b3c = 300;
              
            DPRINTF(LcuDebugFlag, "DoneFiring!\n");
        }
        else{
            schedule(event, curTick() + latency); 
        }
    }
    void
    LcuObject::startup()
    {
        schedule(event, curTick()+latency);
    }
} // namespace gem5

