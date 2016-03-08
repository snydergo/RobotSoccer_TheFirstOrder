#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from robot_soccer.msg import controldata
import sample_pid as pid
#import tunePID
from geometry_msgs.msg import Vector3
import globals


class Param:
    def __init__(self):
        self.cmdType = ''
        self.x = ''
        self.y = ''
        self.theta = ''
        self.x_cmd = ''
        self.y_cmd = ''
        self.theta_cmd = ''
        self.pub = ''

P1 = Param()
P2 = Param()

def callback1(data):
#    print data
    if data.cmdType == 'mov' and data.x == data.x:
        msg = Vector3()
        msg.x, msg.y, msg.z = pid.robot_ctrl(data)
        P1.pub.publish(msg)


def callback2(data):
#    print data
    if data.cmdType == 'mov' and data.x == data.x:
        msg = Vector3()
        msg.x, msg.y, msg.z = pid.robot_ctrl(data)
        P2.pub.publish(msg)

    
def ControlListener():

    rospy.init_node('robot_motion_control', anonymous=True)

    rospy.Subscriber("robot1Com", controldata, callback1)

    P1.pub = rospy.Publisher('/home1/command', Vector3, queue_size=10)

    rospy.Subscriber("robot2Com", controldata, callback2)

    P2.pub = rospy.Publisher('/home2/command', Vector3, queue_size=10)

    while not rospy.is_shutdown():
        rospy.spin()
    return

    # spin() simply keeps python from exiting until this node is stopped
    #rospy.spin()

if __name__ == '__main__':
    ControlListener()
