#!/usr/bin/env python
import time
import roboclaw
import sys
from enum import Enum
import numpy as np
# BECAUSE I HAD TO MAKE A NEW MAT
import mat2 as mat
import math

#Linux comport name
roboclaw.Open("/dev/ttySAC0",38400)

# mlib.py (motor library)
# Defines abstraction for the motors

addr1 = 0x80
addr2 = 0x81
# Hold the kicker back
# roboclaw.ForwardM2(addr2,127) # max power

#Set (default) velocity pids
roboclaw.SetM1VelocityPID(addr1,1,.5,.1,180000)
roboclaw.SetM2VelocityPID(addr1,1,.5,.1,180000)
roboclaw.SetM1VelocityPID(addr2,1,.5,.1,180000)

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



# motor gear ticks for 1 revolution / 2pi radians
mtps = 19820

# scale ticks per second so that a value of 1 corresponds to 2pi rotations per second.
def radian2Qpps(radian):
  result = int(radian * mtps / (2.0*np.pi))
  if result > 308420:
    return 308420
  elif result < -308420:
    return -308420
  else:
    return result
    
# scale from ticks per second to radians
def qpps2Radian(qpps):
  result = int(qpps / mtps * (2.0*np.pi))
  return result

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
def SpeedM(motor,omega):
	# Convert to rotations/second
	qpps = radian2Qpps(omega)
	if motor == 1:
		return roboclaw.SpeedM1(addr1,qpps)
	elif motor == 2:
		return roboclaw.SpeedM2(addr1,qpps)
	elif motor == 3:
		return roboclaw.SpeedM1(addr2,qpps)
	else:
		raise mlibExcept(e_type.motorNumOff)

def speedAccelM(motor,time,speed):
	# Convert to rotations/second
	qpps = radian2Qpps(speed)
	accel = qpps/time
	if motor == 1:
		return roboclaw.SpeedAccelM1(addr1,accel,qpps)
	elif motor == 2:
		return roboclaw.SpeedAccelM2(addr1,accel,qpps)
	elif motor == 3:
		return roboclaw.SpeedAccelM1(addr2,accel,qpps)
	else:
		raise mlibExcept(e_type.motorNumOff)
	return

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

def setMVelocityPID(motor,p,i,d,qpps):
	if motor == 1:
		return roboclaw.SetM1VelocityPID(addr1,p,i,d,qpps)
	elif motor == 2:
		return roboclaw.SetM2VelocityPID(addr1,p,i,d,qpps)
	elif motor == 3:
		return roboclaw.SetM1VelocityPID(addr2,p,i,d,qpps)
	else:
		raise mlibExcept(e_type.motorNumOff)

def readMVelocityPID(motor):
	if motor == 1:
		return roboclaw.ReadM1VelocityPID(addr1)
	elif motor == 2:
		return roboclaw.ReadM2VelocityPID(addr1)
	elif motor == 3:
		return roboclaw.ReadM1VelocityPID(addr1)
	else:
		raise mlibExcept(e_type.motorNumOff)


# Returns the speed of the motor in revolutions/second
def readSpeedM(motor):
	if motor == 1:
		return roboclaw.ReadSpeedM1(addr1)
	elif motor == 2:
		return roboclaw.ReadSpeedM2(addr1)
	elif motor == 3:
		return roboclaw.ReadSpeedM1(addr2)
	else:
		raise mlibExcept(e_type.motorNumOff)




# # Based on wheel speed instead of distance
# def moveBodyX(speed):
# 	ForwardBackM(1,speed) # Maybe use speed instead, since it specifies omega instead of arbitrary motor power
# 	ForwardBackM(2,-speed)
# 	return

# # Based on wheel speed instead of distance
# def rotateRate(speed):
# 	x = [1,2,3]
# 	for n in x:
# 		ForwardBackM(n,speed)

# def goXYOmega(x,y,omega):
# 	# Get what the wheel omegas needs to be
# 	v1,v2,v3 = mat.getWheelVel(x,y,omega)
# 	# Drive each wheel at the appropriate omega
# 	SpeedM(1,v1)
# 	SpeedM(2,v2)
# 	SpeedM(3,v3)

# Go in an XY direction with an omega.
# Theta is defaulted to zero (body and world frames aligned)
def goXYOmegaWorld(vx_w,vy_w,omega=0,theta=0):
	# Get required wheel velocities from the commanded world velocities
	# [Omega1,Omega2,Omega3]
	wheelVels = mat.getWheelVelFromWorld(vx_w,vy_w,omega,theta)
	k = 1
	# Set speed for each wheel
	for w in wheelVels:
		SpeedM(k,w)
		k = k+1
	return

# This function doesn't work well with the roboclaw. they get commands at different times.
def goXYOmegaWorldAccel(vx_w,vy_w,omega=0,theta=0,time=1):
	wheelVels = mat.getWheelVelFromWorld(vx_w,vy_w,omega,theta)
	k = 1
	# Set speed for each wheel
	for w in wheelVels:
		speedAccelM(k,time,w)
		k = k+1
	return

# Hard stop all motors
def stop():
	motors = [1,2,3]
	for m in motors:
		ForwardBackM(m,0)
	return

# Since we are using the roboclaw to source enough voltage for the kicker, we are using a roboclaw command to kick it
def kick():
	roboclaw.ForwardM2(addr2,0) # max power
	time.sleep(.1)
	return roboclaw.ForwardM2(addr2,127)


def init_kick():
	# Hold the kicker back
	return roboclaw.ForwardM2(addr2,127)


def uninit_kick():
	return roboclaw.ForwardM2(addr2,0)

#def SetMVelocityPID(address,p,i,d,qpps):

def deg2rad(deg):
    return deg*math.pi/180
