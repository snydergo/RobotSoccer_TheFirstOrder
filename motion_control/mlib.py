import time
import roboclaw
import sys
from enum import Enum
import mat

#Linux comport name
roboclaw.Open("/dev/ttySAC0",38400)

# mlib.py (motor library)
# Defines abstraction for the motors

addr1 = 0x80
addr2 = 0x81

#Set (default) velocity pids
roboclaw.SetM1VelocityPID(addr1,1,.5,.25,180000)
roboclaw.SetM2VelocityPID(addr1,1,.5,.25,180000)
roboclaw.SetM1VelocityPID(addr2,1,.5,.25,180000)

class e_type(Enum):
	motorNumOff = 1

class mlibExcept(Exception):
	errorType = 0
	def __init__(self,errorType):
		self.errorType = errorType
	def __str__(self):
		if self.errorType == e_type.motorNumOff:
			return "@param motor must be value between 1-3"
		else:
			return "Iunno"



# motor ticks per second
mtps = 19820

# scale ticks per second so that a value of 1 corresponds to 1 rotations per second.
def scaleMtps(omega):
	return int(omega*mtps)

# Move Motor Forward.
# @param motor: 1 => Front-Right, 2 => Front-Left, 3 => Back
# @param val: 	a number between 1-128 (i think)
def ForwardM(motor,val):
	if motor == 1:
		return roboclaw.ForwardM1(addr1,val)
	elif motor == 2:
		return roboclaw.ForwardM2(addr1,val)
	elif motor == 3:
		return roboclaw.ForwardM1(addr2,val)
	else:
		raise mlibExcept(e_type.motorNumOff)

def BackwardM(motor,val):
	if motor == 1:
		return roboclaw.BackwardM1(addr1,val)
	elif motor == 2:
		return roboclaw.BackwardM2(addr1,val)
	elif motor == 3:
		return roboclaw.BackwardM1(addr2,val)
	else:
		raise mlibExcept(e_type.motorNumOff)

# Positive val will move motor forward. Negative val will move it backward
def ForwardBackM(motor,val):
	if motor < 1 or motor > 3:
		raise mlibExcept(e_type.motorNumOff)
	elif val >= 0:
		return ForwardM(motor,val)
	elif val < 0:
		return BackwardM(motor,abs(val))
	else:
		print("This should never happen")
		return		

# @param val should be given in rotations/second
def SpeedM(motor,val):
	# Convert to rotations/second
	omega = scaleMtps(val)
	if motor == 1:
		return roboclaw.SpeedM1(addr1,omega)
	elif motor == 2:
		return roboclaw.SpeedM2(addr1,omega)
	elif motor == 3:
		return roboclaw.SpeedM1(addr2,omega)
	else:
		raise mlibExcept(e_type.motorNumOff)

def ReadEncM(motor):
	if motor == 1:
		return roboclaw._read4_1(addr1,roboclaw.Cmd.GETM1ENC)
	elif motor == 2:
		return roboclaw._read4_1(addr1,roboclaw.Cmd.GETM2ENC)
	elif motor == 3:
		return roboclaw._read4_1(addr2,roboclaw.Cmd.GETM1ENC)
	else:
		raise mlibExcept(e_type.motorNumOff)

def SetEncM(motor,cnt):
	if motor == 1:
		return roboclaw._write4(addr1,roboclaw.Cmd.SETM1ENCCOUNT,cnt)
	elif motor == 2:
		return roboclaw._write4(addr1,roboclaw.Cmd.SETM2ENCCOUNT,cnt)
	elif motor == 3:
		return roboclaw._write4(addr2,roboclaw.Cmd.SETM1ENCCOUNT,cnt)
	else:
		raise mlibExcept(e_type.motorNumOff)

# Based on wheel speed instead of distance
def moveBodyX(speed):
	ForwardBackM(1,speed) # Maybe use speed instead, since it specifies omega instead of arbitrary motor power
	ForwardBackM(2,-speed)
	return

# Based on wheel speed instead of distance
def rotate(speed):
	x = [1,2,3]
	for n in x:
		ForwardBackM(n,speed)

def stop():
	motors = [1,2,3]
	for m in motors:
		ForwardBackM(m,0)
	return

#def SetMVelocityPID(address,p,i,d,qpps):
