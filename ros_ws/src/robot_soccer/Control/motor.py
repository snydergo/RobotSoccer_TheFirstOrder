#***Before using this example the motor/controller combination must be
#***tuned and the settings saved to the Roboclaw using IonMotion.
#***The Min and Max Positions must be at least 0 and 50000

import time
import roboclaw
import sys

def displayspeed():
	enc1 = roboclaw.ReadEncM1(address)
	enc2 = roboclaw.ReadEncM2(address)
	speed1 = roboclaw.ReadSpeedM1(address)
	speed2 = roboclaw.ReadSpeedM2(address)

	print("Encoder1:"),
	if(enc1[0]==1):
		print enc1[1],
		print format(enc1[2],'02x'),
	else:
		print "failed",
	print "Encoder2:",
	if(enc2[0]==1):
		print enc2[1],
		print format(enc2[2],'02x'),
	else:
		print "failed " ,
	print "Speed1:",
	if(speed1[0]):
		print speed1[1],
	else:
		print "failed",
	print("Speed2:"),
	if(speed2[0]):
		print speed2[1]
	else:
		print "failed "

#Windows comport name
#roboclaw.Open("COM3",38400)
#Linux comport name
roboclaw.Open("/dev/ttySAC0",38400)

address = 0x80

version = roboclaw.ReadVersion(address)
if version[0]==False:
	print "GETVERSION Failed"
else:
	print repr(version[1])

roboclaw.SpeedM1(address,10)
roboclaw.SpeedM2(address,10)

time.sleep(2)
roboclaw.SpeedM1(address,0)
roboclaw.SpeedM2(address,0)

# while(1):
# 	try:
# 		speed = int(raw_input("Enter speed: "))
# 		roboclaw.SpeedM1(address,speed)
# 	except ValueError:
# 		print "Not a number"

	# roboclaw.SpeedM1(address,12000)
	# roboclaw.SpeedM2(address,-12000)
	# for i in range(0,200):
	# 	displayspeed()
	# 	time.sleep(0.01)

	# roboclaw.SpeedM1(address,-12000)
	# roboclaw.SpeedM2(address,12000)
	# for i in range(0,200):
	# 	displayspeed()
	# 	time.sleep(0.01)
