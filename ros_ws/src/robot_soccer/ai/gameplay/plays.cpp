/*Contains all of the plays or roles like goalie, rushgoal, split rush, split goal and etc*/
#include "plays.h"


//###FUNCTIONS THAT ARE USED OUTSIDE TO SET WHICH PLAY TO PERFORM.###//
void Plays::start(Point startLoc)
{
    stream::info << "PLAYS::start:: START LOCATION (" << startLoc.x << ", " << startLoc.y << ")\n";
    startLocation = startLoc;
    play_st = PlayState::start;
}

void Plays::rushGoal()
{
    coord_st = CoordSkillState::aim;
    play_st = PlayState::rushGoal;
}

void Plays::playGoalie()
{
    coord_st = CoordSkillState::gotoGoal;
    play_st = PlayState::playGoalie;
}

void Plays::rushSplit(side gvnSide)
{
    //boundary = (gvnSide == side::pos || gvnSide == side::left) ? -SPLIT_OFFSET: SPLIT_OFFSET;
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
//    case CoordSkillState::fetchBall:
//        std::cout << " fetching ball" << std::endl;
//        skill.fetchBall();
//        if (calc::ballFetched(fieldget::getRobot(allyNum), fieldget::getBall())) {
//            std::cout << "Plays::tick() BALL FETCHED" << std::endl;
//            coord_st = CoordSkillState::aim;
//        }
//        break;
    case CoordSkillState::aim:

        if (bkcalc::ballAimed(allyNum)) {
            std::cout << "Plays::tick() BALL AIMED" << std::endl;
            kp = bkcalc::kickPoint(allyNum);
            coord_st = CoordSkillState::kick;
        }
        skill.aim();
        break;
//        static uint16_t aim_cnt = 1;
//        std::cout << " aiming ball::count == " << std::to_string(aim_cnt) << std::endl;

        /*if (!calc::ballFetched(fieldget::getRobot(allyNum))) {
             std::cout << "Plays::tick() BALL FETCHED" << std::endl;
             coord_st = CoordSkillsState::fetchBall;
        } else*/
    case CoordSkillState::kick:
        std::cout << " kicking ball" << std::endl;
        if (calc::ballKicked(fieldget::getRobot(allyNum),kp)) {
            std::cout << "Plays::tick() BALL KICKED" << std::endl;
            coord_st = CoordSkillState::aim;
            skill.kick();
        } else if (calc::getDistance(fieldget::getRobotLoc(allyNum), fieldget::getBall().location) >= KICK_INVALID_DIST){
            std::cout << " not within kick range" << std::endl;
            coord_st = CoordSkillState::aim;
        }
        skill.goToPoint(moveSpeed::fast, kp,bkcalc::getAngleTo(allyNum,fieldget::getBallLoc()));

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
void Plays::splitRushTick(){
    Point ball = fieldget::getBallLoc();
    switch(myside){
    case side::pos:
    case side::left:
        if(ball.y > -SPLIT_OFFSET){
            rushGoalTick();
        }else{
            skill.goToPoint(Point(ball.x-SPLIT_WAIT_XOFFSET, SPLIT_WAIT_Y),0);
        }
        break;
    case side::neg:
    case side::right:
        if(ball.y < SPLIT_OFFSET){
            rushGoalTick();
        }else{
            skill.goToPoint(Point(ball.x-SPLIT_WAIT_XOFFSET, -SPLIT_WAIT_Y),0);
        }
        break;
    }
}

//### END OF FUNCTIONS CALLED OUTSIDE ###//
void Plays::tick()
{
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
    case PlayState::splitRush:
        splitRushTick();
    default:
        //Throw Exception
        break;
    }
    //need skill to perform action depending on change
    skill.tick();
}
