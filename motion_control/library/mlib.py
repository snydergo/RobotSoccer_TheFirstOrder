#!/usr/bin/env python
import time
import roboclaw
import sys
from enum import Enum
# BECAUSE I HAD TO MAKE A NEW MAT
import mat2 as mat

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



# motor gear ticks for 1 revolution
mtps = 19820

# scale ticks per second so that a value of 1 corresponds to 1 rotations per second.
def omega2qpps(omega):
	return int(omega*mtps)
# scale from ticks per second to omega
def qpps2omega(qpps):
	return qpps/mtps

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
	qpps = omega2qpps(omega)
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
	qpps = omega2qpps(speed)
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

# Based on wheel speed instead of distance
def rotateRate(speed):
	x = [1,2,3]
	for n in x:
		ForwardBackM(n,speed)

def rotateDeg(degrees):
	return



def goXYOmega(x,y,omega):
	# Get what the wheel omegas needs to be
	v1,v2,v3 = mat.getWheelVel(x,y,omega)
	# Drive each wheel at the appropriate omega
	SpeedM(1,v1)
	SpeedM(2,v2)
	SpeedM(3,v3)

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

def goDisXYOmegaWorld(x_w,y_w,time,omega=0,theta=0):
	#goXYOmegaWorld()
	return

# Hard stop all motors
def stop():
	motors = [1,2,3]
	for m in motors:
		ForwardBackM(m,0)
	return

def smoothStop():
	return
#def SetMVelocityPID(address,p,i,d,qpps):
