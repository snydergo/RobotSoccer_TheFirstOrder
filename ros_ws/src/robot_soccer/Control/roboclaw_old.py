import serial
import struct
import time

checksum = 0

def sendcommand(address,command):
	global checksum
	checksum = address
	port.write(chr(address));
	checksum += command
	port.write(chr(command));
	return;

def readbyte():
	global checksum
	val = struct.unpack('>B',port.read(1));
	checksum += val[0]
	return val[0];	
def readsbyte():
	global checksum
	val = struct.unpack('>b',port.read(1));
	checksum += val[0]
	return val[0];	
def readword():
	global checksum
	val = struct.unpack('>H',port.read(2));
	checksum += (val[0]&0xFF)
	checksum += (val[0]>>8)&0xFF
	return val[0];	
def readsword():
	global checksum
	val = struct.unpack('>h',port.read(2));
	checksum += val[0]
	checksum += (val[0]>>8)&0xFF
	return val[0];	
def readlong():
	global checksum
	val = struct.unpack('>L',port.read(4));
	checksum += val[0]
	checksum += (val[0]>>8)&0xFF
	checksum += (val[0]>>16)&0xFF
	checksum += (val[0]>>24)&0xFF
	return val[0];	
def readslong():
	global checksum
	val = struct.unpack('>l',port.read(4));
	checksum += val[0]
	checksum += (val[0]>>8)&0xFF
	checksum += (val[0]>>16)&0xFF
	checksum += (val[0]>>24)&0xFF
	return val[0];	

def writebyte(val):
	global checksum
	checksum += val
	return port.write(struct.pack('>B',val));
def writesbyte(val):
	global checksum
	checksum += val
	return port.write(struct.pack('>b',val));
def writeword(val):
	global checksum
	checksum += val
	checksum += (val>>8)&0xFF
	return port.write(struct.pack('>H',val));
def writesword(val):
	global checksum
	checksum += val
	checksum += (val>>8)&0xFF
	return port.write(struct.pack('>h',val));
def writelong(val):
	global checksum
	checksum += val
	checksum += (val>>8)&0xFF
	checksum += (val>>16)&0xFF
	checksum += (val>>24)&0xFF
	return port.write(struct.pack('>L',val));
def writeslong(val):
	global checksum
	checksum += val
	checksum += (val>>8)&0xFF
	checksum += (val>>16)&0xFF
	checksum += (val>>24)&0xFF
	return port.write(struct.pack('>l',val));

def M1Forward(addr,val):
	sendcommand(addr,0)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def M1Backward(addr,val):
	sendcommand(addr,1)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def SetMinMainBattery(val):
	sendcommand(128,2)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def SetMaxMainBattery(val):
	sendcommand(128,3)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def M2Forward(addr,val):
	sendcommand(addr,4)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def M2Backward(addr,val):
	sendcommand(addr,5)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def DriveM1(addr,val):
	sendcommand(addr,6)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def DriveM2(addr,val):
	sendcommand(128,7)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def ForwardMixed(val):
	sendcommand(128,8)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def BackwardMixed(val):
	sendcommand(128,9)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def RightMixed(val):
	sendcommand(128,10)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def LeftMixed(val):
	sendcommand(128,11)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def DriveMixed(val):
	sendcommand(128,12)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def TurnMixed(val):
	sendcommand(128,13)
	writebyte(val)
	writebyte(checksum&0x7F);
	return;

def readM1encoder(addr):
	sendcommand(addr,16);
	enc = readslong();
	status = readbyte();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (enc,status);
	return (-1,-1);

def readM2encoder():
	sendcommand(128,17);
	enc = readslong();
	status = readbyte();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (enc,status);
	return (-1,-1);

def readM1speed(addr):
	sendcommand(addr,18);
	enc = readslong();
	status = readbyte();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return enc
	return -1;

def readM2speed(addr):
	sendcommand(addr,19);
	enc = readslong();
	status = readbyte();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return enc
	return -1;
 
def ResetEncoderCnts(addr):
	sendcommand(addr,20)
	writebyte(checksum&0x7F);
	return;

def readversion():
	sendcommand(128,21)
	return port.read(32);

def readmainbattery():
	sendcommand(128,24);
	val = readword()
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return val
	return -1

def readlogicbattery():
	sendcommand(128,25);
	val = readword()
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return val
	return -1

