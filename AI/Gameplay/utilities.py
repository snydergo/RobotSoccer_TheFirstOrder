"""Contains all of the utilities that a Robot should be able to perform.
   This File has the actual embedded programming sends commands to The Robots
   This will be using functions that communicates with MotionControl
"""

#imports for ros
import MathFunctions
import dataClasses

"""
#pycharm imports
from AI.maincontrol.mathFunctions import *
from AI.maincontrol.dataClasses import *
from robot_soccer.msg import controldata
"""
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
    def move(self, robot, direction, des_theta):
        print ("direction = %d angle = %d." % (direction, des_theta))
        msg = controldata()
        msg.x_dir = direction.x_dir
        msg.y_dir = direction.y_dir
        msg.cur_theta = robot.theta
        msg.des_theta = des_theta

    def moveToPoint(self, robot, point):
        print("moving to point [%d,%d]" %(point.x, point.y))
        direction = directionToPoint(robot.location, point)
        self.move(self, robot, direction, 0)

    def kick(self):
        print ("ball Kicked!")

    #robot follows the ball and keeps facing towards the ball
    def followBall(self, ball, robot):
        print ("Following Ball")
        movVector = directionToPoint(robot.location, ball.location)
        #need to determine the theta necessary to turn to
        self.move(self, robot, movVector, 0)



    def moveToCenter(self, allyRobot):
        movVector = directionToPoint(allyRobot.getLocation(), Point(0,0))
        self.move(self, allyRobot, movVector, 0)
        

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
    
