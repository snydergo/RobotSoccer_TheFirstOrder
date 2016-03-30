import numpy as np
from numpy import matlib
from numpy import matrix
from robot_soccer.msg import visiondata
import globals

# position = matlib.zeros(shape=(3,1))
# position_delayed = matlib.zeros(shape=(3,1))
# velocity = matlib.zeros(shape=(3,1))
# old_position_measurement = matlib.zeros(shape=(3,1))

def noNaNs(pos):
    for i in pos:
        if i != i:
            return False
    return True

# def checkBounds(pos, floor, ceil):
#     for c in pos:
#         if c > ceil:
#             print('out of bounds: ', c)
#         if c < floor:
#             print('out of bounds: ', c)

class GamePieces(object):
    def __init__(self):
        piece_names = ['op0', 'op1', 'tm0', 'tm1', 'ball'] # names from visiondata data type
        self.pieces = {name: Piece(name) for name in piece_names}

    def update_all(self, vision_msg):
        msg = {name: getattr(vision_msg, name) for name in globals.vision_msg_fields}

        for name, piece in self.pieces.items():
            pos_l = [msg['{}_x'.format(name)], msg['{}_y'.format(name)], msg['{}_w'.format(name)]]
            pos = [round(i,3) for i in pos_l]
            # checkBounds(pos)
            if noNaNs(pos):
                setattr(piece, 'position', matrix(pos).reshape(3,1))
                setattr(piece, 'camera_flag', True)

        # self.op0.position = matrix([vision_msg.op0_x, vision_msg.op0_y, vision_msg.op0_w]).reshape(3,1)
        # self.op0.camera_flag = 1
        # self.op1.position = matrix([vision_msg.op1_x, vision_msg.op1_y, vision_msg.op1_w]).reshape(3,1)
        # self.op1.camera_flag = 1
        # self.tm0.position = matrix([vision_msg.tm0_x, vision_msg.tm0_y, vision_msg.tm0_w]).reshape(3,1)
        # self.tm0.camera_flag = 1
        # self.tm1.position = matrix([vision_msg.tm1_x, vision_msg.tm1_y, vision_msg.tm1_w]).reshape(3,1)
        # self.tm1.camera_flag = 1
        # self.ball.position = matrix([vision_msg.ball_x, vision_msg.ball_y, 0]).reshape(3,1)
        # self.ball.camera_flag = 1

    def filter_all(self):
        for piece in self.pieces.values():
            lp_filter(piece)

    def gen_msg(self):
        msg = visiondata()

        for name, piece in self.pieces.items():
            setattr(msg, '{}_x'.format(name), piece.position[0])
            setattr(msg, '{}_y'.format(name), piece.position[1])
            setattr(msg, '{}_w'.format(name), piece.position[2])

        # msg.tm0_x = self.tm0.position[0]
        # msg.tm0_y = self.tm0.position[1]
        # msg.tm0_w = self.tm0.position[2]
        # msg.tm1_x = self.tm1.position[0]
        # msg.tm1_y = self.tm1.position[1]
        # msg.tm1_w = self.tm1.position[2]
        # msg.op0_x = self.op0.position[0]
        # msg.op0_y = self.op0.position[1]
        # msg.op0_w = self.op0.position[2]
        # msg.op1_x = self.op1.position[0]
        # msg.op1_y = self.op1.position[1]
        # msg.op1_w = self.op1.position[2]
        # msg.ball_x = self.ball.position[0]
        # msg.ball_y = self.ball.position[1]

        return msg              

class Piece(object):
    def __init__(self, name):
        self.name = ''
        self.position = matlib.zeros(shape=(3,1))
        self.old_position_measurement = matlib.zeros(shape=(3,1))
        self.position_delayed = matlib.zeros(shape=(3,1))
        self.camera_flag = 0
        self.vel = matlib.zeros((3,1))


def lp_filter(piece):


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
        peice.camera_flag = 0
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


  