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
    , coordSkill(*this)
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

    class CoordSkills { // more detailed coordinated skills
    public:

        CoordSkills(Plays& parent)
        : parent(parent)
        {
            rushGoalReset();
            playGoalieReset();
        }

        void rushGoalReset();
        void playGoalieReset();
        void rushGoalTick();
        void playGoalieTick();

    private:

        enum class CoordSkillState {
            idle,
            gotoGoal,
            kick,
            followBall,
            fetchBall,
            dribble,
            aim,
            splitFetchBall
        } playGoalie_st, rushGoal_st;

        Plays& parent;
    };

    CoordSkills coordSkill;

    //variables used for Kicking
    side myside;
    Point kp;
    Point startLocation;
};

#endif // PLAYS_H
