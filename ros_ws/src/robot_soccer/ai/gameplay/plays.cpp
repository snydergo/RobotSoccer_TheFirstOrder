/*Contains all of the plays or roles like goalie, rushgoal, split rush, split goal and etc*/
#include "plays.h"


//###FUNCTIONS THAT ARE USED OUTSIDE TO SET WHICH PLAY TO PERFORM.###//
void Plays::start(Point startLoc)
{
    startLocation = startLoc;
    play_st = PlayState::start;
}

void Plays::rushGoal()
{
    coordSkill.rushGoalReset();
    play_st = PlayState::rushGoal;
}

void Plays::playGoalie()
{
    coordSkill.playGoalieReset();
    play_st = PlayState::playGoalie;
}

void Plays::split_rushGoal(side gvnSide)
{
    myside = gvnSide;
    play_st = PlayState::splitRush;
}

void Plays::idle()
{
    play_st = PlayState::idle;
}

void Plays::initKicker()
{
    skill.init_kick();
}

void Plays::uninitKicker()
{
    skill.uninit_kick();
}

void Plays::CoordSkills::rushGoalReset()
{
    rushGoal_st = CoordSkillState::fetchBall;
}

void Plays::CoordSkills::playGoalieReset()
{
    playGoalie_st = CoordSkillState::gotoGoal;
}

void Plays::CoordSkills::rushGoalTick()
{
    std::cout << "Plays::tick() rushGoal ";
    switch (rushGoal_st) {
    case CoordSkillState::fetchBall:
        std::cout << " fetching ball" << std::endl;
        parent.skill.fetchBall();
        if (bkcalc::ballFetched(parent.allyNum)) {
            std::cout << "Plays::tick() BALL FETCHED" << std::endl;
            rushGoal_st = CoordSkillState::aim;
        }
        break;
    case CoordSkillState::aim:
        static uint8_t aim_cnt = 1;
        std::cout << " aiming ball::count == " << std::to_string(aim_cnt) << std::endl;
        parent.skill.aim();
        /*if (!bkcalc::ballFetched(allyNum)) {
             std::cout << "Plays::tick() BALL FETCHED" << std::endl;
             coord_st = CoordSkillsState::fetchBall;
        } else*/ if (bkcalc::ballAimed(parent.allyNum) || aim_cnt++ == AIM_MAX_CNT) {
            aim_cnt = 0;
            std::cout << "Plays::tick() BALL AIMED" << std::endl;
            parent.kp = bkcalc::kickPoint(parent.allyNum);
            rushGoal_st = CoordSkillState::kick;
        }
        break;
    case CoordSkillState::kick:
        std::cout << " kicking ball" << std::endl;
        parent.skill.goToPoint(moveSpeed::fast, parent.kp,bkcalc::getAngleTo(parent.allyNum,fieldget::getBallLoc()));
        if (bkcalc::ballKicked(parent.allyNum,parent.kp)) {
            std::cout << "Plays::tick() BALL KICKED" << std::endl;
            std::cout << "#############################################################################" << std::endl;
            rushGoal_st = CoordSkillState::fetchBall;
            parent.skill.kick();
        }
        break;
    default:
        //Throw Exception
        break;
    }
}

void Plays::CoordSkills::playGoalieTick()
{
    //always should first go to goal
    std::cout << "Plays::tick() playGoalie"<< std::endl;
    switch (playGoalie_st) {
    case CoordSkillState::gotoGoal:
        std::cout << "Skills::CoordSkillsState == gotogoal"<<std::endl;
        parent.skill.goToPoint(allyGoal, 0);
        if (bkcalc::atLocation(parent.allyNum, allyGoal)) {
            std::cout << "AT GOAL" << std::endl;
            playGoalie_st = CoordSkillState::followBall;
        }
        break;
    case CoordSkillState::followBall:
    {
        std::cout << "Skills::CoordSkillsState == Followball_st"<<std::endl;
        //if ball is close to robot
        if (bkcalc::ballKickZone(parent.allyNum)) {
            std::cout << "Plays::tick() BALL FOLLOWED" << std::endl;
            parent.kp = bkcalc::kickPoint(parent.allyNum);
            playGoalie_st = CoordSkillState::kick;
        } else{ //if ball isn't close to robot
            Point point;
            //if ball is within goalie box
            if (abs(fieldget::getBallLoc().y) < GOAL_RADIUS) {
                //std::cout << "ball is within Goal width" << std::endl;
                //double y = 2*fieldget::getBallLoc().y - fieldget::getRobotLoc(robotType::ally1).y;

                point = Point(allyGoal.x, fieldget::getBallLoc().y);
            } else{ //ball is outside of goalie box
                //std::cout << "ball is outside Goal width" << std::endl;
                double y_coord = allyGoal.y;
                if (fieldget::getBallLoc().y > 0)
                    y_coord += GOAL_RADIUS;
                else
                    y_coord -= GOAL_RADIUS;

                point = Point(allyGoal.x,y_coord);
            }
            double cmdTheta = bkcalc::getAngleTo(parent.allyNum,fieldget::getBallLoc());
            parent.skill.goToPoint(point,cmdTheta);
        }
    }
        break;
    case CoordSkillState::kick:
        std::cout << "Skills::CoordSkillsState == kick_st"<<std::endl;
        parent.skill.kick();
        parent.skill.goToPoint(parent.kp,bkcalc::getAngleTo(parent.allyNum,fieldget::getBallLoc()));
        if (bkcalc::ballKicked(parent.allyNum, parent.kp)) {
            std::cout << "Plays::tick() BALL KICKED" << std::endl;
            playGoalie_st = CoordSkillState::gotoGoal;
        }
    break;
    default:
        //Throw Exception
        break;
    }
}

//### END OF FUNCTIONS CALLED OUTSIDE ###//
void Plays::tick()
{
    //needed changes
    /*
        Needed Changes
            1. when roles/plays are assigned each state needs to be initialized appropriately
            2. Need each time coord_st switch statement occurs that a function from skills is called
            3. Need to add an exception code for when default case is reached in statemachines.
    */
    switch (play_st) {
    case PlayState::idle:
        std::cout << "Plays::tick() idle_st"<< std::endl;
        skill.idle();
        break;
    case PlayState::start:
        std::cout << "Plays::tick() start_st"<< std::endl;
        skill.goToPoint(startLocation, 0);
        break;
    case PlayState::rushGoal:
        coordSkill.rushGoalTick();
        break;
    case PlayState::playGoalie:
        coordSkill.playGoalieTick();
        break;
    default:
        //Throw Exception
        break;
    }
    //need skill to perform action depending on change
    skill.tick();
}
