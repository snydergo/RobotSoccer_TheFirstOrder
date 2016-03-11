#include "skills.h"


enum class skill_state {idle_st, gotopoint_st, kick_st,
    fetchball_st, dribble_st, aim_st} skill_st;

void Skills::init(){
    skill_st = skill_state::idle_st;
    utils.init(*fieldget::getRobot(allyNum));
    dest = allyGoal;
}

//FUNCTIONS CALLED BY PLAYS OR OUTSIDE TO DO SKILLS
void Skills::idle(){
    skill_st = skill_state::idle_st;
}

void Skills::goToPoint(Point point, double dest_theta){
    std::cout << "Skills::goToPoint theta == " + std::to_string(dest_theta) << std::endl;
    theta_cmd = dest_theta;
    dest = point;
    skill_st = skill_state::gotopoint_st;
}

void Skills::fetchBall(){
    printf("SKILLS:: start fetch ball\n");
    skill_st = skill_state::fetchball_st;
}

void Skills::kick(){
    skill_st = skill_state::kick_st;
}

void Skills::dribble(){
    printf("start dribble ball\n");
    skill_st = skill_state::dribble_st;
}

//function will get the robot to go to the ball
//and aim position to aim at the goal
void Skills::aim(){
    printf("start aim ball\n");
    skill_st = skill_state::aim_st;
}

//### FUNCTIONS THAT USE UTILITIES ###//

void Skills::continueIdle(){
    utils.idle(*fieldget::getRobot(allyNum));
}

void Skills::continueGoToPoint(){
    utils.moveToPoint(*fieldget::getRobot(allyNum),dest,theta_cmd);
}

void Skills::continueKick(){
    utils.kick(*fieldget::getRobot(allyNum));
}

//needs to be changed so it goes around ball to get to desired position
//function that is used to go to a point that is behind the ball
void Skills::continueFetchBall(){
    Point ballP = fieldget::getBallLoc();
    Point fetchballpoint(ballP.x-FETCHBALL_OFFSET,ballP.y);
    double theta = bkcalc::getAngleTo(allyNum, ballP);
    utils.moveToPoint(*fieldget::getRobot(allyNum),fetchballpoint,theta);
}

void Skills::continueDribble(){
    utils.dribble(*fieldget::getRobot(allyNum));
}

//function used for aiming. makes sure that the angle between the robot and the goal and
//the angle between the robot and the ball are approximately the same.
void Skills::continueAim(){
    //need to calculate position
    Point ballLoc = fieldget::getBallLoc();
    Point robGoalDir = calc::directionToPoint(fieldget::getRobotLoc(allyNum), enemyGoal);
    Point robBallDir = calc::directionToPoint(fieldget::getRobotLoc(allyNum), ballLoc);
    double robGoalAngle = calc::getVectorAngle(robGoalDir);
    double robBallAngle = calc::getVectorAngle(robBallDir);

    double x_point = fieldget::getRobotLoc(allyNum).x + 2;
    double y_point = fieldget::getRobotLoc(allyNum).y;
    if(robBallAngle > robGoalAngle ){
        //rotate CCW or move y+ and a little x+
        y_point += 2;
        std::cout << "ROTATE CCW" << std::endl;
    }else{
        //rotate CW or move y- and a little x+
        y_point -= 2;
        std::cout << "ROTATE CW" << std::endl;
    }
    Point aimSpot(x_point, y_point);
    Point aimBallDir = calc::directionToPoint(aimSpot, ballLoc);
    double newTheta = calc::getVectorAngle(aimBallDir);
    std::cout << "SKILL::continueAim theta = " << std::to_string(newTheta)<< std::endl;
    utils.moveToPoint(*fieldget::getRobot(allyNum), aimSpot ,newTheta);
}

void Skills::stop(){

}

void Skills::tick(){
    switch(skill_st){
        case skill_state::idle_st:
            continueIdle();
            break;
        case skill_state::gotopoint_st:
            std::cout << "SKILLS::tick() gottopoint_st"<< std::endl;
            continueGoToPoint();
            break;
        case skill_state::kick_st:
            continueKick();
            break;
        case skill_state::fetchball_st:
            continueFetchBall();
            break;
        case skill_state::dribble_st:
            continueDribble();
            break;
        case skill_state::aim_st:
            continueAim();
            break;
        default:
            //throw exception
            break;
       }
}
