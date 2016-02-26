import numpy as np
from numpy import matlib
from numpy import matrix

position = matlib.zeros(shape=(3,1))
position_delayed = matlib.zeros(shape=(3,1))
velocity = matlib.zeros(shape=(3,1))
old_position_measurement matlib.zeros(shape=(3,1))
a1 = (2*P.dirty_derivative_gain-P.camera_sample_rate)/(2*P.dirty_derivative_gain+P.camera_sample_rate)
a2 = 2/(2*P.dirty_derivative_gain+P.camera_sample_rate)

class Ball(object):
    def __init__(self):
        self.position = matlib.zeros(shape=(3,0))
        self.old_position_measurement = matlib.zeros(shape=(3,0))
        self.position_delayed = matlib.zeros(shape=(3,0))
        self.camera_flag = 0
        self.state = State()

def utility_lpf_ball(ball,P):

    if (t==0):  # initialize filter
        position = ball.position
        position_delayed = position
        velocity = matlib.zeros(shape=(2,1))
        old_position_measurement = position
        # dirty derivative coefficients
        a1 = (2*P.tau-P.camera_sample_rate)/(2*P.tau+P.camera_sample_rate)
        a2 = 2/(2*P.tau+P.camera_sample_rate)
    
   # compensates for camera delay and wall bounces (doesn't account for robot bounces)

    if ball.camera_flag: # correction
        # low pass filter position       
        ball.position_delayed = P.lpf_alpha*ball.position_delayed + (1-P.lpf_alpha)*ball.position_camera;
        # compute velocity by dirty derivative of position
        ball.state.vel = a1*ball.state.vel + a2*(ball.position_camera-ball.old_position_measurement);
        ball.state.vel = utility_wall_bounce(ball.position_delayed,ball.state.vel,P);
        ball.old_position_measurement = ball.position_camera;
        # propagate up to current location
        for i=(1:(P.camera_sample_rate/P.control_sample_rate)):
            ball.position_delayed = ball.position_delayed + P.control_sample_rate*ball.state.vel;
            ball.state.vel = utility_wall_bounce(ball.position_delayed,ball.state.vel,P);                
        ball.position = ball.position_delayed
    else # prediction
        # propagate prediction ahead one control sample time
        ball.position = ball.position + P.control_sample_rate*ball.state.vel;
        ball.state.vel = utility_wall_bounce(ball.position,ball.state.vel,P);

    
    # output current estimate of state
    ball.position     = position;
    ball.state.vel    = velocity;


#------------------------------------------
# utility - low pass filter ball position - differentiate for velocity
#
def utility_wall_bounce(position,velocity,P):
    # check for bounce off end walls
    if  abs(position[1]) >=  P.field_length/2:
        velocity[1]=-velocity[1]
    # check for bounce off side walls
    if  abs(position[2]) >=  P.field_width/2,
        velocity[2]=-velocity[2]

    return velocity


  