from m5.params import * 
from m5.proxy import *
from m5.SimObject import SimObject

class LcuObject(SimObject):
    type='LcuObject'
    cxx_header='hwacc/lcu_object.hh'
    cxx_class = 'gem5::LcuObject'

    time_to_wait = Param.Latency("Time before firing the event")
    number_of_fires = Param.Int(1, "Number of times to be fired")
    result = Param.Int(0, "Write the result to this memory location")
    # lcu_blob = Param.A
    inst_port = ResponsePort("CPU side port, recieves requests. (Instructions)")
    data_port = ResponsePort("CPU side port, recieves messages. (Data)")
    mem_side = RequestPort("Memory side port, sends request. (Inst + data)")


