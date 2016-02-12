import mlib
import sys
import tty

tty.setcbreak(sys.stdin)

def drive():
	w = 119
	a = 97
	s = 115
	d = 100
	q = 113
	e = 101
	space = 32

	while True:
	    pressed = ord(sys.stdin.read(1))
	    if pressed == w:
	    	mlib.goXYOmegaWorld(.1,0)
	    elif pressed == a:
	    	mlib.goXYOmegaWorld(0,.1)
	    elif pressed == s:
	    	mlib.goXYOmegaWorld(-.1,0)
	    elif pressed == d:
	    	mlib.goXYOmegaWorld(-.1,0)
	    elif pressed == q:
	    	mlib.goXYOmegaWorld(0,0,1)
	    elif pressed == e:
	    	mlib.goXYOmegaWorld(0,0,-1)
	    elif pressed == space:
	    	mlib.stop()

	return