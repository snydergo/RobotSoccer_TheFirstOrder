#include "plays.h"


enum class play_state {idle_st, rushgoal_st, playgoalie_st } play_st;

enum class coordSkills_st {coordIdle_st, coordGotogoal_st, coordKick_st,
    coordFollowball_st, coordFetchball_st, coordDribble_st, coordAim_st} coord_st;

Plays::Plays()
{

}


void Plays::play_tick(){
    //needed changes
    /*
        Needed Changes
            1. when roles/plays are assigned each state needs to be initialized appropriately
            2. need to provide ways for skills to be able to access the GameStatus without difficulty
                a. inheritance, globals or passed in as parameters
            3. Need each time coord_st switch statement occurs that a function from skills is called
            4. Need to add an exception code for when default case is reached in statemachines.
    */
    switch(play_st){
            case play_state::idle_st:
                break;
            case play_state::rushgoal_st:
                coord_st = coordSkills_st::coordFetchball_st; //needs to be set when role switches
                switch(coord_st){
                    case coordSkills_st::coordFetchball_st:
                       skill.skill_fetchBall();
                       if(true/*ballFetched()*/){
                            coord_st = coordSkills_st::coordAim_st;
                        }
                        break;
                    case coordSkills_st::coordAim_st:
                        skill.skill_aim();
                        if(true /*ballAimed()*/){
                            coord_st = coordSkills_st::coordKick_st;
                        }
                        break;
                    case coordSkills_st::coordKick_st:
                        skill.skill_kick();
                        if(true /*ballKicked()*/){
                            coord_st = coordSkills_st::coordFetchball_st;
                        }
                        break;
                    default:
                        //Throw Exception
                        break;
                }
                break;
            case play_state::playgoalie_st:
                //always should first go to goal
                coord_st = coordSkills_st::coordGotogoal_st; //needs to be set when role switches
                switch(coord_st){
                    case coordSkills_st::coordGotogoal_st:
                        skill.skill_goToPoint(&center);
                        if(true /*atGoal()*/){
                            coord_st = coordSkills_st::coordFollowball_st;
                        }
                        break;
                    case coordSkills_st::coordFollowball_st:
                    {
                        double ball_ycoord = 0; //ball location
                        double goalie_box = 0;
                        Point* point = new Point(ball_ycoord, goalie_box);
                        skill.skill_goToPoint(point);
                        if(true /*BallClose()*/){
                            coord_st = coordSkills_st::coordKick_st;
                        }
                    }
                        break;
                    case coordSkills_st::coordKick_st:
                        skill.skill_kick();
                        if(true /*BallKicked()*/){
                            coord_st = coordSkills_st::coordFollowball_st;
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
}
