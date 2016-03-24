#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from robot_soccer.msg import visiondata
from scipy import io
import numpy as np
import time
import csv

# class Position(object):
# 	def __init__(self):
# 		self.raw = list()
# 		self.fitlered = list()
		
# P = Position()

# io.savemat('filter_ball.mat',{'time':np.zeros(2)})  # write
# io.savemat('ball.mat',{'time':np.zeros(2)})  # write
        
def plot_vision(data):
    with open("ball.csv", "a") as fp:
        wr = csv.writer(fp, dialect='excel')
        wr.writerow([data.ball_x, data.ball_y])

def plot_filtered(data):
    with open("filter_ball.csv", "a") as fp:
        wr = csv.writer(fp, dialect='excel')
        wr.writerow([data.ball_x, data.ball_y])
        
def plotter():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('plot', anonymous=True)

    # camera data
    rospy.Subscriber("vision_data", visiondata, plot_vision)
    # Filtered data
    rospy.Subscriber("filteredvision_data", visiondata, plot_filtered)


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
