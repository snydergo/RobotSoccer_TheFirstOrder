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


filteredData = visiondata
old_time = visiondata.sys_time
newData = False


game_pieces = lpf.GamePieces()

def filter(raw_data):
    global old_time
    global game_pieces
    global filteredData
    global newData
    if old_time != raw_data.sys_time:
        print("old_time != raw_data.sys_time")
        game_pieces.update_all(raw_data)
    else:        
        game_pieces.filter_all()
    
    newData = True
    filteredData = game_pieces.gen_msg()
    print("filteredData generated")
    

def filterData():
    global filteredData
    global newData
    #subscriber information setup
    rospy.init_node('filterNode', anonymous=True)
    rospy.Subscriber("vision_data", visiondata, filter)
    #publisher information setup
    pub = rospy.Publisher('filteredvision_data', visiondata, queue_size=10)
    rate = rospy.Rate(60) # 60hz

    ## Decisions ##
    while not rospy.is_shutdown():
        if newData:
            print("newData received")
            pub.publish(filteredData)
            newData = False
        rate.sleep()
    return

if __name__ == '__main__':
    filterData()
