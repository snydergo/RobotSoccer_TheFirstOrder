#include "skills.h"


enum class skill_state {idle_st, gotopoint_st, kick_st,
    fetchball_st, dribble_st, aim_st} skill_st;

void Skills::init(){
    skill_st = skill_state::idle_st;
    utils.init();
    dest = allyGoal;
}

//FUNCTIONS CALLED BY PLAYS OR OUTSIDE TO DO SKILLS
void Skills::idle(){
    printf("start circle ball\n");
    skill_st = skill_state::idle_st;
}

void Skills::goToPoint(Point point){
    dest = point;
    skill_st = skill_state::gotopoint_st;
}

void Skills::fetchBall(){
    printf("start fetch ball\n");
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

}

//FUNCTIONS THAT USE UTILITIES

void Skills::continueIdle(){
    utils.idle();
}

void Skills::continueGoToPoint(){
    utils.moveToPoint(field.currentStatus.ally1,dest,0);
}

void Skills::continueKick(){
    utils.kick(1,0);
}

void Skills::continueFetchBall(){
    utils.moveToPoint(field.currentStatus.ally1,field.currentStatus.ball.location,0);
}

void Skills::continueDribble(){
    utils.dribble();
}

void Skills::continueAim(){
    //need to calculate position
    double angleAtGoal = 0;
    double x_point = field.currentStatus.ball.location.x -1 ;
    double y_point = field.currentStatus.ball.location.y;
    Point aimSpot = Point(x_point,y_point);
    utils.moveToPoint(field.currentStatus.ally1, aimSpot ,0);
}

void Skills::stop(){

}

void Skills::tick(){
    switch(skill_st){
        case skill_state::idle_st:
            continueIdle();
            break;
        case skill_state::gotopoint_st:
            //continueGoToPoint();
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