def SetM1pidq(addr,p,i,d,qpps):
	sendcommand(addr,28)
	writelong(d)
	writelong(p)
	writelong(i)
	writelong(qpps)
	writebyte(checksum&0x7F);
	return;

def SetM2pidq(addr,p,i,d,qpps):
	sendcommand(addr,29)
	writelong(d)
	writelong(p)
	writelong(i)
	writelong(qpps)
	writebyte(checksum&0x7F);
	return;

def readM1instspeed(addr):
	sendcommand(addr,30);
	enc = readslong();
	status = readbyte();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (enc,status);
	return (-1,-1);

def readM2instspeed(addr):
	sendcommand(addr,31);
	enc = readslong();
	status = readbyte();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (enc,status);
	return (-1,-1);

def SetM1Duty(val):
	sendcommand(128,32)
	writesword(val)
	writebyte(checksum&0x7F);
	return;

def SetM2Duty(val):
	sendcommand(128,33)
	writesword(val)
	writebyte(checksum&0x7F);
	return;

def SetMixedDuty(m1,m2):
	sendcommand(128,34)
	writesword(m1)
	writesword(m2)
	writebyte(checksum&0x7F);
	return;

def SetM1Speed(addr,val):
	sendcommand(addr,35)
	writeslong(val)
	writebyte(checksum&0x7F);
	return;

def SetM2Speed(addr,val):
	sendcommand(addr,36)
	writeslong(val)
	writebyte(checksum&0x7F);
	return;

def SetMixedSpeed(m1,m2):
	sendcommand(128,37)
	writeslong(m1)
	writeslong(m2)
	writebyte(checksum&0x7F);
	return;

def SetM1SpeedAccel(addr,accel,speed):
	sendcommand(addr,38)
	writelong(accel)
	writeslong(speed)
	writebyte(checksum&0x7F);
	return;

def SetM2SpeedAccel(addr,accel,speed):
	sendcommand(128,39)
	writelong(accel)
	writeslong(speed)
	writebyte(checksum&0x7F);
	return;

def SetMixedSpeedAccel(accel,speed1,speed2):
	sendcommand(128,40)
	writelong(accel)
	writeslong(speed1)
	writeslong(speed2)
	writebyte(checksum&0x7F);
	return;

def SetM1SpeedDistance(addr,speed,distance,buffer):
	sendcommand(addr,41)
	writeslong(speed)
	writelong(distance)
	writebyte(buffer)
	writebyte(checksum&0x7F);
	return;

def SetM2SpeedDistance(addr,speed,distance,buffer):
	sendcommand(addr,42)
	writeslong(speed)
	writelong(distance)
	writebyte(buffer)
	writebyte(checksum&0x7F);
	return;

def SetMixedSpeedDistance(speed1,distance1,speed2,distance2,buffer):
	sendcommand(128,43)
	writeslong(speed1)
	writelong(distance1)
	writeslong(speed2)
	writelong(distance2)
	writebyte(buffer)
	writebyte(checksum&0x7F);
	return;

def SetM1SpeedAccelDistance(accel,speed,distance,buffer):
	sendcommand(128,44)
	writelong(accel)
	writeslong(speed)
	writelong(distance)
	writebyte(buffer)
	writebyte(checksum&0x7F);
	return;

def SetM2SpeedAccelDistance(accel,speed,distance,buffer):
	sendcommand(128,45)
	writelong(accel)
	writeslong(speed)
	writelong(distance)
	writebyte(buffer)
	writebyte(checksum&0x7F);
	return;

def SetMixedSpeedAccelDistance(accel,speed1,distance1,speed2,distance2,buffer):
	sendcommand(128,46)
	writelong(accel)
	writeslong(speed1)
	writelong(distance1)
	writeslong(speed2)
	writelong(distance2)
	writebyte(buffer)
	writebyte(checksum&0x7F);
	return;

def readbuffercnts():
	sendcommand(128,47);
	buffer1 = readbyte();
	buffer2 = readbyte();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (buffer1,buffer2);
	return (-1,-1);

def readcurrents():
	sendcommand(128,49);
	motor1 = readword();
	motor2 = readword();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (motor1,motor2);
	return (-1,-1);

def SetMixedSpeedIAccel(accel1,speed1,accel2,speed2):
	sendcommand(128,50)
	writelong(accel1)
	writeslong(speed1)
	writelong(accel2)
	writeslong(speed2)
	writebyte(checksum&0x7F);
	return;

