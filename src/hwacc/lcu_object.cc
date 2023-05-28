#include "hwacc/lcu_object.hh"

#include "base/trace.hh"
#include "debug/LcuDebugFlag.hh"

namespace gem5
{
    LcuObject::LcuObject(const LcuObjectParams &params) :
        SimObject(params), 
        event([this]{processEvent();}, name()),
        myName(params.name),
        latency(params.time_to_wait),
        timesLeft(params.number_of_fires),
        result(params.result_addr)
        {
            DPRINTF(LcuDebugFlag,"%s: Welcome to my LCU SimObject\n", myName);
        }
    void
    LcuObject::processEvent()
    {
        lcu_blob[3 - timesLeft] = timesLeft;
        timesLeft--;
        
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
        schedule(event, curTick());
    }
} // namespace gem5

