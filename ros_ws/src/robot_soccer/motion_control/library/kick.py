#!/usr/bin/env python
import os
import roboclaw

def init_kick():
	# Hold the kicker back
	roboclaw.ForwardM2(roboclaw.addr2,127) # max power

def uninit_kick():
	roboclaw.ForwardM2(roboclaw.addr2, 0)

def kick():
  os.system("echo 0 > /sys/class/gpio/gpio204/value; sleep .1; echo 1 > /sys/class/gpio/gpio204/value")

if __name__ == '__main__':
  kick()