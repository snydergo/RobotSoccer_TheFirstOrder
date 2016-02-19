#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from robot_soccer.msg import controldata
import mlib

class Param:
    def __init__(self):
        cmdType = ''
        x_c = ''
        y_c = ''
        theta = ''
        theta_c = ''
        
P = Param()

def callback1(data):
    P.cmdType = data.cmdType
    if P.cmdType == 'mov':
        P.x_c = data.x_dir
        P.y_c = data.y_dir
        P.theta = data.cur_theta
        P.theta_c = data.des_theta

    # print data
    #rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)

    
def ControlListener1():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('robot1', anonymous=True)

    rospy.Subscriber("robot1Com", controldata, callback1)

    #scale factor
    sf = .01
    # threshhold for what is considered "close enough"
    threshold = .05
    ## Decisions ##
    while not rospy.is_shutdown():
        # ignoring theta for now
        # if the commanded values are small enough, we are close enough. Just stop movement.
        if P.x_c < threshold and P.y_x < threshold:
            mlib.stop()
        else:
            mlib.goXYOmegaWorld(sf*P.x_c,sf*P.y_c)

        rospy.spinOnce()
        return

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    ControlListener1()
