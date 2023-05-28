import m5
from m5.objects import *

root = Root(full_system=False)
root.lcu = LcuObject(time_to_wait='1us', number_of_fires = 3)

m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()
print('Exiting @ tick {} because {}'
      .format(m5.curTick(), exit_event.getCause()))