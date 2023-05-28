import m5
from m5.objects import *
import ctypes

result = 0;
root = Root(full_system=False)
root.lcu = LcuObject(time_to_wait='1us', number_of_fires = 3, result_addr = hex(id(result)))

m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()
print('Exiting @ tick {} because {}'
      .format(m5.curTick(), exit_event.getCause()))

# print(ctypes.cast(0x55901be42b3c, ctypes.py_object).value)
