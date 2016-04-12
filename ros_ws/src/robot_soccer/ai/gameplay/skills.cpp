#include "skills.h"


inline moveSpeed Skills::getSpeed()
{
    Point robot = fieldget::getRobot(allyNum).location;
    double xValues = robot.x-dest.x;
    xValues *= xValues;
    double yValues = robot.y-dest.y;
    yValues *= yValues;
    double distance = std::sqrt(xValues+yValues);
    if (distance > MVSPD_FAST_THRESH) {
//        std::cout << "moveSpeed::fast" << std::endl;
        return moveSpeed::fast;
    } else {
//        std::cout << "moveSpeed::slow" << std::endl;
        return moveSpeed::slow;
    }
}

//FUNCTIONS CALLED BY PLAYS OR OUTSIDE TO DO SKILLS
void Skills::idle()
{
    skill_st = SkillState::idle;
}

void Skills::goToPoint(Point point, double dest_theta)
{
//    std::cout << "Skills::goToPoint theta == " + std::to_string(dest_theta) << std::endl;
    theta_cmd = dest_theta;
    dest = point;
    speed = getSpeed();
    skill_st = SkillState::gotoPoint;
}

void Skills::goToPoint(moveSpeed gvnspeed, Point point, double dest_theta)
{
//    std::cout << "Skills::goToPoint theta == " + std::to_string(dest_theta) << std::endl;
    theta_cmd = dest_theta;
    dest = point;
    speed = gvnspeed;
    skill_st = SkillState::gotoPoint;
}

void Skills::fetchBall()
{
//    printf("SKILLS:: start fetch ball\n");
    skill_st = SkillState::fetchBall;
}

void Skills::kick()
{
    skill_st = SkillState::kick;
}

void Skills::init_kick()
{
    skill_st = SkillState::initKick;
    initKicker();
}

void Skills::uninit_kick()
{
    skill_st = SkillState::uninitKick;
    uninitKicker();
}

void Skills::dribble()
{
//    printf("start dribble ball\n");
    skill_st = SkillState::dribble;
}

void Skills::aim()
{
//    printf("start aim ball\n");
    skill_st = SkillState::aim;
}

//### FUNCTIONS THAT USE UTILITIES ###//
void Skills::continueIdle()
{
    utils.idle(fieldget::getRobot(allyNum));
}

void Skills::continueGoToPoint()
{
    utils.moveToPoint(speed, fieldget::getRobot(allyNum),dest,theta_cmd);
}

void Skills::initKicker()
{
    utils.initKick(fieldget::getRobot(allyNum));
}

void Skills::uninitKicker()
{
    utils.uninitKick(fieldget::getRobot(allyNum));
}

void Skills::continueKick()
{
    utils.kick(fieldget::getRobot(allyNum));
}

//needs to be changed so it goes around ball to get to desired position
//function that is used to go to a point that is behind the ball
void Skills::continueFetchBall()
{
    Point ballP = fieldget::getBallLoc();
    Point fetchballpoint(ballP.x-FETCHBALL_OFFSET,ballP.y);
    double theta = bkcalc::getAngleTo(allyNum, ballP);
    utils.moveToPoint(getSpeed(), fieldget::getRobot(allyNum),fetchballpoint,theta);
}

void Skills::continueDribble()
{
    utils.dribble(fieldget::getRobot(allyNum));
}

//function used for aiming. makes sure that the angle between the robot and the goal and
//the angle between the robot and the ball are approximately the same.
void Skills::continueAim()
{
    //need to calculate position
    Point ballLoc = fieldget::getBallLoc();
    Robot robot = fieldget::getRobot(allyNum);
//    Point robGoalDir = calc::directionToPoint(fieldget::getRobotLoc(allyNum), enemyGoal);
//    Point robBallDir = calc::directionToPoint(fieldget::getRobotLoc(allyNum), ballLoc);
    Point ballGoalDir = calc::directionToPoint(fieldget::getBall().location, enemyGoal);
//    double robGoalAngle = calc::getVectorAngle(robGoalDir);
//    double robBallAngle = calc::getVectorAngle(robBallDir);
    double ballGoalAngle = calc::getVectorAngle(ballGoalDir);
    double ballGoalRadians = calc::degToRad(ballGoalAngle);

    double x_point = ballLoc.x - AIM_BALL_DIST*std::cos(ballGoalRadians);
    double y_point = ballLoc.y - AIM_BALL_DIST*std::sin(ballGoalRadians);
//    if (robBallAngle > robGoalAngle ) {
//        //rotate CCW or move y+ and a little x+
//        y_point += 2;
//        std::cout << "ROTATE CCW" << std::endl;
//    } else {
//        //rotate CW or move y- and a little x+
//        y_point -= 2;
//        std::cout << "ROTATE CW" << std::endl;
//    }

    Point aimSpot(x_point, y_point);
    //Point aimBallDir = calc::directionToPoint(aimSpot, ballLoc);
    double newTheta = ballGoalAngle; //calc::getVectorAngle(aimBallDir);
    if(ballLoc.x < robot.location.x){
        aimSpot.y += (ballLoc.y < 0) ? 30 : -30;
        aimSpot.x -= 30;
        aimSpot = calc::getNewPoint(aimSpot);
        utils.moveToPoint(moveSpeed::fast, robot, aimSpot ,newTheta);
        return;
    }
    utils.moveToPoint(moveSpeed::slow, robot, aimSpot ,newTheta);
}

void Skills::stop()
{

}

void Skills::tick()
{
    switch(skill_st) {
    case SkillState::idle:
        continueIdle();
        break;
    case SkillState::gotoPoint:
        continueGoToPoint();
        break;
    case SkillState::kick:
        continueKick();
        break;
    case SkillState::fetchBall:
        continueFetchBall();
        break;
    case SkillState::dribble:
        continueDribble();
        break;
    case SkillState::aim:
        continueAim();
        break;
    case SkillState::initKick:
        initKicker();
        break;
    case SkillState::uninitKick:
        uninitKicker();
        break;
    default:
        //throw exception
        break;
    }
}
