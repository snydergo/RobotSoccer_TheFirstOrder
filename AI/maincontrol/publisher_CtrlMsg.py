#!/usr/bin/env python
# license removed for brevity
import rospy
from robot_soccer.msg import controldata

def talker():
    pub1 = rospy.Publisher('robot1Com', controldata)
    pub2 = rospy.Publisher('robot2Com', controldata)
    rospy.init_node('mainhub', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        cmd1 = controldata()
        cmd1.cmdType = 'move'
        cmd1.x_dir = 1
        cmd1.y_dir = 2
        cmd1.cur_theta = 3
        cmd1.des_theta = 4
        rospy.loginfo(cmd1)
        pub1.publish(cmd1)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
