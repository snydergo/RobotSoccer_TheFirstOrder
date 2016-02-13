#include <cstdio>

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
    ros::Rate loop_rate(TICKS_PER_SEC);

    int count = 0;
    while (ros::ok())
    {
        count++;
        if(visionUpdated){
            visionUpdated = false;
            std::cout << "dataRecieved: " << visionStatus_msg.ally1.location.x << " " <<
                         visionStatus_msg.ally1.location.y << " " << visionStatus_msg.ally1.theta << std::endl;
            //field.updateStatus(visionStatus_msg);
        }

        if(count){
            robot_soccer::controldata msg;
            msg.cmdType = "mov";
            msg.x_dir = count;
            msg.y_dir = count;
            msg.cur_theta = count;
            msg.des_theta = count;
            chatter_pub.publish(msg);
            std::cout << "Message sent" << std::endl;
        }
        //std::cout << "spinning" << std::endl;
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}







