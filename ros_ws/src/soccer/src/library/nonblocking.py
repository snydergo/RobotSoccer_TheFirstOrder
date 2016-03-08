#!/usr/bin/env python

import signal
TIMEOUT = .1 # number of seconds your want for timeout

def interrupted(signum, frame):
    "called when read times out"
    print 'interrupted!'
signal.signal(signal.SIGALRM, interrupted)

def input():
    try:
            print 'You have 5 seconds to type in your stuff...'
            foo = raw_input()
            return foo
    except:
            # timeout
            return

# set alarm
signal.alarm(TIMEOUT)
s = input()
# disable the alarm after success
signal.alarm(0)
print 'You typed', s


def main():
	while 1:
		input()

if __name__ == '__main__':
    main()