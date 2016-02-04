"""Contains all of the utilities that a Robot should be able to perform.
   This File has the actual embedded programming that talks with the hardware
   This will be using functions that the Motion Control guy uses
"""

import MathFunctions
import dataClasses

class Rotation():
    CCW = 1
    CW = -1
    noRotation = 0

class MtrCtrlParam(Exception):
    errorType = 0
    def __init__(self, errorType):
        self.errorType = errorType
    def __str__(self):
        return "incorrect parameters passed in"

class  Utilities(object):
    name = ""
    robotNum = 0
    role = "nothing"

    """Simple constructor """
    def __init__(self, name, robotNum, role):
        """Contains robotNumber so one knows which Robot to send Commands to"""
        self.robotNum = robotNum
        self.name = name
        self.role = role;
        print ("Number = %d, name = %s, role = %s" % (self.robotNum, self.name, self.role))


    """angle is in degrees and direction is CW = -1 and CCW = -1"""
    def rotate(self, rotation, angle):
        if (rotation == Rotation.CCW):
            raise MtrCtrlParam(1)
        else:
            print ("direction = %d angle = %d." % (rotation, angle))

    """
    move in a specified direction enumerated right, left, back, forward
    also moves a certain distance measured in feet
    """
    def moveToPoint(self, point):
        print("moving to point [%d,%d]" %(point.x, point.y))

    def move(self, direction, distance):
        print ("direction = %d angle = %d." % (direction, distance))

    def kick(self):
        print ("ball Kicked!")

    def followBall(self):
        print ("Following Ball")

    def moveToCenter(self, allyRobot):
        movVector = directionToPoint(allyRobot.getLocation(), Point(0,0))
        

if __name__=="__main__":
   robot1 = Utilities( "Supreme Leader", 1, "Goalie")
   rotation = Rotation.CCW
   try:
       robot1.rotate(rotation, 90)
   except MtrCtrlParam as e:
       print (e)

   robot1.move(Direction.Right, 20)
   robot1.kick()
   robot1.followBall()
    
