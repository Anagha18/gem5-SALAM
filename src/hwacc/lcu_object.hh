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
            const Addr result;
            int timesLeft;
            int lcu_blob[100];
        public: 
            LcuObject(const LcuObjectParams &p);
            void startup();
    };
} //namespace gem5

#endif 