def SetMixedSpeedIAccelDistance(accel1,speed1,distance1,accel2,speed2,distance2,buffer):
	sendcommand(128,51)
	writelong(accel1)
	writeslong(speed1)
	writelong(distance1)
	writelong(accel2)
	writeslong(speed2)
	writelong(distance2)
	writebyte(buffer)
	writebyte(checksum&0x7F);
	return;

def SetM1DutyAccel(accel,duty):
	sendcommand(128,52)
	writesword(duty)
	writeword(accel)
	writebyte(checksum&0x7F);
	return;

def SetM2DutyAccel(accel,duty):
	sendcommand(128,53)
	writesword(duty)
	writeword(accel)
	writebyte(checksum&0x7F);
	return;

def SetMixedDutyAccel(accel1,duty1,accel2,duty2):
	sendcommand(128,54)
	writesword(duty1)
	writeword(accel1)
	writesword(duty2)
	writeword(accel2)
	writebyte(checksum&0x7F);
	return;

def readM1pidq(addr):
	sendcommand(addr,55);
	p = readlong();
	i = readlong();
	d = readlong();
	qpps = readlong();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (p,i,d,qpps);
	return (-1,-1,-1,-1)

def readM2pidq(addr):
	sendcommand(addr,56);
	p = readlong();
	i = readlong();
	d = readlong();
	qpps = readlong();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (p,i,d,qpps);
	return (-1,-1,-1,-1)

def readmainbatterysettings():
	sendcommand(128,59);
	min = readword();
	max = readword();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (min,max);
	return (-1,-1);

def readlogicbatterysettings():
	sendcommand(128,60);
	min = readword();
	max = readword();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (min,max);
	return (-1,-1);

def SetM1PositionConstants(kp,ki,kd,kimax,deadzone,min,max):
	sendcommand(128,61)
	writelong(kd)
	writelong(kp)
	writelong(ki)
	writelong(kimax)
	writelong(deadzone);
	writelong(min);
	writelong(max);
	writebyte(checksum&0x7F);
	return;

def SetM2PositionConstants(kp,ki,kd,kimax,deadzone,min,max):
	sendcommand(128,62)
	writelong(kd)
	writelong(kp)
	writelong(ki)
	writelong(kimax)
	writelong(deadzone);
	writelong(min);
	writelong(max);
	writebyte(checksum&0x7F);
	return;

def readM1PositionConstants():
	sendcommand(128,63);
	p = readlong();
	i = readlong();
	d = readlong();
	imax = readlong();
	deadzone = readlong();
	min = readlong();
	max = readlong();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (p,i,d,imax,deadzone,min,max);
	return (-1,-1,-1,-1,-1,-1,-1)

def readM2PositionConstants():
	sendcommand(128,64);
	p = readlong();
	i = readlong();
	d = readlong();
	imax = readlong();
	deadzone = readlong();
	min = readlong();
	max = readlong();
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return (p,i,d,imax,deadzone,min,max);
	return (-1,-1,-1,-1,-1,-1,-1)

def SetM1SpeedAccelDeccelPosition(accel,speed,deccel,position,buffer):
	sendcommand(128,65)
	writelong(accel)
	writelong(speed)
	writelong(deccel)
	writelong(position)
	writebyte(buffer)
	writebyte(checksum&0x7F);
	return;

def SetM2SpeedAccelDeccelPosition(accel,speed,deccel,position,buffer):
	sendcommand(128,66)
	writelong(accel)
	writelong(speed)
	writelong(deccel)
	writelong(position)
	writebyte(buffer)
	writebyte(checksum&0x7F);
	return;

def SetMixedSpeedAccelDeccelPosition(accel1,speed1,deccel1,position1,accel2,speed2,deccel2,position2,buffer):
	sendcommand(128,67)
	writelong(accel1)
	writelong(speed1)
	writelong(deccel1)
	writelong(position1)
	writelong(accel2)
	writelong(speed2)
	writelong(deccel2)
	writelong(position2)
	writebyte(buffer)
	writebyte(checksum&0x7F);
	return;

def readtemperature():
	sendcommand(128,82);
	val = readword()
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return val
	return -1

