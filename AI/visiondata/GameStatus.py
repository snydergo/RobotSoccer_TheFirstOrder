"""
Contains the data received from Vision. It will have the Soccer ball and its location and velocity and predicted
value, alongside with the robots and their locations along with any other important parameters.
"""
from AI.Gameplay.utilities import Direction

class VisionAI(Exception):
    errorType = 0;
    def __init__(self, errorType):
        self.errorType = errorType
    def __str__(self):
        return "incorrect parameters passed in"

class Point(object):
    x = 0;
    y = 0;
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return "[%d,%d]\n" %(self.x,self.y)

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

class Robot(FieldObject):
    theta = 0
    address = 0
    def __init__(self, gvnLocation: Point, velocity: Direction, theta, ):
        FieldObject.__init__(gvnLocation, velocity)
        self.theta = theta




class GameStatus(object):
    allyRobot1 = FieldObject(Point(0,0),Direction(0,0))
    allyRobot2 = FieldObject(Point(0,0),Direction(0,0))
    enemyRobot1 = FieldObject(Point(0,0),Direction(0,0))
    enemyRobot2 = FieldObject(Point(0,0),Direction(0,0))
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


