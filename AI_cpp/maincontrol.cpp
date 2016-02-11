#include <cstdio>
#include <thread>

#include <ros/callback_queue.h>

#include "bookkeeping.h"
#include "visiondata/subscriber_visionmsg.h"
#include "ctrldata/publisher_ctrlmsg.h"

int main(int argc, char *argv[])
{
    GameStatus game = initGameStatus();
    field.currentStatus = game;
    field.visionStatus = game;\

    ros::init(argc, argv, "visiondata_sub");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("vision_data", 1000, visionCallback);
    while (ros::ok())
    {
        if(visionUpdated){
            visionUpdated = false;
            printf("dataRecieved:\n %f, %f, %f\n", visionStatus_msg.ally1.location.x,
                   visionStatus_msg.ally1.location.y, visionStatus_msg.ally1.theta);
            field.updateStatus(&visionStatus_msg);
        }
        ros::getGlobalCallbackQueue()->callAvailable(ros::WallDuration(0.1));
    }
    return 0;
}







