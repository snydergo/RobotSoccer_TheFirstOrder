"""Contains all of the plays that a Robot should be able to perform.
   This File uses utilities in certain orders to form plays.
   These plays are dependent on the desired strategies
"""
#Center field is origin towards away team and left facing home team is negative
from AI.gameplay.skills import Skills

class Location():
    x = 0
    y = 0


class  Plays(object):
    name = ""
    robotNum = 0
    role = "nothing"
    skills = Skills()

    """Simple constructor """
    def __init__(self, name, robotNum, role, skills):
        """Contains robotNumber so one knows which Robot to send Commands to"""
        self.robotNum = robotNum
        self.name = name
        self.role = role;
        skills = Skills(self.robotNum, self.name, self.role)
        print ("Num = %d name = %s role = %s" % (self.robotNum, self.name, self.role))


    """kick ball to a certain location"""
    def splitField(self, Location):
        print ("splitting field\n")

    """tells a certain robot to push goal"""
    def pushGoal(self, direction, distance):
        print ("pushing goal")

    def playGoalie(self):
        print ("Playing Goal!")

    def splitPlayGoalie(self):
        print ("Splitting Goal")



if __name__=="__main__":
    print("running Main for plays\n")
