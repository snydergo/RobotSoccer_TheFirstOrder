#include <cstdio>
#include <ros/callback_queue.h>
#include "bookkeeping.h"
#include "visiondata/subscriber_visionmsg.h"
#include "gameplay/strategy.h"
#include "types.h"

void checkCmd(robot_soccer::controldata &cmdRob1);

int main(int argc, char *argv[])
{
    //need to perform all necessary inits
    //SUBSCRIBER FROM VISION
    ros::init(argc, argv, "visiondata_sub");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("vision_data", 1000, visionCallback);

    //PUBLISHER FOR MOTIONCONTROL
    ros::init(argc, argv, "mainhub");
    ros::Publisher robo1Com = n.advertise<robot_soccer::controldata>("robot1Com", 1000);
    //ros::Publisher robo2Com = n.advertise<robot_soccer::controldata>("robot2Com", 1000);
    ros::Rate loop_rate(10);
    int count = 0;
    //sendCmd_Rob1 = true;

//    //strategies STATEMACHINE
//    Strategies strategies;
//    strategies.init();
//    Plays play;
//    play.init();
//    play.start();
    Skills skill;
    skill.init();
    //std::cout << "Main Control Started Successfully" << std::endl;
    bool dataInitialized = false;
    while (ros::ok())
    {
        count++;



        if(visionUpdated && count%5==0){
            visionUpdated = false;
            dataInitialized = true;

            /*//std::cout << "dataRecieved: " << visionStatus_msg.ally1.location.x << " " <<
                        visionStatus_msg.ally1.location.y << " " << visionStatus_msg.ally1.theta << std::endl;*/
            field.updateStatus(visionStatus_msg);
        }

        //        strategies.tick();
                // play.tick();

        skill.goToPoint(Point(0,0), -90.0);
        skill.tick();
        std::cout << "mark" << std::endl;
        if(sendCmd_Rob1){

            sendCmd_Rob1 = false;
            checkCmd(cmdRob1);
            cmdRob1.x_cmd = cmdRob1.x;
            cmdRob1.y_cmd = cmdRob1.y;
            cmdRob1.theta_cmd = -90;
            robo1Com.publish(cmdRob1);
        }
        std::cout << "spin once" << std::endl;
        ros::spinOnce();
        std::cout << "sleep" << std::endl;
        loop_rate.sleep();
        std::cout << "end of loop" << std::endl;
        //ros::getGlobalCallbackQueue()->callAvailable(ros::WallDuration(0.1));
    }
    std::cout << "returning" << std::endl;
    return 0;
}

void checkCmd(robot_soccer::controldata &cmdRob){
    if(cmdRob.x != cmdRob.x || cmdRob.x_cmd != cmdRob.x_cmd){
        std::cout << "maincontrol::checkCmd() ##nan's detected## reverting to IDLE" << std::endl;
        cmdRob.cmdType = "idle";

    }
}






