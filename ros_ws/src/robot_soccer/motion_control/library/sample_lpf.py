import numpy as np
from numpy import matlib
from numpy import matrix
import globals

# position = matlib.zeros(shape=(3,1))
# position_delayed = matlib.zeros(shape=(3,1))
# velocity = matlib.zeros(shape=(3,1))
# old_position_measurement = matlib.zeros(shape=(3,1))

class GamePieces(object):
    def __init__(self):
        self.op0 = Piece('op0')
        self.op1 = Piece('op1')
        self.tm0 = Piece('tm0')
        self.tm1 = Piece('tm1')
        self.ball = Piece('ball')

    def update_all(self, vision_msg):
        idx = {'x':0, 'y':1, 'w':2}
        for var in vars(vision_msg):
            name, t = var.split('_')
            if t in idx:
                exec("self.{0}.position[idx['{1}']] = vision_msg.{2}".format(name, t, var))
            exec("self.{}.camera_flag = 1".format(name))
                

class Piece(object):
    def __init__(self, name):
        self.name = ''
        self.position = matlib.zeros(shape=(3,1))
        self.old_position_measurement = matlib.zeros(shape=(3,1))
        self.position_delayed = matlib.zeros(shape=(3,1))
        self.camera_flag = 0
        self.vel = matlib.zeros((3,1))


def lpf(piece):


    piece.old_position_measurement = piece.position
    # dirty derivative coefficients
    a1 = (2*globals.tau-globals.camera_sample_rate)/(2*globals.tau+globals.camera_sample_rate)
    a2 = 2/(2*globals.tau+globals.camera_sample_rate)

    # compensates for camera delay and wall bounces (doesn't account for robot bounces)

    if piece.camera_flag: # correction
        # low pass filter position       
        piece.position_delayed = globals.lpf_alpha*piece.position_delayed + (1-globals.lpf_alpha)*piece.position_camera
        # compute velocity by dirty derivative of position
        piece.vel = a1*piece.vel + a2*(piece.position_camera-piece.old_position_measurement)
        piece.vel = utility_wall_bounce(piece.position_delayed,piece.vel)
        piece.old_position_measurement = piece.position_camera;
        # propagate up to current location
        for i in range(1,(globals.camera_sample_rate/globals.loop_rate)):
            piece.position_delayed = piece.position_delayed + globals.loop_rate*piece.vel
            piece.vel = utility_wall_bounce(piece.position_delayed,piece.vel)               
        piece.position = piece.position_delayed
    else: # prediction
        # propagate prediction ahead one control sample time
        piece.position = piece.position + globals.loop_rate*piece.vel
        piece.vel = utility_wall_bounce(piece.position,piece.vel)


#------------------------------------------
# utility - low pass filter ball position - differentiate for velocity
#
def utility_wall_bounce(position,velocity):
    # check for bounce off end walls
    if  abs(position[1]) >=  globals.field_length/2:
        velocity[1]=-velocity[1]
    # check for bounce off side walls
    if  abs(position[2]) >=  globals.field_width/2:
        velocity[2]=-velocity[2]

    return velocity


  