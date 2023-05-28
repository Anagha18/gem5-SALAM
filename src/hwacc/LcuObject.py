from m5.params import * 
from m5.proxy import *
from m5.SimObject import SimObject

class LcuObject(SimObject):
    type='LcuObject'
    cxx_header='hwacc/lcu_object.hh'
    cxx_class = 'gem5::LcuObject'

    time_to_wait = Param.Latency("Time before firing the event")
    number_of_fires = Param.Int(1, "Number of times to be fired")
    result_addr = Param.Addr(0, "Write the result to this memory location")
    # lcu_blob = Param.A
    # inst_port = SlavePort("CPU side port, recieves requests")
    # data_port = SlavePort("CPU side port, recieves messages")
    # mem_side = MasterPort("Memory side port, sends request")


