from mlib import *
from time import sleep

sleep(2)

 ##spin in circle
goXYOmegaWorld(0,0,3)
sleep(2)
stop()
sleep(2)

##go forward
# goXYOmegaWorld(.1,0)
# sleep(1)
# stop()
# sleep(1)
##go in square
goXYOmegaWorld(.1,0)
sleep(1)
stop()
sleep(.5)
goXYOmegaWorld(0,.1)
sleep(1)
stop()
sleep(.5)
goXYOmegaWorld(-.1,0)
sleep(1)
stop()
sleep(.5)
goXYOmegaWorld(0,-.1)
sleep(1)
stop()

