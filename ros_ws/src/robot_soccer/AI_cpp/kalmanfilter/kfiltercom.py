#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from robot_soccer.msg import visiondata


global filteredData
global newData
newData = False

def filter(raw_data):
    print raw_data
    global filteredData
    global newData
    filteredData = raw_data
    newData = True

def filterData():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.

    #subscriber information setup
    rospy.init_node('filterNode', anonymous=True)
    rospy.Subscriber("inputfilter", visiondata, filter)
    #publisher information setup
    pub = rospy.Publisher('outputfilter', visiondata, queue_size=10)
    rate = rospy.Rate(10) # 10hz


    ## Decisions ##
    
    # rate = rospy.Rate(100) # 100 Hz
    # while not rospy.is_shutdown():
    	# hello_str = "hello world %s" % rospy.get_time()
	# rospy.loginfo(hello_str)
	# pub.publish(hello_str)
	   # rate.sleep()
    while not rospy.is_shutdown():
        if newData:
            global filteredData
            publishData = filteredData
            pub.publish(publishData)
            newData = False
        rate.sleep()
    return

    # spin() simply keeps python from exiting until this node is stopped
    #rospy.spin()

if __name__ == '__main__':
    filterData()