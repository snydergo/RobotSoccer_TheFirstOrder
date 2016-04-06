#!/usr/bin/env python
# Node where filtering occurs. Recieves data on rostopic inputfilter
# and sends data via output filter topic.
# Author Glade Snyder
import rospy
from std_msgs.msg import String
from robot_soccer.msg import visiondata
import globals
import sample_lpf as lpf 
from numpy.matlib import matrix


filteredData = visiondata()
lastVisionData = visiondata()
newDataFlag = False
gamePieces = lpf.GamePieces()

def filter():
    global gamePieces
    global filteredData
    global lastVisionData
    global newDataFlag
    # print lastVisionData
    if newDataFlag:
        gamePieces.update_all(lastVisionData)
        newDataFlag = False    
    gamePieces.filter_all()
    filteredData = gamePieces.gen_msg()
    #print filteredData
    # print("filteredData generated")

def callback(new_data):
    global newDataFlag
    global lastVisionData
    lastVisionData = new_data
    newDataFlag = True
    

def filterData():
    global filteredData
    global newDataFlag
    #subscriber information setup
    rospy.init_node('filterNode', anonymous=True)
    rospy.Subscriber("vision_data", visiondata, callback)
    #publisher information setup
    pub = rospy.Publisher('filteredvision_data', visiondata, queue_size=10)
    rate = rospy.Rate(60) # 60hz

    ## Decisions ##
    while not rospy.is_shutdown():
        filter()
        pub.publish(filteredData)           
        rate.sleep()
    return

if __name__ == '__main__':
    filterData()
