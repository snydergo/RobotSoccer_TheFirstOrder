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
global old_time
global newData = False


game_pieces = lpf.GamePieces()

def filter(raw_data):
    if old_time != raw_data.time:
        game_pieces.update_all(raw_data)
    else:        
        game_pieces.filter_all()
    
    newData = True

    filteredData = game_pieces.gen_msg()
    
    

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
            pub.publish(filteredData)
            newData = False
        rate.sleep()
    return

if __name__ == '__main__':
    filterData()
