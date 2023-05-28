#ifndef __HWACC_LCU_OBJECT_HH__
#define __HWACC_LCU_OBJECT_HH__

#include "mem/port.hh"
#include "params/LcuObject.hh"
#include "sim/sim_object.hh"

namespace gem5
{
    class LcuObject : public SimObject
    {
        private:
            
            void processEvent();
            EventFunctionWrapper event;
            const std::string myName;
            const Tick latency;
            int result;
            int timesLeft;
            int lcu_blob[100];
            
            class CPUSidePort : public ResponsePort
            {
                private:
                    LcuObject* owner;
                    bool needRetry;
                    PacketPtr blockedPacket;
                public:
                    CPUSidePort(const std::string& name, LcuObject* owner):
                        ResponsePort(name, owner),
                        owner(owner), needRetry(false), blockedPacket(nullptr)
                    {}
                    AddrRangeList getAddrRanges() const override;
                    bool blocked(){ return blockedPacket != nullptr;}
                    void sendPacket(PacketPtr pkt);
                    void trySendRetry();

                protected:
                    Tick recvAtomic(PacketPtr pkt) override
                    {
                        panic("recvAtomic unimplemented.");
                    }
                    void recvFunctional(PacketPtr pkt) override;
                    bool recvTimingReq(PacketPtr pkt) override;
                    void recvRespRetry() override;

            };
            class MemSidePort : public RequestPort
            {
                private:
                    LcuObject* owner;
                    PacketPtr blockedPacket;
                public:
                    MemSidePort(const std::string& name, LcuObject* owner):
                        RequestPort(name, owner), owner(owner), blockedPacket(nullptr)
                    {}
                    bool blocked(){return blockedPacket != nullptr;}
                    void sendPacket(PacketPtr pkt);
                
                protected:
                    bool recvTimingResp(PacketPtr pkt) override;
                    void recvReqRetry() override;
                    void recvRangeChange() override;

            };

            CPUSidePort instPort;
            CPUSidePort dataPort;

            MemSidePort memPort;
            bool blocked;

            void handleFunctional(PacketPtr pkt);
            
            AddrRangeList getAddrRanges() const;
            
            void sendRangeChange();

            bool handleRequest(PacketPtr pkt);

            bool handleResponse(PacketPtr pkt);


        public: 
            
            PARAMS(LcuObject);
            LcuObject(const Params &p);
            Port& getPort(const std::string& if_name, PortID idx = InvalidPortID) override;

            void startup();
    };
} //namespace gem5

#endif 

