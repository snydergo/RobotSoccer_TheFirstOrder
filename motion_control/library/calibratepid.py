#!/usr/bin/python
from roboclaw import *
import mlib
import time

p = int(65536 * 4) #262144
i = int(65536 * 2) #131072
d = int(65536 * 6)  #65536
q = 308419

print readmainbattery()
p1,i1,d1,q1 = readM1pidq(128)
print "128 M1 P=%.2f" % (p1/65536.0)
print "128 M1 I=%.2f" % (i1/65536.0)
print "128 M1 D=%.2f" % (d1/65536.0)
print "128 M1 QPPS=",q1
p2,i2,d2,q2 = readM2pidq(128)
print "128 M2 P=%.2f" % (p2/65536.0)
print "128 M2 I=%.2f" % (i2/65536.0)
print "128 M2 D=%.2f" % (d2/65536.0)
print "128 M2 QPPS=",q2
p3,i3,d3,q3 = readM1pidq(129)
print "121 M1 P=%.2f" % (p3/65536.0)
print "129 M1 I=%.2f" % (i3/65536.0)
print "129 M1 D=%.2f" % (d3/65536.0)
print "129 M1 QPPS=",q3

def read(addr,motor):
  samples = 4
  result = 0
  for i in range(0, samples):
    sample = 0
    if motor == 1:
      sample = readM1speed(addr)
    elif motor == 2:
      sample = readM2speed(addr)
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

#Forwards
M1Backward(128,speed); #M1 backward sample 1
M2Forward(128,speed); #M2 forward sample 1
time.sleep(2)

speedM1Backward=speedM1Backward+read(128,1)
speedM2Forward=speedM2Forward+read(128,2)

mlib.stop();
time.sleep(1);

#Backwards
M1Forward(128,speed); #M1 forward sample 1
M2Backward(128,speed); #M2 backward sample 1
time.sleep(2)

speedM1Forward=speedM1Forward+read(128,1)
speedM2Backward=speedM2Backward+read(128,2)

mlib.stop();
time.sleep(1);

#Left back
M2Backward(128,speed); #M2 backward sample 2 
M1Forward(129,speed); #M3 forward sample 1
time.sleep(2)

speedM2Backward=speedM2Backward+read(128,2)
speedM2Backward=speedM2Backward/2
speedM3Forward=speedM3Forward+read(129,1)

mlib.stop();
time.sleep(1);

#Left forward
M2Forward(128,speed); #M2 forward sample 2
M1Backward(129,speed); #M3 backward sample 1
time.sleep(2)

speedM2Forward=speedM2Forward+read(128,2)
speedM2Forward=speedM2Forward/2
speedM3Backward=speedM3Backward+read(129,1)

mlib.stop();
time.sleep(1);

# RightBack
M1Forward(128,speed); #M1 forward sample 2
M1Backward(129,speed); #M3 backward sample 2
time.sleep(2)

speedM1Forward=speedM1Forward+read(128,1)
speedM1Forward=speedM1Forward/2
speedM3Backward=speedM3Backward+read(129,1)
speedM3Backward=speedM3Backward/2

mlib.stop();
time.sleep(1);

# Right Forward
M1Backward(128,speed); #M1 backward sample 2
M1Forward(129,speed); #M3 forward sample 2
time.sleep(2)

speedM1Backward=speedM1Backward+read(128,1)
speedM1Backward=speedM1Backward/2
speedM3Forward=speedM3Forward+read(129,1)
speedM3Forward=speedM3Forward/2

mlib.stop();

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

SetM1pidq(128,p,i,d,speedM1)
SetM2pidq(128,p,i,d,speedM2)
SetM1pidq(129,p,i,d,speedM3)

p1,i1,d1,q1 = readM1pidq(128)
print "128 M1 P=%.2f" % (p1/65536.0)
print "128 M1 I=%.2f" % (i1/65536.0)
print "128 M1 D=%.2f" % (d1/65536.0)
print "128 M1 QPPS=",q1
p2,i2,d2,q2 = readM2pidq(128)
print "128 M2 P=%.2f" % (p2/65536.0)
print "128 M2 I=%.2f" % (i2/65536.0)
print "128 M2 D=%.2f" % (d2/65536.0)
print "128 M2 QPPS=",q2
p3,i3,d3,q3 = readM1pidq(129)
print "121 M1 P=%.2f" % (p3/65536.0)
print "129 M1 I=%.2f" % (i3/65536.0)
print "129 M1 D=%.2f" % (d3/65536.0)
print "129 M1 QPPS=",q3