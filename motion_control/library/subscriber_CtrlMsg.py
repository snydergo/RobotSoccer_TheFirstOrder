#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from robot_soccer.msg import controldata
import mlib
import sample_pid as pid
import tunePID
from sample_pid import P

class Param:
    def __init__(self):
        self.cmdType = ''
        self.x = ''
        self.y = ''
        self.theta = ''
        self.x_cmd = ''
        self.y_cmd = ''
        self.theta_cmd = ''
        
def callback1(data):
    #print data
    if data.cmdType == 'mov':

        #scale factor
        sf = .01
        # threshhold for what is considered "close enough"
        threshold = .05
        ## Decisions ##
        # if the commanded values are small enough, we are close enough. Just stop movement.
           # data.x = 0; data.y = 0; data.theta = 0;
        if data.x == data.x: # check for NaN
            vx, vy, omega = pid.robot_ctrl(data)
            # vx, vy, omega = tunePID.do_action(data)
            mlib.goXYOmegaWorld(vx,vy,omega,mlib.deg2rad(data.theta))
            print("\n")
        else:
            print("NaN - stopping")
            mlib.stop()
    elif data.cmdType == 'pid':
        for i in vars(data):
            if i in vars(P):
                setattr(P, i, eval('data.{}'.format(i)))

	
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
    ControlListener1()
