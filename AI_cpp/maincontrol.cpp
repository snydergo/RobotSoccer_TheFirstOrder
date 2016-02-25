#include <cstdio>
#include <ros/callback_queue.h>
#include "bookkeeping.h"
#include "visiondata/subscriber_visionmsg.h"
#include "gameplay/strategy.h"
#include "types.h"

void checkCmd(robot_soccer::controldata *cmdRob1);

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
    //sendCmd_Rob1 = true;

//    //STRATEGY STATEMACHINE
    Strategies strategy;
    strategy.init();
    Plays play;
    play.init();
    play.start();
    //std::cout << "Main Control Started Successfully" << std::endl;
    bool dataInitialized = false;
    while (ros::ok())
    {
        count++;
        //if(dataInitialized){}

        strategy.tick();
        //play.tick();
        if(visionUpdated && count%5==0){
            visionUpdated = false;
            dataInitialized = true;

            /*//std::cout << "dataRecieved: " << visionStatus_msg.ally1.location.x << " " <<
                        visionStatus_msg.ally1.location.y << " " << visionStatus_msg.ally1.theta << std::endl;*/
            field.updateStatus(visionStatus_msg);
        }

        if(sendCmd_Rob1){
            sendCmd_Rob1 = false;
            checkCmd(&cmdRob1);
            chatter_pub.publish(cmdRob1);
            ////std::cout<<"sending data:\n"<<std::endl; destobj.x - startobj.x
            std::cout << "x_dir = " << cmdRob1.x_cmd-cmdRob1.x << std::endl <<
                         "y_dir = " << cmdRob1.y_cmd-cmdRob1.y << std::endl <<
                         "theta = " << cmdRob1.theta_cmd-cmdRob1.theta << std::endl;
            std::cout << "desired_x = " << cmdRob1.x_cmd << std::endl <<
                         "desired_y = " << cmdRob1.y_cmd << std::endl <<
                         "desired_theta = " << cmdRob1.theta_cmd << "\n\n";
        }
        ////std::cout << "spinning" << std::endl;

        ros::spinOnce();
        loop_rate.sleep();
        //ros::getGlobalCallbackQueue()->callAvailable(ros::WallDuration(0.1));
    }
    return 0;
}

void checkCmd(robot_soccer::controldata *cmdRob1){
    if(cmdRob1->x != cmdRob1->x || cmdRob1->x_cmd != cmdRob1->x_cmd){
        std::cout << "maincontrol::checkCmd() ##nan's detected## reverting to IDLE" << std::endl;
        cmdRob1->cmdType = "idle";

    }
}






