""" Class that imports information from Ross and Wraps them up to be used in GamePlay. Populates GameStatus
{"vision_data": {
    "ball": {
        "posX": 10,
        "posY": 10,
        "velX": 10,
        "velY": 10
    },
    "teamRobot1": {
        "posX": 10,
        "posY": 10,
        "theta": 10,
        "velX": 10,
        "velY": 10
    },
    "teamRobot2": {
        "posX": 10,
        "posY": 10,
        "theta": 10,
        "velX": 10,
        "velY": 10
    },
    "opponentRobot1": {
        "posX": 10,
        "posY": 10,
        "theta": 10,
        "velX": 10,
        "velY": 10
    },
    "opponentRobot2": {
        "posX": 10,
        "posY": 10,
        "theta": 10,
        "velX": 10,
        "velY": 10
    }
}}
"""

import json

from main_control import *
from visiondata import *
from string import *
#from main_control.StarKillerMainControl import glob_gameStatus


class DataImporter(object):

     def __init__(self):
         print("initstuff")

     def importBallInfo_json(self, string, starKiller):
         print("Importing ball info")
         createdBall = FieldObject("ball",Point(string['posX'],string['posY']),Direction(string['velX'],string['velY']))
         starKiller.gameStatus.ball = createdBall
         print(createdBall)
         return createdBall


     def importAllyRobots_json(self, robot1, robot2, starKiller):
         print("Importing Ally Robots")
         #need to make sure the wanted data is passed in
         ally1 = Robot("ally1",Point(robot1['posX'],robot1['posY']),Direction(robot1['velX'],robot1['velY']),robot1['theta'])
         starKiller.gameStatus.allyRobot1 = ally1
         print(ally1)
         ally2 = Robot("ally2",Point(robot2['posX'],robot2['posY']),Direction(robot2['velX'],robot2['velY']),robot2['theta'])
         starKiller.gameStatus.allyRobot2 = ally2
         print(ally2)
         return ally1.__str__() + ally2.__str__()

     def importRebelRobots_json(self, robot1, robot2,starKiller):
         print("Importing Rebel Robots")
         #need to make sure the wanted data is passed in
         enemy1 = Robot("enemy1",Point(robot1['posX'],robot1['posY']),Direction(robot1['velX'],robot1['velY']),robot1['theta'])
         starKiller.gameStatus.enemyRobot1 = enemy1
         print(enemy1)
         enemy2 = Robot("enemy2",Point(robot2['posX'],robot2['posY']),Direction(robot2['velX'],robot2['velY']),robot2['theta'])
         starKiller.gameStatus.enemyRobot2 = enemy2
         print(enemy2)
         return enemy1.__str__() + enemy2.__str__()

     def importGameStatus_json(self, Filename, starKiller):
         with open(Filename) as json_file:
             json_data = json.load(json_file)
             balljson = json_data['vision_data']['ball']
             allyRobot1 = json_data['vision_data']['teamRobot1']
             allyRobot2 = json_data['vision_data']['teamRobot2']
             rebelRobot1 = json_data['vision_data']['opponentRobot1']
             rebelRobot2 = json_data['vision_data']['opponentRobot2']
             #need to initialize data
             importer = DataImporter
             importer.importBallInfo(importer, balljson, starKiller)
             importer.importAllyRobots(importer, allyRobot1, allyRobot2, starKiller)
             importer.importRebelRobots(importer,rebelRobot1,rebelRobot2,starKiller)

     def importVisionData(self, vision_data):
         ally1 = Robot("ally1",Point(vision_data.tm0_x,vision_data.tm0_y),Direction(vision_data.tm0_x,vision_data.tm0_y),vision_data.tm0_w)
         glob_gameStatus.allyRobot1 = ally1
         print(ally1)
         ally2 = Robot("ally2",Point(vision_data.tm1_x,vision_data.tm1_y),Direction(vision_data.tm1_x,vision_data.tm1_y),vision_data.tm1_w)
         glob_gameStatus.allyRobot2 == ally2
         print(ally2)
         enemy1 = Robot("enemy1",Point(vision_data.op0_x,vision_data.op0_y),Direction(vision_data.op0_x,vision_data.op0_y),vision_data.op0_w)
         glob_gameStatus.enemyRobot1 = enemy1
         print(enemy1)
         enemy2 = Robot("enemy2",Point(vision_data.op1_x,vision_data.op1_y),Direction(vision_data.op1_x,vision_data.op1_y),vision_data.op1_w)
         glob_gameStatus.enemyRobot2 == enemy2
         print(enemy2)
         createdBall = FieldObject("ball",Point(vision_data.ball_x,vision_data.ball_y),Direction(vision_data.ball_x,vision_data.ball_y))
         glob_gameStatus.ball = createdBall
         print(createdBall)





if __name__=="__main__":
    with open("dataExample.json") as json_file:
        json_data = json.load(json_file)
        print(json_data)
        balljson = json_data['vision_data']['ball']
        print(balljson)
        allyRobot1 = json_data['vision_data']['teamRobot1']
        print(allyRobot1)
        allyRobot2 = json_data['vision_data']['teamRobot2']
        print(allyRobot2)
        rebelRobot1 = json_data['vision_data']['opponentRobot1']
        print(rebelRobot1)
        rebelRobot2 = json_data['vision_data']['opponentRobot2']
        theta = rebelRobot2['theta']
        print(rebelRobot2)
        print(theta)

        #need to initialize data
        importer = DataImporter
        ally1 = Robot("ally1",Point(0,0),Direction(0,0),0)
        ally2 = Robot("ally2",Point(0,0),Direction(0,0),0)
        enemy1 = Robot("enemy1",Point(0,0),Direction(0,0),0)
        enemy2= Robot("enemy2",Point(0,0),Direction(0,0),0)
        ball = FieldObject("ball",Point(0,0),Direction(0,0))
        gameStat = GameStatus(ally1,ally2,enemy1,enemy2,ball)
        deathstar = StarKillerData(gameStat)
        importer.importGameStatus(importer,"dataExample.json", deathstar)
        """
        importer.importBallInfo(importer, balljson, deathstar)
        importer.importAllyRobots(importer, allyRobot1, allyRobot2, deathstar)
        importer.importRebelRobots(importer,rebelRobot1,rebelRobot2,deathstar)
        """



