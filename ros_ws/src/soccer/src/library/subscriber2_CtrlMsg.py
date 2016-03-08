#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from robot_soccer.msg import controldata
import sample_pid as pid
#import tunePID
from sample_pid import P
from geometry_msgs.msg import Vector3


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
        
def callback2(data):
#    print data
    if data.cmdType == 'mov':

        #scale factor
        sf = .01
        # threshhold for what is considered "close enough"
        threshold = .05
        ## Decisions ##
        # if the commanded values are small enough, we are close enough. Just stop movement.
#        data.x = 0; data.y = 0; data.theta = 90;
#	data.theta_cmd = 90;
	#print data
        if data.x == data.x: # check for NaN
            msg = Vector3()
            msg.x, msg.y, msg.z = pid.robot_ctrl(data)

	   # print("omega: " + str(omega))
            # vx, vy, omega = tunePID.do_action(data)
            # mlib.goXYOmegaWorld(vx,vy,omega,mlib.deg2rad(data.theta))

            #print ("callback: " + str(msg.x) + ' ' + str(msg.y) + ' ' + str(msg.z))
            P.pub.publish(msg)
          #  print("\n")
        else:
            print("NaN - stopping")
            mlib.stop()
    elif data.cmdType == 'pid':
        for i in vars(data):
            if i in vars(P):
                setattr(P, i, eval('data.{}'.format(i)))

	
    # print data
    #rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)

    
def ControlListener2():

    rospy.init_node('robot2', anonymous=True)

    rospy.Subscriber("robot2Com", controldata, callback2)

    P.pub = rospy.Publisher('/home1/command', Vector3, queue_size=10)

    while not rospy.is_shutdown():
        rospy.spin()
    return

    # spin() simply keeps python from exiting until this node is stopped
    #rospy.spin()

if __name__ == '__main__':
    ControlListener2()
