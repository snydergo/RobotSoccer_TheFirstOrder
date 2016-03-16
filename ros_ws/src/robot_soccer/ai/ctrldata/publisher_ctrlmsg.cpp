#include "publisher_ctrlmsg.h"
/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
void chat_ctrl(int argc, char **argv)
{

  ros::init(argc, argv, "mainhub");

  ros::NodeHandle n;

  ros::Publisher chatter_pub = n.advertise<robot_soccer::controldata>("chatter", 1000);

  ros::Rate loop_rate(50);

  int count = 0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    robot_soccer::controldata msg;

    //std::stringstream ss;
    //ss << "hello world " << count;
    //msg.data = ss.str();

    //ROS_INFO("%s", msg.);

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
