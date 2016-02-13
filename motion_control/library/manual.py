import mlib
import sys
import tty
from Getch import _Getch

tty.setcbreak(sys.stdin)

def get_action():
    getch = _Getch()
    pressed = getch()
    if pressed == 'w':
        return 'UP'
    elif pressed == 'a':
        return 'LEFT'
    elif pressed == 's':
        return 'DOWN'
    elif pressed == 'd':
        return 'RIGHT'
    elif pressed == 'q':
        return 'SPIN_CCW'
    elif pressed == 'e':
        return 'SPIN_CW'
    elif pressed == ' ':
        return 'STOP'
    elif pressed == 'k':
        return 'KILL'

def main():
    while 1:
        action = get_action()
        if action == 'UP':
            mlib.goXYOmegaWorld(.1,0)
        elif action == 'LEFT'
            mlib.goXYOmegaWorld(0,.1)
        elif action == 'DOWN'
            mlib.goXYOmegaWorld(-.1,0)
        elif action == 'RIGHT'
            mlib.goXYOmegaWorld(0,-.1)
        elif action == 'SPIN_CCW'
            mlib.goXYOmegaWorld(0,0,1)
        elif action == 'SPIN_CW'
            mlib.goXYOmegaWorld(0,0,-1)
        elif action == 'STOP'
            mlib.stop()
        elif action == 'KILL'
            return

    return

if __name__ == '__main__':
    main()