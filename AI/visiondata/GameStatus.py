"""
Contains the data received from Vision. It will have the Soccer ball and its location and velocity and predicted
value, alongside with the robots and their locations along with any other important parameters.
"""
from dataClasses import *

class AIException():
    parsingData = 0
    catchData = 1

class VisionAIException(Exception):
    errorType = 0;
    def __init__(self, errorType):
        self.errorType = errorType

    def __str__(self):
        switcher = {
            0: "Problem with parsing data\n",
            1: "unable to get data from Vision\n",
        }
        # Get the string that correlates with Exception
        return switcher.get(self.errorType, "untyped Exception")



class GameStatus(object):
    allyRobot1  = Robot("ally1",Point(0,0),Direction(0,0),0)
    allyRobot2  = Robot("ally2",Point(0,0),Direction(0,0),0)
    enemyRobot1 = Robot("enemy1",Point(0,0),Direction(0,0),0)
    enemyRobot2 = Robot("enemy2",Point(0,0),Direction(0,0),0)
    ball = FieldObject("ball",Point(0,0),Direction(0,0))

    def __init__(self, allyRobot1, allyRobot2, enemyRobot1, enemyRobot2, ball):
        self.allyRobot1 = allyRobot1
        self.allyRobot2 = allyRobot2
        self.enemyRobot1 = enemyRobot1
        self.enemyRobot2 = enemyRobot2
        self.ball = ball

    def getEnemyRobot1(self):
        return self.enemyRobot1

    def getEnemyRobot2(self):
        return self.enemyRobot2

    def getAllyRobot1(self):
        return self.allyRobot1

    def getAllyRobot2(self):
        return self.allyRobot2

    def getBall(self):
        return self.ball

    def setEnemyRobot1(self, enemyRobot1):
        self.enemyRobot1 = enemyRobot1

    def setEnemyRobot2(self, enemyRobot2):
        self.enemyRobot2 = enemyRobot2

    def setAllyRobot1(self, allyRobot1):
        self.allyRobot1 = allyRobot1

    def setAllyRobot2(self, allyRobot2):
        self.allyRobot2 = allyRobot2

    def setBall(self, ball):
        self.ball = ball


