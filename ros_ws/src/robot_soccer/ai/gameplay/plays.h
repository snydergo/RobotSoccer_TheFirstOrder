#ifndef PLAYS_H
#define PLAYS_H

#include "skills.h"

class Plays
{


public:

    Plays(robotType type)
    : allyNum(type)
    , skill(type)
    , myside(side::none)
    , startLocation(center)
    , kp(center)
    , play_st(PlayState::idle)
    {
    }

    void split_rushGoal(side gvnSide);
    void initKicker();
    void uninitKicker();
    void rushGoal();
    void playGoalie();
    void idle();
    void tick();
    void init();
    void start(Point startLoc);
    robotType allyNum;

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
