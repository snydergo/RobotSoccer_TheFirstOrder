# utility - kalman filter for own team

import numpy as np
from numpy import matlib
from numpy import matrix
import globals


# xhat = matlib.zeros((3,1))
# S = matlib.zeros((3,3))

def utility_kalman_filter_robot(robot,v_command):
        
    # prediction step between measurements
    N = 10
    for i in range(1,N):
        f = matrix(v_command).reshape(3,1)
        robot.xhat = robot.xhat + (globals.loop_rate/N)*f
        # print robot.xhat
        robot.S = robot.S + (globals.loop_rate/N)*(globals.Q_ownteam)
    

    # correction step at measurement
    if robot.camera_flag:  # only update when the camera flag is one indicating a new measurement
        y_pred = robot.xhat  # predicted measurement
        L = np.diag(np.diag(robot.S)/np.diag(globals.R_ownteam+robot.S))
        robot.S = (np.eye(3)-L)*robot.S
        robot.xhat = robot.xhat + L*(matrix([robot.x,robot.y,robot.theta]).reshape(3,1)-y_pred)
        # print robot.x, robot.y, robot.theta

    # output current estimate of state
    xhat_array 	= robot.xhat.getA()

    robot.x 	= float(xhat_array[0])
    robot.y 	= float(xhat_array[1])
    robot.theta = float(xhat_array[2])

    # print robot.x, robot.y, robot.theta
    # robot.S 	= globals.S


	# xhat_ball
 #    xhat_delayed_ball
 #    S_ball
 #    S_delayed

def utility_kalman_filter_ball(ball):    
    
    if (t==0):  # initialize filter
        ball.xhat = matlib.zeros((8,1))
        ball.xhat_delayed = ball.xhat
        ball.S = np.diag([\
            globals.field_width/2, 	# initial variance of x-position of ball
            globals.field_width/2, 	# initial variance of y-position of ball 
            .01, 				# initial variance of x-velocity of ball 
            .01,				# initial variance of y-velocity of ball
            .001,				# initial variance of x-acceleration of ball 
            .001,				# initial variance of y-acceleration of ball
            .0001,				# initial variance of x-jerk of ball 
            .0001,				# initial variance of y-jerk of ball
            ]);
        ball.S_delayed=ball.S
    
    # prediction step between measurements
    N = 10;
    for i in range(1,N):
        xhat = xhat + (globals.loop_rate/N)*globals.A_ball*xhat;
        ball.S = ball.S + (globals.loop_rate/N)*(globals.A_ball*ball.S+ball.S*matrix.transpose(globals.A_ball)+globals.Q_ball)
 
    # correction step at measurement
    # only update when the camera flag is one indicating a new measurement
    # case 1 does not compensate for camera delay
    # case 2 compensates for fixed camera delay
    if ball.camera_flag == 1:
            y = ball.position_camera; # actual measurement
            y_pred = globals.C_ball*xhat;  # predicted measurement
            L = S*matrix.transpose(globals.C_ball)/(globals.R_ball+globals.C_ball*S*matrix.transpose(globals.C_ball))
            S = (np.eye(8)-L*globals.C_ball)*S;
            xhat = xhat + L*(y-y_pred);
    elif ball.camera_flag == 2:
            y = ball.position_camera; # actual measuremnt
            y_pred = globals.C_ball*ball.xhat_delayed;  # predicted measurement
            L = ball.S_delayed*matrix.transpose(globals.C_ball)/(globals.R_ball+globals.C_ball*ball.S_delayed*matrix.transpose(globals.C_ball))  
            ball.S_delayed = (np.eye(8)-L*globals.C_ball)*ball.S_delayed
            ball.xhat_delayed = ball.xhat_delayed + L*(y-y_pred);
            for i in range(1,N*(globals.camera_sample_rate/globals.loop_rate)):
                ball.xhat_delayed = ball.xhat_delayed + (globals.loop_rate/N)*(globals.A_ball*ball.xhat_delayed);
                ball.S_delayed = ball.S_delayed + (globals.loop_rate/N)*(globals.A_ball*ball.S_delayed+ball.S_delayed*matrix.transpose(globals.A_ball)+globals.Q_ball)
            ball.xhat = ball.xhat_delayed
            ball.S    = ball.S_delayed

    
    # output current estimate of state
    ball.position     = xhat[0:1];
    ball.velocity     = xhat[2:3];
    ball.acceleration = xhat[4:5];
    ball.jerk         = xhat[6:7];
    ball.S            = globals.S_ball;


def div0( a, b ):
    # ignore / 0, div0( [-1, 0, 1], 0 ) -> [0, 0, 0]
    with np.errstate(divide='ignore', invalid='ignore'):
        c = np.true_divide( a, b )
        c[ ~ np.isfinite( c )] = 0  # -inf inf NaN
    return c