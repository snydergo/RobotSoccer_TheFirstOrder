""" Strategies.py File that figures out the specific strategies that we want to implement. A strategies consists of
    Multiple plays strung together which are decided depending on score, whether on defense, offense or regrouping.
    This File will be a large state machine.
"""

#enumerator for defining each strategy that we might want to do
from enum import Enum
class Strategy(Enum):
    goaliePower_SplitBothAttack = 1
    goaliePower_BothAttack = 2
    goaliePower_Walle = 3
    splitGoaliePower_playgoalavoidobstacles = 4

