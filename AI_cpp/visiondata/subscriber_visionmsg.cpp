#include "ros/ros.h"
#include "robot_soccer/visiondata.h"
#include "bookkeeping.h"
/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */

extern bool visionUpdated;
extern Gamestatus visionStatus_msg;
extern sem visionMsg_sem;
void visionCallback(const robot_soccer::visiondata* msg)
{
    visionUpdated = true;
    //pend on semaphore
    if(pend(visionMsg_sem)){
        visionStatus_msg.ally1->location->x = msg->tm0_x;
        visionStatus_msg.ally1->location->y = msg->tm0_y;
        visionStatus_msg.ally1->theta = msg->tm0_w;
        visionStatus_msg.ally2->location->x = msg->tm1_x;
        visionStatus_msg.ally2->location->y = msg->tm1_y;
        visionStatus_msg.ally2->theta = msg->tm1_w;
        visionStatus_msg.enemy1->location->x = msg->op0_x;
        visionStatus_msg.enemy1->location->y = msg->op0_y;
        visionStatus_msg.enemy1->theta = msg->op0_w;
        visionStatus_msg.enemy2->location->x = msg->op1_x;
        visionStatus_msg.enemy2->location->y = msg->op1_y;
        visionStatus_msg.enemy2->theta = msg->op1_w;
        visionStatus_msg.ball->location->x = msg->ball_x;
        visionStatus_msg.ball->location->y = msg->ball_y;
        post(visionMsg_sem);
    }
}

int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "visiondata_sub");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber sub = n.subscribe("visiondata", 1000, visionCallback);

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();

  return 0;
}
