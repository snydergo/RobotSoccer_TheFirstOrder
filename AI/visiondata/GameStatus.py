"""
Contains the data received from Vision. It will have the Soccer ball and its location and velocity and predicted
value, alongside with the robots and their locations along with any other important parameters.
"""
from AI.Gameplay.utilities import Direction
from enum import Enum

class AIException(Enum):
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


class Point(object):
    x = 0;
    y = 0;
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return "[%d,%d]\n" %(self.x,self.y)

#class that contains location and velocity of a certain object on field
class FieldObject(object):
    location = Point(0,0)
    v = Direction(0,0)
    def __init__(self, gvnLocation: Point, velocity: Direction) -> object:
        self.location = gvnLocation
        self.v = velocity

    def __str__(self):
        return "Point = [%d,%d] velocity = [%d,%d]\n" %(self.Point.x,self.Point.y, self.v[1], self.v[2])

    def getVelArray(self):
        return self.v

    def getLocation(self) -> Point:
        return self.location

    def setLocation(self, x, y):
        location = Point(x,y)

    def setVelocity(self, velocity: Direction):
        v = velocity

#Robot Class inherits from FieldObject so it has all parameters and adds a theta and maybe address
class Robot(FieldObject):
    theta = 0
    def __init__(self, gvnLocation: Point, velocity: Direction, theta):
        FieldObject.__init__(gvnLocation, velocity)
        self.theta = theta

    def setTheta(self, theta):
        self.theta = theta

    def getTheta(self):
        return self.theta




class GameStatus(object):
    allyRobot1 = Robot(Point(0,0),Direction(0,0),0)
    allyRobot2 = Robot(Point(0,0),Direction(0,0),0)
    enemyRobot1 = Robot(Point(0,0),Direction(0,0),0)
    enemyRobot2 = Robot(Point(0,0),Direction(0,0),0)
    ball = FieldObject(Point(0,0),Direction(0,0))

    def __init__(self, allyRobot1: FieldObject, allyRobot2: FieldObject, enemyRobot1: FieldObject, enemyRobot2: FieldObject, ball: FieldObject):
        self.allyRobot1 = allyRobot1
        self.allyRobot2 = allyRobot2
        self.enemyRobot1 = enemyRobot1
        self.enemyRobot2 = enemyRobot2
        self.ball = ball

    def getEnemyRobot1(self) -> FieldObject:
        return self.enemyRobot1

    def getEnemyRobot2(self) -> FieldObject:
        return self.enemyRobot2

    def getAllyRobot1(self) -> FieldObject:
        return self.allyRobot1

    def getAllyRobot2(self) -> FieldObject:
        return self.allyRobot2

    def getBall(self) -> FieldObject:
        return self.ball

    def setEnemyRobot1(self, enemyRobot1: FieldObject):
        self.enemyRobot1 = enemyRobot1

    def setEnemyRobot2(self, enemyRobot2: FieldObject):
        self.enemyRobot2 = enemyRobot2

    def setAllyRobot1(self, allyRobot1: FieldObject):
        self.allyRobot1 = allyRobot1

    def setAllyRobot2(self, allyRobot2: FieldObject):
        self.allyRobot2 = allyRobot2

    def setBall(self, ball: FieldObject):
        self.ball = ball


