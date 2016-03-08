#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Vector3.h"
#include "std_msgs/Float64.h"
#include "walle/SoccerPoses.h"
#include "stdio.h"
#include <eigen3/Eigen/Eigen>

using namespace std;
using namespace geometry_msgs;
using namespace Eigen;

#define ROBOT_MAX_VXY .5
#define ROBOT_MAX_OMEGA 2*M_PI
#define CONTROL_K_XY 5
#define CONTROL_K_OMEGA 2
#define FIELD_WIDTH 3.048  // in meters
#define FIELD_HEIGHT 1.524 
#define ROBOT_RADIUS 0.10

string team;
Vector2d goal;
ros::Publisher motor_pub1;
ros::Publisher motor_pub2;
ros::Subscriber vision_sub;

struct RobotPose
{
	Vector2d pos;
	double theta;
};

void moveRobot(Vector3d v_world, int robotId)
{
	geometry_msgs::Vector3 v;
	if(team == "away") // Flip coordinates if team is away
	{
		v_world(0) = -v_world(0);
		v_world(1) = -v_world(1);
	}
	v.x = v_world(0);
	v.y = v_world(1);
	v.z = v_world(2);
	if(robotId == 1)
		motor_pub1.publish(v);
	else if(robotId == 2)
		motor_pub2.publish(v);
}

void param_init()
{
	goal << FIELD_WIDTH/2, 0;
}

int utility_sgn(double val)
{
	return (0 < val) - (val < 0);
}

// utility - saturate velocity
// 	Saturate the commanded velocity .
Vector3d utility_saturateVelocity(Vector3d v)
{
	if(fabs(v(0)) > ROBOT_MAX_VXY)
		v(0) = utility_sgn(v(0)) * ROBOT_MAX_VXY;
	if(fabs(v(1)) > ROBOT_MAX_VXY)
		v(1) = utility_sgn(v(1)) * ROBOT_MAX_VXY;
	if(fabs(v(2)) > ROBOT_MAX_OMEGA)
		v(2) = utility_sgn(v(2)) * ROBOT_MAX_OMEGA;
	return v;
}

double utility_vecLength(Vector2d v)
{
	return sqrt(v(0)*v(0) + v(1)*v(1));
}

Vector2d utility_unitVector(Vector2d v)
{
	return v / utility_vecLength(v);
}

double utility_angleMod(double angle)
{
	while(angle < 0)
		angle += 2*M_PI;
	return fmod(angle + M_PI, (2*M_PI)) - M_PI;
}

RobotPose utility_toRobotPose(Pose2D robot)
{
	if(team == "away") // Flip coordinates if team is away
	{
		robot.x = -robot.x;
		robot.y = -robot.y;
		robot.theta = utility_angleMod(robot.theta + M_PI);
	}
	Vector2d pos;
	pos << robot.x, robot.y;
	return (RobotPose){pos, robot.theta};
}

Vector2d utility_toBallPose(Pose2D ball)
{
	if(team == "away") // Flip coordinates if team is away
	{
		ball.x = -ball.x;
		ball.y = -ball.y;
	}
	Vector2d pos;
	pos << ball.x, ball.y;
	return pos;
}

// skill - follow ball on line
//   Follows the y-position of the ball, while maintaining x-position at x_pos. 
//   Angle always faces the goal.
void skill_followBallOnLine(RobotPose robot, Vector2d ball, double x_pos, int robotId)
{
	// control x position to stay on current line
	double vx = CONTROL_K_XY * (x_pos - robot.pos(0));

	// control y position to match the ball's y-position
	double vy = CONTROL_K_XY * (ball(1) - robot.pos(1));

	// control angle to face the goal
	Vector2d dirGoal = goal - robot.pos;
	double theta_d = atan2(dirGoal(1), dirGoal(0));
	double omega = -CONTROL_K_OMEGA * (robot.theta - theta_d); 
	
	// Output velocities to motors	
	Vector3d v;
	v << vx, vy, omega;
	v = utility_saturateVelocity(v);
	moveRobot(v, robotId);
}

// skill - go to point
//   Travels towards a point. Angle always faces the goal.
void skill_goToPoint(RobotPose robot, Vector2d point, int robotId)
{
	Vector2d dirPoint = point - robot.pos;
	Vector2d vxy = dirPoint * CONTROL_K_XY;

	// control angle to face the goal
	Vector2d dirGoal = goal - robot.pos;
	double theta_d = atan2(dirGoal(1), dirGoal(0));
	double omega = -CONTROL_K_OMEGA * (robot.theta - theta_d); 

	// Output velocities to motors
	Vector3d v;
	v << vxy, omega;
	v = utility_saturateVelocity(v);
	moveRobot(v, robotId);
}

// play - rush goal
//   - go to position behind ball
//   - if ball is between robot and goal, go to goal
// NOTE:  This is a play because it is built on skills, and not control
// commands.  Skills are built on control commands.  A strategy would employ
// plays at a lower level.  For example, switching between offense and
// defense would be a strategy.
void play_rushGoal(RobotPose robot, Vector2d ball, int robotId)
{
	// normal vector from ball to goal
	Vector2d n = utility_unitVector(goal - ball);

	// compute position 10cm behind ball, but aligned with goal.
	Vector2d position = ball - 0.2*n;

	if(utility_vecLength(position - robot.pos) < .21)
		skill_goToPoint(robot, goal, robotId);
	else
		skill_goToPoint(robot, position, robotId);
}

void visionCallback(const walle::SoccerPoses& msg)
{
	RobotPose player1;
	RobotPose player2;
	RobotPose opponent1;
	RobotPose opponent2;
	Vector2d ball;
	player1   = utility_toRobotPose((team == "home") ? msg.home1 : msg.away1);
	player2   = utility_toRobotPose((team == "home") ? msg.home2 : msg.away2);
	opponent1 = utility_toRobotPose((team == "home") ? msg.away1 : msg.home1);
	opponent2 = utility_toRobotPose((team == "home") ? msg.away2 : msg.home2);
	ball      = utility_toBallPose(msg.ball);

	// robot #1 positions itself behind ball and rushes the goal.
	play_rushGoal(player1, ball, 1);

	// robot #2 stays on line, following the ball, facing the goal
	skill_followBallOnLine(player2, ball, -2 * FIELD_WIDTH / 6, 2);
}

int main(int argc, char **argv)
{
	param_init();
	ros::init(argc, argv, "home");
	ros::NodeHandle nh("~");
	nh.param<string>("team", team, "home");

	vision_sub = nh.subscribe("/vision", 1, visionCallback);
	motor_pub1 = nh.advertise<geometry_msgs::Vector3>("/" + team + "1/command", 5);
	motor_pub2 = nh.advertise<geometry_msgs::Vector3>("/" + team + "2/command", 5);

	ros::spin();
	Vector3d zeroVel;
	zeroVel << 0, 0, 0;
	moveRobot(zeroVel, 1);
	moveRobot(zeroVel, 2);
	return 0;
}


