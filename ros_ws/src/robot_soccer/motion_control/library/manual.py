import mlib
import sys
import tty
from getch import _Getch

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
    elif pressed == ',':
        return 'SPEED_DEC'
    elif pressed == '.':
        return 'SPEED_INC'
    elif pressed == 'k':
        return 'KILL'

def main():
    #default speed
    speed = .1

    while 1:
        action = get_action()
        if action == 'UP':
            mlib.goXYOmegaWorld(speed,0)
        elif action == 'LEFT':
            mlib.goXYOmegaWorld(0,speed)
        elif action == 'DOWN':
            mlib.goXYOmegaWorld(-speed,0)
        elif action == 'RIGHT':
            mlib.goXYOmegaWorld(0,-speed)
        elif action == 'SPIN_CCW':
            mlib.goXYOmegaWorld(0,0,1+speed)
        elif action == 'SPIN_CW':
            mlib.goXYOmegaWorld(0,0,-1-speed)
        elif action == 'STOP':
            mlib.stop()
        elif action == 'SPEED_DEC':
            speed = speed - .1
            print("Speed: ",speed)
        elif action == 'SPEED_INC':
            speed = speed + .1
            print("Speed: ",speed)
        elif action == 'KILL':
            return

    return

if __name__ == '__main__':
    main()