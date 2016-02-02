#!/usr/bin/python

from numpy import matrix
from numpy import linalg
import math

# Measurements in meters

# radius of wheel
R = .02712 #tune me #meters
#radius from center of robot to wheel
r = .066 #tune me. #meters
 
# Front-Right wheel
r1theta = -math.pi/3.0 # angular position
r1x = math.cos(r1theta)*r # Cartesian x
r1y = math.sin(r1theta)*r # Cartedsian y

# Front-Left wheel
r2theta = math.pi/3.0 # angular position
r2x = math.cos(r2theta)*r # Cartesian x
r2y = math.sin(r2theta)*r # Cartedsian y

# Back Wheel
r3theta = math.pi # angular position
r3x = math.cos(r3theta)*r # Cartesian x
r3y = math.sin(r3theta)*r # Cartedsian y

# Wheel 1 velocity vector 
s1theta = r1theta + math.pi/2 # angular position vector
s1x = math.cos(s1theta) # Cartesian x
s1y = math.sin(s1theta) # Cartedsian y

# Wheel 2 velocity vector 
s2theta = r2theta + math.pi/2 # angular position vector
s2x = math.cos(s2theta) # Cartesian x
s2y = math.sin(s2theta) # Cartedsian y

# Wheel 3 velocity vector 
s3theta = r3theta + math.pi/2 # angular position vector
s3x = math.cos(s3theta) # Cartesian x
s3y = math.sin(s3theta) # Cartedsian y

# Converts body frame velocities (x,y,omega) to wheel omegas
M = (1.0/R)*matrix( [[s1x,s1y,(s1y*r1x - s1x*r1y)],
                [s2x,s2y,(s2y*r2x - s2x*r2y)],
                [s3x,s3y,(s3y*r3x - s3x*r3y)]] )

# Rotation matrix from body to world frame
R = lambda theta: matrix( [[math.cos(theta),math.sin(theta),0.0],
             [-math.sin(theta),math.cos(theta),0.0],
             [0.0,0.0,1.0]] )

# Given an x, y, and omega in the body frame, this function returns the necessary
# wheel velocites Omega1, Omega2, Omega3
# @param x: Desired velocity in x direction in body frame
# @param y: Desired velocity in y direction in body frame
# @param omega: Desired angular velocity
# @return: Omega1, Omega2, Omega3 wheel velocities (rads/s)
def getWheelVel(x,y,omega):
  	desired = matrix( [[x],
                     [y],
                     [omega]] )
	
	# Convert from body frame to wheel velocities                 
  	result = M*desired

  	return result.getA()[0][0], result.getA()[1][0], result.getA()[2][0]

# Given world frame velocities, function will return corresponding body frame velocities
# @param x: Desired velocity in x direction in world frame
# @param y: Desired velocity in y direction in world frame
# @param omega: Desired angular velocity
# @param theta: angle between body and world frames
# @return: Velocities in body frame
def getBodyXYOmega(x,y,omega,theta):
  	desired = matrix( [[x],
                     [y],
                     [omega]] )
  	desired = R(theta)*desired
  	return desired

def getBodyXYOmegaAsTuple(x, y, omega, theta):
    desired = getRobotXYOmega(x, y, omega, theta)
    asArray = desired.getA()
    return asArray[0][0], asArray[1][0], asArray[2][0]

# Wheel speeds from world frame data.
def getWheelVelTheta(x,y,omega,theta):
  	desired = getBodyXYOmega(x, y, omega, theta)
                   
  	result = M*desired

  	return result.getA()[0][0], result.getA()[1][0], result.getA()[2][0]

# Will get the wheel omegas required for world frame movement.
# @param vx_w: Velocity in the x in the world frame
# @param vy_w: Velocity in the y in the world frame
# @param omega: Angular velocity in the world (any) frame
# @param theta: Angle (degrees) between body and world frames
# @return: Wheel velocities as array [Omega1,Omega2,Omega3]
def getWheelVelFromWorld(vx_w,vy_w,omega=0,theta=0):
    # Desired velocities
    desVel = matrix([[vx_w],
                      [vy_w],
                      [omega]])
    transferMatrix = M*R(theta)
    wheelVels = transferMatrix*desVel

    return wheelVels
