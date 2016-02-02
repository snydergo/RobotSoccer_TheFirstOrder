""" Python script that contains Math Functions that are necessary for calculating vector distances and finding
necessary direction vectors and Also for projecting and prediciting future locationso of ball and robots.
"""
from AI.visiondata.GameStatus import *
from AI.visiondata.GameStatus import FieldObject
from AI.Gameplay.utilities import Direction

#function that determines the vector difference between two Objects
def directionTo(startObject: FieldObject, destObject: FieldObject) -> Direction:
    destPoint = destObject.getLocation()
    startPoint = startObject.getLocation()
    return Direction(destPoint.x - startPoint.x, destPoint.y - startPoint.y)

#function for predicting the new location of object with its velocity vector and amount of time in future
#Time is in seconds as the velocity will be cm/second
def predictLocation(myObject : FieldObject, Time) -> Point:
    newX = myObject.location.x + myObject.v.x_dir*Time
    newY = myObject.location.y + myObject.v.y_dir*Time
    return Point(newX,newY)

#function for finding the angle difference between the desired angle and current angle
#returns negative value if CCW and positive is CW
def angleDifference(currentTheta, desiredTheta):
    angle = desiredTheta - currentTheta
    if abs(angle) > 180:
        angle = -(360 - angle)
    return angle

if __name__=="__main__":
    currentAngle = 0
    desiredAngle = -270
    print("angle is %d should be 90\n", angleDifference(currentAngle, desiredAngle))
    currentAngle = 180
    desiredAngle = -90
    print("angle is %d should be -90\n", angleDifference(currentAngle, desiredAngle))
    currentAngle = 120
    desiredAngle = -45
    print("angle is %d should be -165\n", angleDifference(currentAngle, desiredAngle))