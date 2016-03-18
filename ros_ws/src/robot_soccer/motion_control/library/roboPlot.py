#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from robot_soccer.msg import visiondata

class Position(object):
	def __init__(self):
		self.raw = list()
		self.fitlered = list()
		
P = Position()
        
def plot(data):
    #rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)

    
def plotter():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('plot', anonymous=True)

    # camera data
    rospy.Subscriber("inputfilter", visiondata, plot)
    # Filtered data
    rospy.Subscriber("filterNode", visiondata, plot)


    ## Decisions ##
    
    # rate = rospy.Rate(100) # 100 Hz
    # while not rospy.is_shutdown():
    	# hello_str = "hello world %s" % rospy.get_time()
	# rospy.loginfo(hello_str)
	# pub.publish(hello_str)
	   # rate.sleep()
    while not rospy.is_shutdown():
        rospy.spin()
    return

    # spin() simply keeps python from exiting until this node is stopped
    #rospy.spin()

if __name__ == '__main__':
    plotter()
