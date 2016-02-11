#include <QCoreApplication>
#include "gamestatus.h"

GameStatus* initGamestatus();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GameStatus* game = initGamestatus();
    printf("Main ran");
    return a.exec();
}

GameStatus* initGameStatus(){
    Point* point = new Point(0,0);
    Robot* ally1 = new Robot("ally1", point, point, 0, ally1_rt);
    Robot* ally2 = new Robot("ally2", point, point, 0, ally2_rt);
    Robot* enemy1 = new Robot("enemy1", point, point, 0, enemy1_rt);
    Robot* enemy2 = new Robot("enemy2", point, point, 0, enemy2_rt);
    FieldObject* ball = new FieldObject("ball", point, point);
    return new GameStatus(ally1, ally2, enemy1, enemy2, ball);
}
