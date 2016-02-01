"""Program Contains the Skills that are built upon the utilities that talk with the hardware"""
from AI.Gameplay.utilities import Utilities

class  Skills(object):
    name = ""
    robotNum = 0
    role = "nothing"
    utilities = Utilities(0, 0, 0)

    """Simple constructor """
    def __init__(self, name, robotNum, role, utilities):
        """Contains robotNumber so one knows which Robot to send Commands to"""
        self.robotNum = robotNum
        self.name = name
        self.role = role;
        utilities = Utilities(self.robotNum, self.name, self.role)
        print ("Num = %d name = %s role = %s" % (self.robotNum, self.name, self.role))


    """kick ball to a certain location"""
    def splitField(self, Location):
        print ("splitting field")

    """tells a certain robot to push goal"""
    def pushGoal(self, direction, distance):
        print ("direction = %d angle = %d." % (direction, distance))

    def playGoalie(self):
        print ("Playing Goal!")

    def splitPlayGoalie(self):
        print ("Splitting Goal")



if __name__=="__main__":
    print ("in main of Skills")
