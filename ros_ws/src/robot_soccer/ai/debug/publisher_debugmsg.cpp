#include "publisher_debugmsg.h"
/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */

bool validmsg(robot_soccer::controldata msg){

}
bool isPointCmd(std::string cmd, robot_soccer::controldata* msg){
    bool pointCmd = true;
    if(cmd == "center"){
        msg->x_cmd = center.x;
        msg->y_cmd = center.y;
        msg->theta_cmd = 90;
    }else if(cmd == "enemygoal"){
        msg->x_cmd = enemyGoal.x;
        msg->y_cmd = enemyGoal.y;
        msg->theta_cmd = -180;
    }else if(cmd == "allygoal"){
        msg->x_cmd = allyGoal.x;
        msg->y_cmd = allyGoal.y;
        msg->theta_cmd = -180;
    }else
        pointCmd = false;

    return pointCmd;
}

int main(int argc, char **argv)
{
  std::cout << "RUNNING DEBUG PUBLISHER" << std::endl;
  ros::init(argc, argv, "debug_input");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<robot_soccer::controldata>("debug", 1000);

  ros::Rate loop_rate(TICKS_PER_SEC);
  bool cmdTypeValid = false;
  std::string cmdtype;
  std::string gvn_xcmd;
  std::string gvn_ycmd;
  std::string gvn_thetacmd;


  int count = 0;
  while (ros::ok())
  {
    cmdTypeValid = false;
    robot_soccer::controldata msg;
    while(!cmdTypeValid){
        std::cout << "### NEW CMD ###\nINPUT CMDTYPE" << std::endl;
        std::cin >> cmdtype;
        if(cmdtype == "movefast" || "moveslow"){
            msg.cmdType = cmdtype;
            std::cout << "Enter xcmd\n";
            std::cin >> gvn_xcmd;
            if(!isPointCmd(gvn_xcmd,&msg)){
                std::cout << "Enter ycmd\n";
                std::cin >> gvn_ycmd;
                std::cout << "Enter thetacmd\n";
                std::cin >> gvn_thetacmd;
                try{
                    msg.x_cmd = stold(gvn_xcmd);
                    msg.y_cmd = stold(gvn_ycmd);
                    msg.theta_cmd = stold(gvn_thetacmd);
                    cmdTypeValid = true;
                }catch(...){
                    std::cout << "INVALID PARAMETERS\n";
                }
            }else {cmdTypeValid = true;}

        }else if(cmdtype == "kick" || cmdtype == "idle"){
            cmdTypeValid = true;
            msg.cmdType = cmdtype;
            std::cout << "cmdType == " + msg.cmdType << std::endl;
        }else if (cmdtype == "quit" ||
                  cmdtype == "q"){
            exit(0);
        }else
            std::cout << "Not a valid CMD TYPE\n";
        std::cout << "\n";
    }
    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */

    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }
}
