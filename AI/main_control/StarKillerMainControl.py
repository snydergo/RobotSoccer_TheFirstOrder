#!/usr/bin/env python
""" StarKillerMainControl.py Contains the most abstract level of Program that controls the Robot and Contains
all information of the Field. This is the main control of all First Order forces
"""
import Gameplay.strategies
import visiondata.GameStatus
import MathFunctions
import mlib
from time import sleep

#global that is updated by AI/visiondata/listener.py by vision and is updated periodically
glob_gameStatus = object()
glob_gameStatusUpdated = 0

class StarKillerData(object):
    robots = FieldObject("robot",Point(0,0), Direction(0,0))
    ball = FieldObject("ball",Point(0,0), Direction(0,0))
    gameStatus = GameStatus(robots, robots, robots, robots, ball)

    def __init__(self, gameStatus):
        self.gameStatus = GameStatus



if __name__=="__main__":
    #perfrom all necessary inits
    ally1 = Robot("ally1",Point(0,0),Direction(0,0),0)
    ally2 = Robot("ally2",Point(0,0),Direction(0,0),0)
    enemy1 = Robot("enemy1",Point(0,0),Direction(0,0),0)
    enemy2= Robot("enemy2",Point(0,0),Direction(0,0),0)
    ball = FieldObject("ball",Point(0,0),Direction(0,0))
    glob_gameStatus = GameStatus(ally1,ally2,enemy1,enemy2,ball)
    ##spin in circle
    goXYOmegaWorld(0,0,.5)
    sleep(2)
    stop()
    sleep(2)

    ##go forward
    goXYOmegaWorld(.1,0)
    sleep(1)
    stop()
    sleep(1)
    ##go in square
    goXYOmegaWorld(.1,0)
    sleep(.2)
    goXYOmegaWorld(-.1,0)
    sleep(.2)
    goXYOmegaWorld(0,-.1)
    sleep(.2)
    stop()


    #go to the center
    while(1):
        if(glob_gameStatusUpdated):
            toCenter = directionToPoint(glob_gameStatus.allyRobot1.getLocation(), Point(0,0))
            vx_w = 0
            vy_w = 0
            #set the velocities to approach center
            if(toCenter.x_dir < 3):
                vx_w = -0.1
            elif(toCenter.x_dir > 3):
                vx_w = 0.1

            if(toCenter.y_dir < 3):
                vy_w = -0.1
            elif(toCenter.y_dir > 3):
                vy_w = 0.1

            goXYOmegaWorld(vx_w,vy_w)



    #importer.importGameStatus(importer,"dataExample.json", deathstar)
    #NEED to Initialize GameStatus
