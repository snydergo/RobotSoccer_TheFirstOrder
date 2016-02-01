""" Python script that contains Math Functions that are necessary for calculating vector distances and finding
necessary direction vectors and Also for projecting and prediciting future locationso of ball and robots.
"""
from AI.visiondata.GameStatus import Point
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
