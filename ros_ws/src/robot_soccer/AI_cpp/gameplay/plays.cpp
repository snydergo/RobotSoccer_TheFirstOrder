/*Contains all of the plays or roles like goalie, rushgoal, split rush, split goal and etc*/
#include "plays.h"


enum class play_state {idle_st, start_st, rushgoal_st, playgoalie_st } play_st;

enum class coordSkills_st {coordIdle_st, coordGotogoal_st, coordKick_st,
    coordFollowball_st, coordFetchball_st, coordDribble_st, coordAim_st} coord_st;

//variable that is used for Kicking
//Kick Point
Point kp = center;

//performs all of the necessary inits and calls skills init
void Plays::init(){
    play_st = play_state::idle_st;
    skill.init();
    skill.idle();
}

//###FUNCTIONS THAT ARE USED OUTSIDE TO SET WHICH PLAY TO PERFORM.###//
void Plays::start(){
    play_st = play_state::start_st;
}

void Plays::rushGoal(){
    coord_st = coordSkills_st::coordFetchball_st; //needs to be set when role switches
    play_st = play_state::rushgoal_st;
}

void Plays::playGoalie(){
    coord_st = coordSkills_st::coordGotogoal_st; //needs to be set when role switches firstime
    play_st = play_state::playgoalie_st;
}

void Plays::idle(){
    play_st = play_state::idle_st;
}

//### END OF FUNCTIONS CALLED OUTSIDE ###//
void Plays::tick(){
    //needed changes
    /*
        Needed Changes
            1. when roles/plays are assigned each state needs to be initialized appropriately
            2. Need each time coord_st switch statement occurs that a function from skills is called
            3. Need to add an exception code for when default case is reached in statemachines.
    */
    switch(play_st){
            case play_state::idle_st:
                std::cout << "Plays::tick() idle_st"<< std::endl;
                skill.idle();
                break;
            case play_state::start_st:
                std::cout << "Plays::tick() start_st"<< std::endl;
                skill.goToPoint(start1Location, 0);
                break;
            //## RUSH GOAL PLAY ##//
            case play_state::rushgoal_st:
                std::cout << "Plays::tick() rushgoal_st ";
                switch(coord_st){
                    case coordSkills_st::coordFetchball_st:
                       std::cout << " fetching ball" << std::endl;
                       skill.fetchBall();
                       if(bkcalc::ballFetched(allyNum)){
                            std::cout << "Plays::tick() BALL FETCHED" << std::endl;
                            coord_st = coordSkills_st::coordAim_st;
                        }
                        break;
                    case coordSkills_st::coordAim_st:
                        static uint8_t aim_cnt = 1;
                        std::cout << " aiming ball" << std::endl;
                        skill.aim();
                        /*if(!bkcalc::ballFetched(allyNum)){
                             std::cout << "Plays::tick() BALL FETCHED" << std::endl;
                             coord_st = coordSkills_st::coordFetchball_st;
                        }else*/ if(bkcalc::ballAimed(allyNum) || !aim_cnt++){
                            std::cout << "Plays::tick() BALL AIMED" << std::endl;
                            kp = bkcalc::kickPoint(allyNum);
                            coord_st = coordSkills_st::coordKick_st;
                        }
                        break;
                    case coordSkills_st::coordKick_st:
                        std::cout << " kicking ball" << std::endl;
                        //skill.kick();
                        skill.goToPoint(kp,bkcalc::getAngleTo(allyNum,fieldget::getBallLoc()));
                        if(bkcalc::ballKicked(allyNum,kp)){
                            skill.kick();
                            std::cout << "Plays::tick() BALL KICKED" << std::endl;
                            coord_st = coordSkills_st::coordFetchball_st;
                        }
                        break;
                    default:
                        //Throw Exception
                        break;
                }
                break;
            //## GOALIE PLAY ##//
            case play_state::playgoalie_st:
                //always should first go to goal
                std::cout << "Plays::tick() playgoalie_st"<< std::endl;
                switch(coord_st){
                    case coordSkills_st::coordGotogoal_st:
                        std::cout << "Skills::coordSkills_st == gotogoal"<<std::endl;
                        skill.goToPoint(allyGoal, 0);
                        if(bkcalc::atLocation(allyNum, allyGoal)){
                            std::cout << "AT GOAL" << std::endl;
                            coord_st = coordSkills_st::coordFollowball_st;
                        }
                        break;
                    case coordSkills_st::coordFollowball_st:
                    {
                        std::cout << "Skills::coordSkills_st == Followball_st"<<std::endl;
                        //if ball is close to robot
                        if(bkcalc::ballKickZone(allyNum)){
                            std::cout << "Plays::tick() BALL FOLLOWED" << std::endl;
                            kp = bkcalc::kickPoint(allyNum);
                            coord_st = coordSkills_st::coordKick_st;
                        }else{ //if ball isn't close to robot
                            Point point;
                            //if ball is within goalie box
                            if(abs(fieldget::getBallLoc().y) < GOAL_RADIUS){
                                std::cout << "ball is within Goal width" << std::endl;
                                point = Point(allyGoal.x,fieldget::getBallLoc().y);
                            }else{ //ball is outside of goalie box
                                std::cout << "ball is outside Goal width" << std::endl;
                                double y_coord = allyGoal.y;
                                if(fieldget::getBallLoc().y > 0)
                                    y_coord += GOAL_RADIUS;
                                else
                                    y_coord -= GOAL_RADIUS;

                                point = Point(allyGoal.x,y_coord);
                            }
                            double cmdTheta = bkcalc::getAngleTo(allyNum,fieldget::getBallLoc());
                            skill.goToPoint(point,cmdTheta);
                        }
                    }
                        break;
                    case coordSkills_st::coordKick_st:
                        std::cout << "Skills::coordSkills_st == kick_st"<<std::endl;
                        //skill.kick();
                        skill.goToPoint(kp,bkcalc::getAngleTo(allyNum,fieldget::getBallLoc()));
                        if(bkcalc::ballKicked(allyNum, kp)){
                            std::cout << "Plays::tick() BALL KICKED" << std::endl;
                            coord_st = coordSkills_st::coordGotogoal_st;
                        }
                    break;
                    default:
                        //Throw Exception
                        break;
                };
                break;
            default:
                //Throw Exception
                break;


    };
    //need skill to perform action depending on change
    skill.tick();
}
