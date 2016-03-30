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
    coord_st = CoordSkillState::fetchBall;
    play_st = PlayState::rushGoal;
}

void Plays::playGoalie()
{
    coord_st = CoordSkillState::gotoGoal;
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

void Plays::rushGoalTick()
{
//    std::cout << "Plays::tick() rushGoal ";
    switch (coord_st) {
    case CoordSkillState::fetchBall:
        std::cout << " fetching ball" << std::endl;
        skill.fetchBall();
        if (calc::ballFetched(fieldget::getRobot(allyNum), fieldget::getBall())) {
            std::cout << "Plays::tick() BALL FETCHED" << std::endl;
            coord_st = CoordSkillState::aim;
        }
        break;
    case CoordSkillState::aim:
        static uint16_t aim_cnt = 1;
        std::cout << " aiming ball::count == " << std::to_string(aim_cnt) << std::endl;
        skill.aim();
        /*if (!calc::ballFetched(fieldget::getRobot(allyNum))) {
             std::cout << "Plays::tick() BALL FETCHED" << std::endl;
             coord_st = CoordSkillsState::fetchBall;
        } else*/
        if (bkcalc::ballAimed(allyNum) || aim_cnt++ >= AIM_MAX_CNT) {
            aim_cnt = 0;
  //          std::cout << "#############################################################################" << std::endl;
            std::cout << "Plays::tick() BALL AIMED" << std::endl;
            kp = bkcalc::kickPoint(allyNum);
            coord_st = CoordSkillState::kick;
        }
        break;
    case CoordSkillState::kick:
        std::cout << " kicking ball" << std::endl;
        skill.goToPoint(moveSpeed::fast, kp,bkcalc::getAngleTo(allyNum,fieldget::getBallLoc()));
        if (calc::ballKicked(fieldget::getRobot(allyNum),kp)) {
            std::cout << "Plays::tick() BALL KICKED" << std::endl;
//            std::cout << "#############################################################################" << std::endl;
            coord_st = CoordSkillState::fetchBall;
            skill.kick();
        } else if (calc::getDistance(fieldget::getRobotLoc(allyNum), fieldget::getBall().location) >= KICK_INVALID_DIST){
            std::cout << " not within kick range" << std::endl;
            coord_st = CoordSkillState::fetchBall;
        }
        break;
    default:
        //Throw Exception
        break;
    }
}

void Plays::playGoalieTick()
{
    //always should first go to goal
    std::cout << "Plays::tick() playGoalie"<< std::endl;
    switch (coord_st) {
    case CoordSkillState::gotoGoal:
        std::cout << "Skills::CoordSkillsState == gotogoal"<<std::endl;
        skill.goToPoint(allyGoal, 0);
        if (calc::atLocation(fieldget::getRobotLoc(allyNum), allyGoal)) {
            std::cout << "AT GOAL" << std::endl;
            coord_st = CoordSkillState::followBall;
        }
        break;
    case CoordSkillState::followBall:
    {
        std::cout << "Skills::CoordSkillsState == Followball_st"<<std::endl;
        //if ball is close to robot
        if (bkcalc::ballKickZone(allyNum)) {
            std::cout << "Plays::tick() BALL FOLLOWED" << std::endl;
            kp = bkcalc::kickPoint(allyNum);
            coord_st = CoordSkillState::kick;
        } else{ //if ball isn't close to robot
            Point point;
            //if ball is within goalie box
            if (abs(fieldget::getBallLoc().y) < GOAL_RADIUS) {
                //std::cout << "ball is within Goal width" << std::endl;
                //double y = 2*fieldget::getBallLoc().y - fieldget::getRobotLoc(RobotType::ally1).y;

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
            double cmdTheta = bkcalc::getAngleTo(allyNum,fieldget::getBallLoc());
            skill.goToPoint(point,cmdTheta);
        }
    }
        break;
    case CoordSkillState::kick:
        std::cout << "Skills::CoordSkillsState == kick_st"<<std::endl;
        skill.kick();
        skill.goToPoint(kp,bkcalc::getAngleTo(allyNum,fieldget::getBallLoc()));
        if (calc::ballKicked(fieldget::getRobot(allyNum), kp)) {
            std::cout << "Plays::tick() BALL KICKED" << std::endl;
            coord_st = CoordSkillState::gotoGoal;
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
        rushGoalTick();
        break;
    case PlayState::playGoalie:
        playGoalieTick();
        break;
    default:
        //Throw Exception
        break;
    }
    //need skill to perform action depending on change
    skill.tick();
}
