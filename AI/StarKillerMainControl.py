""" StarKillerMainControl.py Contains the most abstract level of Program that controls the Robot and Contains
all information of the Field. This is the main control of all First Order forces
"""
from AI.Gameplay.strategies import *
from AI.visiondata.GameStatus import *

class StarKillerData(object):
    robots = FieldObject("robot",Point(0,0), Direction(0,0))
    ball = FieldObject("ball",Point(0,0), Direction(0,0))
    gameStatus = GameStatus(robots, robots, robots, robots, ball)

    def __init__(self, gameStatus : GameStatus ):
        self.gameStatus = GameStatus



if __name__=="__main__":
    print("peform needed Commands")