#ifndef PLAYS_H
#define PLAYS_H

#include "skills.h"

class Plays
{


public:

    Plays(RobotType type)
    : allyNum(type)
    , skill(type)
    , myside(side::none)
    , startLocation(center)
    , kp(center)
    , play_st(PlayState::idle)
    {
    }

    void rushSplit(side gvnSide);
    void splitRushTick();
    void initKicker();
    void uninitKicker();
    void rushGoal();
    void playGoalie();
    void idle();
    void tick();
    void init();
    void start(Point startLoc);
    RobotType allyNum;

private:
    Skills skill;

    enum class PlayState {
        idle,
        start,
        rushGoal,
        playGoalie,
        splitRush,
        splitDefense
    } play_st;

    enum class CoordSkillState {
        idle,
        gotoGoal,
        kick,
        followBall,
        fetchBall,
        dribble,
        aim,
        splitFetchBall
    } coord_st;

    void rushGoalTick();
    void playGoalieTick();

    //variables used for Kicking
    side myside;
    Point kp;
    Point startLocation;
};

#endif // PLAYS_H
