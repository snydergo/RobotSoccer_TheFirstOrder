#!/usr/bin/python
from roboclaw import *
import mlib
import time

# Inital Values
p = 1 
i = .25 
d = .5  
q = 44000

def default():
  for k in range(1,4):
    mlib.setMVelocityPID(k,p,i,d,q)

def calibrate():
  print "==Initial Values==\n"
  print 'Battery Voltage: ' + str(ReadMainBatteryVoltage(128))
  crc,p1,i1,d1,q1 = mlib.readMVelocityPID(1)
  print "M1 P= ",p1
  print "M1 I= ",i1
  print "M1 D= ",d1
  print "M1 QPPS=",q1
  crc,p2,i2,d2,q2 = mlib.readMVelocityPID(2)
  print "M2 P= ",p2
  print "M2 I= ",i2
  print "M2 D= ",d2
  print "M2 QPPS= ",q2
  crc,p3,i3,d3,q3 = mlib.readMVelocityPID(3)
  print "M3 P= ",p3
  print "M3 I= ",i3
  print "M3 D= ",d3
  print "M3 QPPS= ",q3

  # This function takes multiple samples of the motor speed and then returns the average
  def read(motor):
    samples = 2
    result = 0
    for k in range(0, samples):
      sample = 0
      sample = mlib.readSpeedM(motor)[1]
      #print sample
      result = result + sample
    result = result/samples
    return result
    
  speedM1Forward=0
  speedM1Backward=0
  speedM2Forward=0
  speedM2Backward=0
  speedM3Forward=0
  speedM3Backward=0

  speed = 48

  mlib.stop();

  ## Forward Back
  #Forwards
  mlib.BackwardM(1,speed); #M1 backward sample 1
  mlib.ForwardM(3,speed); #M3 forward sample 1
  time.sleep(2)

  speedM1Backward=speedM1Backward+read(1)
  speedM3Forward=speedM3Forward+read(3)

  mlib.stop();
  time.sleep(1);

  #Backwards
  mlib.ForwardM(1,speed); #M1 forward sample 1
  mlib.BackwardM(3,speed); #M3 backward sample 1
  time.sleep(2)

  speedM1Forward=speedM1Forward+read(1)
  speedM3Backward=speedM3Backward+read(3)

  mlib.stop();
  time.sleep(5);

  ## Diagonal 1
  #Left back
  mlib.BackwardM(2,speed); #M2 backward sample 1 
  mlib.ForwardM(1,speed); #M1 forward sample 2
  time.sleep(2)

  speedM2Backward=speedM2Backward+read(2)
  speedM1Forward=speedM1Forward+read(1)
  speedM1Forward=speedM1Forward/2

  mlib.stop();
  time.sleep(1);

  #Left forward
  mlib.ForwardM(2,speed); #M2 forward sample 1
  mlib.BackwardM(1,speed); #M1 backward sample 2
  time.sleep(2)

  speedM2Forward=speedM2Forward+read(2)
  speedM1Backward=speedM1Backward+read(1)
  speedM1Backward=speedM1Backward/2

  mlib.stop();
  time.sleep(5);

  ## Diagonal 2
  # RightBack
  mlib.ForwardM(2,speed); #M2 forward sample 2
  mlib.BackwardM(3,speed); #M3 backward sample 2
  time.sleep(2)

  speedM2Forward=speedM2Forward+read(2)
  speedM2Forward=speedM2Forward/2
  speedM3Backward=speedM3Backward+read(3)
  speedM3Backward=speedM3Backward/2

  mlib.stop();
  time.sleep(1);

  # Right Forward
  mlib.BackwardM(2,speed); #M2 backward sample 2
  mlib.ForwardM(3,speed); #M3 forward sample 2
  time.sleep(2)

  speedM2Backward=speedM2Backward+read(2)
  speedM2Backward=speedM2Backward/2
  speedM3Forward=speedM3Forward+read(3)
  speedM3Forward=speedM3Forward/2

  mlib.stop();

  # Make Calculations
  speedM1Forward=(speedM1Forward*127)/speed
  speedM1Backward=(speedM1Backward*127)/speed
  speedM2Forward=(speedM2Forward*127)/speed
  speedM2Backward=(speedM2Backward*127)/speed
  speedM3Forward=(speedM3Forward*127)/speed
  speedM3Backward=(speedM3Backward*127)/speed

  #print speedM1Forward;
  #print speedM1Backward;
  #print speedM2Forward;
  #print speedM2Backward;
  #print speedM3Forward;
  #print speedM3Backward;

  speedM1 = (speedM1Forward - speedM1Backward)/2
  speedM2 = (speedM2Forward - speedM2Backward)/2
  speedM3 = (speedM3Forward - speedM3Backward)/2
  print "qpps values:"
  print speedM1
  print speedM2
  print speedM3
  mlib.setMVelocityPID(1,p,i,d,speedM1)
  mlib.setMVelocityPID(2,p,i,d,speedM2)
  mlib.setMVelocityPID(3,p,i,d,speedM3)

  print "==Final Values==\n"
  crc,p1,i1,d1,q1 = mlib.readMVelocityPID(1)
  print "M1 P= ",p1
  print "M1 I= ",i1
  print "M1 D= ",d1
  print "M1 QPPS= ",q1
  crc,p2,i2,d2,q2 = mlib.readMVelocityPID(2)
  print "M2 P= ",p2
  print "M2 I= ",i2
  print "M2 D= ",d2
  print "M2 QPPS= ",q2
  crc,p3,i3,d3,q3 = mlib.readMVelocityPID(3)
  print "M3 P= ",p3
  print "M3 I= ",i3
  print "M3 D= ",d3
  print "M3 QPPS=",q3
