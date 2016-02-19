#include <cstdio>
#include <ros/callback_queue.h>
#include "bookkeeping.h"
#include "visiondata/subscriber_visionmsg.h"

int main(int argc, char *argv[])
{
    //need to perform all necessary inits
    //SUBSCRIBER FROM VISION
    ros::init(argc, argv, "visiondata_sub");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("vision_data", 1000, visionCallback);

    //PUBLISHER FOR MOTIONCONTROL
    ros::init(argc, argv, "mainhub");
    ros::Publisher chatter_pub = n.advertise<robot_soccer::controldata>("robot1Com", 1000);
    //ros::Rate loop_rate(TICKS_PER_SEC);
    int count = 0;
    sendCmd_Rob1 = true;
    while (ros::ok())
    {
        count++;
        if(visionUpdated && count%5==0){
            visionUpdated = false;
            std::cout << "dataRecieved: " << visionStatus_msg.ally1.location.x << " " <<
                         visionStatus_msg.ally1.location.y << " " << visionStatus_msg.ally1.theta << std::endl;
            field.updateStatus(visionStatus_msg);
        }

        if(sendCmd_Rob1){
            cmdRob1.cmdType = "mov";
            Point direction = calc::directionToPoint(field.currentStatus.ally1.location, center);
            cmdRob1.x = field.currentStatus.ally1.location.x;
            cmdRob1.y = field.currentStatus.ally1.location.y;
            cmdRob1.theta = field.currentStatus.ally1.theta;
            cmdRob1.x_cmd = direction.x;
            cmdRob1.y_cmd = direction.y;
            cmdRob1.theta_cmd = calc::angleDifference(cmdRob1.theta, 90);
            chatter_pub.publish(cmdRob1);
            std::cout << "Message sent" << std::endl;
        }
        //std::cout << "spinning" << std::endl;
        //ros::spinOnce();
        ros::getGlobalCallbackQueue()->callAvailable(ros::WallDuration(0.1));
    }
    return 0;
}







