import numpy as np
from numpy import matlib
from numpy import matrix
import globals

position = matlib.zeros(shape=(3,1))
position_delayed = matlib.zeros(shape=(3,1))
velocity = matlib.zeros(shape=(3,1))
old_position_measurement = matlib.zeros(shape=(3,1))

class Ball(object):
    def __init__(self):
        self.position = matlib.zeros(shape=(3,0))
        self.old_position_measurement = matlib.zeros(shape=(3,0))
        self.position_delayed = matlib.zeros(shape=(3,0))
        self.camera_flag = 0
        self.state = State()

def utility_lpf_ball(ball):


    old_position_measurement = position
    # dirty derivative coefficients
    a1 = (2*globals.tau-globals.camera_sample_rate)/(2*globals.tau+globals.camera_sample_rate)
    a2 = 2/(2*globals.tau+globals.camera_sample_rate)

    # compensates for camera delay and wall bounces (doesn't account for robot bounces)

    if ball.camera_flag: # correction
        # low pass filter position       
        ball.position_delayed = globals.lpf_alpha*ball.position_delayed + (1-globals.lpf_alpha)*ball.position_camera;
        # compute velocity by dirty derivative of position
        ball.state.vel = a1*ball.state.vel + a2*(ball.position_camera-ball.old_position_measurement);
        ball.state.vel = utility_wall_bounce(ball.position_delayed,ball.state.vel,P);
        ball.old_position_measurement = ball.position_camera;
        # propagate up to current location
        for i in range(1,(globals.camera_sample_rate/globals.control_sample_rate)):
            ball.position_delayed = ball.position_delayed + globals.control_sample_rate*ball.state.vel;
            ball.state.vel = utility_wall_bounce(ball.position_delayed,ball.state.vel,P);                
        ball.position = ball.position_delayed
    else: # prediction
        # propagate prediction ahead one control sample time
        ball.position = ball.position + globals.control_sample_rate*ball.state.vel;
        ball.state.vel = utility_wall_bounce(ball.position,ball.state.vel,P);


    # output current estimate of state
    ball.position     = position;
    ball.state.vel    = velocity;


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


  