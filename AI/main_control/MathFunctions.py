""" Python script that contains Math Functions that are necessary for calculating vector distances and finding
necessary direction vectors and Also for projecting and prediciting future locationso of ball and robots.
"""
import dataClasses

#function that determines the vector difference between two Objects and calculates predicted location
def directionToPredict(startObject, destObject, Time):
    startPoint = startObject.getLocation()
    predictedLocation = predictLocation(destObject, Time)
    return directionToPoint(startPoint, predictedLocation)

def directionToPoint(startPoint, destPoint):
    return Direction(destPoint.x - startPoint.x, destPoint.y - startPoint.y)

#function for predicting the new location of object with its velocity vector and amount of time in future
#Time is in seconds as the velocity will be cm/second
def predictLocation(myObject, Time):
    newX = myObject.location.x + myObject.v.x_dir*Time
    newY = myObject.location.y + myObject.v.y_dir*Time
    return Point(newX,newY)

#function for finding the angle difference between the desired angle and current angle
#returns negative value if CCW and positive is CW
def angleDifference(currentTheta, desiredTheta):
    angle = currentTheta - desiredTheta
    if angle < -180:
        angle = -(360 - abs(angle))
    elif angle > 180:
        angle = 360 - angle
    else:
        angle = -angle
    return angle

if __name__=="__main__":
    print("###Testing Math Functions###")
    print("--testing angleDifference function--")
    currentAngle = 0
    desiredAngle = 90
    print("angle is %d should be 90" %angleDifference(currentAngle, desiredAngle))
    currentAngle = 180
    desiredAngle = 270
    print("angle is %d should be 90" %angleDifference(currentAngle, desiredAngle))
    currentAngle = 120
    desiredAngle = 315
    print("angle is %d should be -165" %angleDifference(currentAngle, desiredAngle))
    currentAngle = 45
    desiredAngle = 315
    print("angle is %d should be -90" %angleDifference(currentAngle, desiredAngle))
    currentAngle = 315
    desiredAngle = 45
    print("angle is %d should be 90" %angleDifference(currentAngle, desiredAngle))
    currentAngle = 240
    desiredAngle = 0
    print("angle is %d should be 120" %angleDifference(currentAngle, desiredAngle))
    currentAngle = 120
    desiredAngle = 0
    print("angle is %d should be -120" %angleDifference(currentAngle, desiredAngle))
    currentAngle = 0
    desiredAngle = 0
    print("angle is %d should be 0" %angleDifference(currentAngle, desiredAngle))
    currentAngle = 180
    desiredAngle = 0
    print("angle is %d should be -/+180\n" %angleDifference(currentAngle, desiredAngle))

    print("--testing directionToPoint function--")
    print("direction vector = %s should be 2,2" %directionToPoint(Point(0,0), Point(2,2)))
    print("direction vector = %s should be -1,-2" %directionToPoint(Point(3,4), Point(2,2)))
    print("direction vector = %s should be 10,-8" %directionToPoint(Point(5,4), Point(-5,-4)))
    print("direction vector = %s should be 1,-1\n" %directionToPoint(Point(1,2), Point(2,1)))

    print("--testing predict location--")
    myObject = FieldObject(Point(0,0),Direction(2,2))
    Time = 1
    print("object predicted location %s should be 2,2" %predictLocation(myObject, Time))
    Time = 2
    print("object predicted location %s should be 4,4" %predictLocation(myObject, Time))
    myObject = FieldObject(Point(-2,-1),Direction(-2,-1))
    Time = 1
    print("object predicted location %s should be -4,-2" %predictLocation(myObject, Time))
    Time = 2
    print("object predicted location %s should be -6,-3" %predictLocation(myObject, Time))
    myObject = FieldObject(Point(-2,-3),Direction(2,3))
    Time = 1
    print("object predicted location %s should be 0,0" %predictLocation(myObject, Time))
    Time = 2
    print("object predicted location %s should be 2,3\n" %predictLocation(myObject, Time))

    print("--testing gotToPointPredict--")
    myObject = FieldObject(Point(0,0),Direction(2,3))
    destObject = FieldObject(Point(-2,-3),Direction(2,3))
    Time = 1
    print("dir to Predicted = %s should be 0,0"%directionToPredict(myObject, destObject, Time))
    Time = 2
    print("dir to Predicted = %s should be 2,3"%directionToPredict(myObject, destObject, Time))

    myObject = FieldObject(Point(1,3),Direction(2,3))
    destObject = FieldObject(Point(-1,-3),Direction(-2,-3))
    Time = 1
    print("dir to Predicted = %s should be -4,-9"%directionToPredict(myObject, destObject, Time))
    Time = 2
    print("dir to Predicted = %s should be -6,-12"%directionToPredict(myObject, destObject, Time))

    myObject = FieldObject(Point(-1,5),Direction(2,3))
    destObject = FieldObject(Point(4,-3),Direction(2,-3))
    Time = 1
    print("dir to Predicted = %s should be 7,-11"%directionToPredict(myObject, destObject, Time))
    Time = 2
    print("dir to Predicted = %s should be 9,-14"%directionToPredict(myObject, destObject, Time))
