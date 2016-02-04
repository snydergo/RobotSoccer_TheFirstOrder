"""
Contains the Data Classes that contain information about Robots, Field and etc.
It contains Direction, Point, FieldObject and etc classes
Also it contains all of the necessary enums
"""
import Gameplay.utilities
class Point(object):
    x = 0;
    y = 0;
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return "[%d,%d]" %(self.x,self.y)

class Direction():
    x_dir = 0
    y_dir = 0

    def __init__(self, x, y):
        self.x_dir = x
        self.y_dir= y

    def __str__(self):
        return "[%d,%d]" %(self.x_dir,self.y_dir)

#class that contains location and velocity of a certain object on field
class FieldObject(object):
    tag = "nothing"
    location = Point(0,0)
    v = Direction(0,0)
    def __init__(self, tag, gvnLocation, velocity):
        self.location = gvnLocation
        self.v = velocity
        self.tag = tag

    def __str__(self):
        return "tag = %s Point = %s Velocity = %s" %(self.tag,self.location, self.v)

    def getVelArray(self):
        return self.v

    def getLocation(self):
        return self.location

    def setLocation(self, x, y):
        location = Point(x,y)

    def setVelocity(self, velocity):
        v = velocity

#Robot Class inherits from FieldObject so it has all parameters and adds a theta and maybe address
class Robot(FieldObject):
    theta = 0
    utils = object()

    def __init__(self, tag, gvnLocation, velocity, theta):
        FieldObject.__init__(self,tag, gvnLocation, velocity)
        self.theta = theta

    def __str__(self):
        string = FieldObject.__str__(self) + " theta = %d" %self.theta
        return string

    def setTheta(self, theta):
        self.theta = theta

    def getTheta(self):
        return self.theta

    def addUtilities(self, utilities):
        self.utils = utilities
