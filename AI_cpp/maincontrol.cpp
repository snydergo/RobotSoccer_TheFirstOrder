#include <cstdio>

#include <ros/callback_queue.h>

#include "bookkeeping.h"
#include "visiondata/subscriber_visionmsg.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "visiondata_sub");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("vision_data", 1000, visionCallback);
    while (ros::ok())
    {
        if(visionUpdated){
            visionUpdated = false;
            std::cout << "dataRecieved: " << visionStatus_msg.ally1.location.x << " " <<
                         visionStatus_msg.ally1.location.y << " " << visionStatus_msg.ally1.theta << std::endl;
            field.updateStatus(visionStatus_msg);
        }
        ros::getGlobalCallbackQueue()->callAvailable(ros::WallDuration(0.1));
    }
    return 0;
}







