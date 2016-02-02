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
from AI import StarKillerMainControl
from AI.visiondata.GameStatus import *
from string import *



class DataImporter(object):

    def __init__(self):
        print("initstuff")

    def importAllyRobots(self, string : str, starKiller : StarKillerMainControl):
        print("importing Data")
        if '__type__' in str and str['__type__'] == 'teamRobot1':
            firstOrderRobot1 = Robot(Point(str['posX'],str['posY']), Direction(str['velX'],str['velY']), str['theta'])
            starKiller.gameStatus.allyRobot1 = firstOrderRobot1
            return firstOrderRobot1
        else:
            raise VisionAIException(AIException.parsingData)

    def importRebelRobots(self, string : str, starKiller : StarKillerMainControl):
        print("importing Rebel Robots info")
        if '__type__' in str and str['__type__'] == 'opponentRobot1':
            rebel1 = Robot(Point(str['posX'],str['posY']), Direction(str['velX'],str['velY']), str['theta'])
            starKiller.gameStatus.enemyRobot1 = rebel1
            return rebel1
        else:
            raise VisionAIException(AIException.parsingData)

    def importBallInfo(self, string : str, starKiller : StarKillerMainControl) -> FieldObject:
        print("importing ball info")
        if '__type__' in str and str['__type__'] == 'ball':
            ball = FieldObject(Point(str['posX'],str['posY']), Direction(str['velX'],str['velY']))
            starKiller.gameStatus.ball = ball
            return ball
        else:
            raise VisionAIException(AIException.parsingData)


