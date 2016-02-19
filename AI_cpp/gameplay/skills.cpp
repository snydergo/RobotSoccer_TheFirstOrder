#include "skills.h"


enum class skill_state {idle_st, gotopoint_st, kick_st,
    fetchball_st, dribble_st, aim_st} skill_st;


//FUNCTIONS CALLED BY PLAYS OR OUTSIDE TO DO SKILLS
void Skills::skill_idle(){
    printf("start circle ball\n");
    skill_st = skill_state::idle_st;
}

void Skills::skill_goToPoint(Point* point){
    dest = *point;
    skill_st = skill_state::gotopoint_st;
}

void Skills::skill_fetchBall(){
    printf("start fetch ball\n");
    skill_st = skill_state::fetchball_st;
}

void Skills::skill_kick(){
    skill_st = skill_state::kick_st;
}

void Skills::skill_dribble(){
    printf("start dribble ball\n");
    skill_st = skill_state::dribble_st;
}

//function will get the robot to go to the ball
//and aim position to aim at the goal
void Skills::skill_aim(){

}

//FUNCTIONS THAT USE UTILITIES

void Skills::skill_continueIdle(){
    utils.idle();
}

void Skills::skill_continueGoToPoint(){
    utils.moveToPoint(field.currentStatus.ally1,dest,0);
}

void Skills::skill_continueKick(){
    utils.kick(1,0);
}

void Skills::skill_continueFetchBall(){
    utils.moveToPoint(field.currentStatus.ally1,field.currentStatus.ball.location,0);
}

void Skills::skill_continueDribble(){
    utils.dribble();
}

void Skills::skill_continueAim(){
    //need to calculate position
    double angleAtGoal = 0;
    double x_point = field.currentStatus.ball.location.x -1 ;
    double y_point = field.currentStatus.ball.location.y;
    Point aimSpot = Point(x_point,y_point);
    utils.moveToPoint(field.currentStatus.ally1, aimSpot ,0);
}

void Skills::skill_stop(){

}

void Skills::skill_tick(){
    switch(skill_st){
        case skill_state::idle_st:
            skill_continueIdle();
            break;
        case skill_state::gotopoint_st:
            //skill_continueGoToPoint();
            break;
        case skill_state::kick_st:
            skill_continueKick();
            break;
        case skill_state::fetchball_st:
            skill_continueFetchBall();
            break;
        case skill_state::dribble_st:
            skill_continueDribble();
            break;
        case skill_state::aim_st:
            skill_continueAim();
            break;
        default:
            //throw exception
            break;
       }
}
