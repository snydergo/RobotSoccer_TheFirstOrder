# utility - kalman filter for own team

import numpy as np
from numpy import matlib
from numpy import matrix


xhat = matlib.zeros(3,1)
S = matlib.zeros(3,3)

def utility_kalman_filter_robot(robot,v_command,t,P):
        
    if (t==0): # initialize filter
        robobt.xhat = matlib.zeros(3,1)
        robot.S = np.diag([\
            P.field_width/2,	 # initial variance of x-position of ownteam i
            P.field_width/2, 	# initial variance of y-position of ownteam i 
            (5*pi/180)^2,		# initial variance of angle of ownteam i 
            ]);

    # prediction step between measurements
    N = 10
    for i in range(1,N):
        f = matrix(v_command).reshape(3,1)
        robot.xhat = xhat + (P.control_sample_rate/N)*f
        robot.S = robots.S + (P.control_sample_rate/N)*(P.Q_ownteam)

    # correction step at measurement
    if robot.camera_flag:  # only update when the camera flag is one indicating a new measurement
        y_pred = xhat  # predicted measurement
        L = robot.S/(P.R_ownteam+robot.S)
        robots.S = (matrix.eye(3)-L)*S
        robot.xhat = robot.xhat + L*(matrix([robot.x],[robot.y],[robot.theta])-y_pred)

    # output current estimate of state
    xhat_array 	= xhat.getA()
    robot.x 	= xhat_array[0]
    robot.y 	= xhat_array[1]
    robot.theta = xhat_array[2]
    robot.S 	= S


	# xhat_ball
 #    xhat_delayed_ball
 #    S_ball
 #    S_delayed

def utility_kalman_filter_ball(ball,t,P):    
    
    if (t==0):  # initialize filter
        ball.xhat = matlib.zeros(8,1)
        ball.xhat_delayed = ball.xhat
        ball.S = np.diag([\
            P.field_width/2, 	# initial variance of x-position of ball
            P.field_width/2, 	# initial variance of y-position of ball 
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
        xhat = xhat + (P.control_sample_rate/N)*P.A_ball*xhat;
        ball.S = ball.S + (P.control_sample_rate/N)*(P.A_ball*ball.S+ball.S*matrix.transpose(P.A_ball)+P.Q_ball)
 
    # correction step at measurement
    # only update when the camera flag is one indicating a new measurement
    # case 1 does not compensate for camera delay
    # case 2 compensates for fixed camera delay
    if ball.camera_flag == 1:
            y = ball.position_camera; # actual measurement
            y_pred = P.C_ball*xhat;  # predicted measurement
            L = S*matrix.transpose(P.C_ball)/(P.R_ball+P.C_ball*S*matrix.transpose(P.C_ball))
            S = (eye(8)-L*P.C_ball)*S;
            xhat = xhat + L*(y-y_pred);
    elif ball.camera_flag == 2:
            y = ball.position_camera; # actual measuremnt
            y_pred = P.C_ball*ball.xhat_delayed;  # predicted measurement
            L = ball.S_delayed*matrix.transpose(P.C_ball)/(P.R_ball+P.C_ball*ball.S_delayed*matrix.transpose(P.C_ball))  
            ball.S_delayed = (matrix.eye(8)-L*P.C_ball)*ball.S_delayed
            ball.xhat_delayed = ball.xhat_delayed + L*(y-y_pred);
            for i in range(1,N*(P.camera_sample_rate/P.control_sample_rate)):
                ball.xhat_delayed = ball.xhat_delayed + (P.control_sample_rate/N)*(P.A_ball*ball.xhat_delayed);
                ball.S_delayed = ball.S_delayed + (P.control_sample_rate/N)*(P.A_ball*ball.S_delayed+ball.S_delayed*matrix.transpose(P.A_ball)+P.Q_ball)
            ball.xhat = ball.xhat_delayed
            ball.S    = ball.S_delayed

    
    # output current estimate of state
    ball.position     = xhat[0:1];
    ball.velocity     = xhat[2:3];
    ball.acceleration = xhat[4:5];
    ball.jerk         = xhat[6:7];
    ball.S            = P.S_ball;