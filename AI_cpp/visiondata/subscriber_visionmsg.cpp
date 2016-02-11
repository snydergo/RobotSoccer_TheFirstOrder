#include "subscriber_visionmsg.h"
/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */


void visionCallback(const robot_soccer::visiondata& msg)
{
    visionUpdated = true;
    //pend on semaphore
    if(pend(visionMsg_sem)){
        visionStatus_msg.ally1.location.x = msg.tm0_x;
        visionStatus_msg.ally1.location.y = msg.tm0_y;
        visionStatus_msg.ally1.theta = msg.tm0_w;
        visionStatus_msg.ally2.location.x = msg.tm1_x;
        visionStatus_msg.ally2.location.y = msg.tm1_y;
        visionStatus_msg.ally2.theta = msg.tm1_w;
        visionStatus_msg.enemy1.location.x = msg.op0_x;
        visionStatus_msg.enemy1.location.y = msg.op0_y;
        visionStatus_msg.enemy1.theta = msg.op0_w;
        visionStatus_msg.enemy2.location.x = msg.op1_x;
        visionStatus_msg.enemy2.location.y = msg.op1_y;
        visionStatus_msg.enemy2.theta = msg.op1_w;
        visionStatus_msg.ball.location.x = msg.ball_x;
        visionStatus_msg.ball.location.y = msg.ball_y;
        post(visionMsg_sem);
    }
}

//void listen_vision(int argc, char **argv)
//{

//    std::cout << "before init" << std::endl;
//    ros::init(argc, argv, "visiondata_sub");
//    std::cout << "before new node" << std::endl;
//    ros::NodeHandle n;
//    std::cout << "before subscription" << std::endl;
//    ros::Subscriber sub = n.subscribe("vision_data", 1000, visionCallback);
//}
