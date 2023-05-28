from m5.params import * 
from m5.SimObject import SimObject

class LcuObject(SimObject):
    type='LcuObject'
    cxx_header='hwacc/lcu_object.hh'
    cxx_class = 'gem5::LcuObject'

    time_to_wait = Param.Latency("Time before firing the event")
    number_of_fires = Param.Int(1, "Number of times to be fired")