def readerrorstate():
	sendcommand(129,90);
	val = readbyte()
	crc = checksum&0x7F
	if crc==readbyte()&0x7F:
		return val
	return -1
	
def readEncoderMode(addr):
  sendcommand(addr,91)
  mode1 = readbyte()
  mode2 = readbyte()
  crc = checksum&0x7F
  if crc==readbyte()&0x7F:
    return (mode1,mode2)
  return (-1,-1)
  
def writeSettingsToMem(addr):
  sendcommand(addr,94)
  crc = checksum&0x7F
  if crc==readbyte()&0x7F:
    return 0
  return -1

def calibrateRoboclaws():
    p = int(65536 * 4)
    i = int(65536 * 2)
    d = int(65536 * 6)
    #last good calibration readings
    voltage = 16.9 # 16.7   # 15.7   # 15.9   # 15.9   # 15.8   # 16.5   # 16.5   # 15.9   # 15.9   # 15.5   # 15.3   # 16.6   # 15.5
    qqps_m1 = 142977 # 141606 # 118234 # 129122 # 136502 # 140181 # 146772 # 130185 # 146330 # 149353 # 137669 # 141136 # 148132 # 149287
    qqps_m2 = 178091 # 187808 # 139632 # 159086 # 164265 # 164244 # 177244 # 180669 # 180616 # 166407 # 172434 # 165175 # 168984 # 169069
    qqps_m3 = 195319 # 175863 # 130377 # 154211 # 171489 # 165285 # 183906 # 181536 # 175021 # 170281 # 159700 # 161999 # 165146 # 164071

    read_v = readmainbattery() / 10.0
    
    scale = lambda x: int(x*voltage/read_v)
    speedM1 = scale(qqps_m1)
    speedM2 = scale(qqps_m2)
    speedM3 = scale(qqps_m3)
    
    SetM1pidq(128,p,i,d,speedM1)
    SetM2pidq(128,p,i,d,speedM2)
    SetM1pidq(129,p,i,d,speedM3)

#print "Roboclaw Example 1\r\n"

#Rasberry Pi/Linux Serial instance example
port = serial.Serial("/dev/ttySAC0", baudrate=38400, timeout=1)

#Windows Serial instance example
#port = serial.Serial("COM126", baudrate=38400, timeout=1)

# M1Forward(0x80,127)
# M1Forward(0x80,0)
# ## Get version string
# sendcommand(128,21)
# rcv = port.read(32)
# print repr(rcv)

# sendcommand(129,90)
# rcv = port.read(1)
# print repr(rcv)

# cnt = 0
# while True:
# 	cnt=cnt+1
# 	print "Count = ",cnt
	
# 	print "Error State:",repr(readerrorstate())

# 	print "Temperature:",readtemperature()/10.0

# 	print "Main Battery:",readmainbattery()/10.0
	
# 	print "Logic Battery:",readlogicbattery()/10.0

# 	m1cur, m2cur = readcurrents();
# 	print "Current M1: ",m1cur/100.0," M2: ",m2cur/100.0
	
# 	min, max = readlogicbatterysettings()
# 	print "Logic Battery Min:",min/10.0," Max:",max/10.0

# 	min, max = readmainbatterysettings()
# 	print "Main Battery Min:",min/10.0," Max:",max/10.0

# 	p,i,d,qpps = readM1pidq()
# 	print "M1 P=%.2f" % (p/65536.0)
# 	print "M1 I=%.2f" % (i/65536.0)
# 	print "M1 D=%.2f" % (d/65536.0)
# 	print "M1 QPPS=",qpps

# 	p,i,d,qpps = readM2pidq()
# 	print "M2 P=%.2f" % (p/65536.0)
# 	print "M2 I=%.2f" % (i/65536.0)
# 	print "M2 D=%.2f" % (d/65536.0)
# 	print "M2 QPPS=",qpps

# 	SetM1DutyAccel(1500,1500)
# 	SetM2DutyAccel(1500,-1500)
# 	M1Forward(0x80,127);
# 	M2Backward(0x80,127);
# 	time.sleep(2)
# 	SetM1DutyAccel(1500,-1500)
# 	SetM2DutyAccel(1500,1500)
# 	M1Forward(0);
# 	M2Backward(0);
# 	time.sleep(10);
# 	M1Backward(0x80,127)
# 	M2Forward(0x80,127);
# 	time.sleep(2)

