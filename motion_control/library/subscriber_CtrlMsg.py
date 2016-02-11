#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from robot_soccer.msg import controldata

def callback1(data):
    #rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)

def callback2(data):
    #rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)

    
def ControlListener1():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('robot1', anonymous=True)

    rospy.Subscriber("robot1Com", controldata, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

def ControlListener2():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('robot2', anonymous=True)

    rospy.Subscriber("robot2Com", controldata, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    ControlListener1()
    ControlListener2()