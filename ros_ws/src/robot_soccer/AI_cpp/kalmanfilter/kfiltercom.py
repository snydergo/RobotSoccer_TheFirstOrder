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


global filteredData
global newData
newData = False

def filter(raw_data):
    # print raw_data
    temp = lpf.GamePiece()
    temp.position = 
    global filteredData
    global newData
    filteredData = raw_data
    newData = True

def filterData():
    #subscriber information setup
    rospy.init_node('filterNode', anonymous=True)
    rospy.Subscriber("inputfilter", visiondata, filter)
    #publisher information setup
    pub = rospy.Publisher('outputfilter', visiondata, queue_size=10)
    rate = rospy.Rate(10) # 10hz

    ## Decisions ##
    while not rospy.is_shutdown():
        if newData:
            global filteredData
            publishData = filteredData
            pub.publish(publishData)
            newData = False
        rate.sleep()
    return

if __name__ == '__main__':
    filterData()
