import numpy as np
from numpy import matlib

# Troy's values

kp_x	 = 0.02
ki_x	 = 0.001
kd_x	 = 0.0
kp_y	 = kp_x
ki_y	 = ki_x
kd_y	 = kd_x
kp_theta = 0.01
ki_theta = 0.001
kd_theta = 0.00

xy_limit	 = 1.0#.4
th_limit	 = 1.0#0.2 #5

xy_thresh	 = 100#5
theta_thresh = 100#5

loop_rate = 50 # Hz (get value from publisher)
Ts     = 1.00/loop_rate
tau    = 0.005
camera_sample_rate = 33
lpf_alpha          = 0.7 
a1      = (2*tau-camera_sample_rate)/(2*tau-tau+camera_sample_rate)
a2      = 2/(2*tau+camera_sample_rate)

# field characteristics
field_length  = 3.048; # meters (5 ft)
field_width   = 1.52; # meters (10 ft)

xhat = matlib.zeros((3,1))
S = np.diag([\
    field_width/2,      # initial variance of x-position of ownteam i
    field_width/2,      # initial variance of y-position of ownteam i 
    (5*np.pi/180)**2,               # initial variance of angle of ownteam i 
    ]);

camera_sample_rate = 10*loop_rate
# noise levels on the camera
camera_sigma_ball = 0.01 # units are meters
camera_sigma_robot_position = 0.01 # units are meters
camera_sigma_robot_angle = 2*np.pi/180 # units are radians

# Q_ownteam = np.diag([1**2,1**2,(2*np.pi/180)**2])
# R_ownteam = np.diag([camera_sigma_robot_position**2, camera_sigma_robot_position**2, camera_sigma_robot_angle**2])


# parameters for the Kalman filter
Q_ownteam = np.diag([1**2,1**2,(2*np.pi/180)**2])
R_ownteam = np.diag([camera_sigma_robot_position**2, camera_sigma_robot_position**2, camera_sigma_robot_angle**2])
Q_opponent = 1*np.diag([.001**2, .001**2, .01**2, 0.1**2, 0.1**2, 0.1**2, 1**2,1**2,(2*np.pi/180)**2])
R_opponent = np.diag([camera_sigma_robot_position**2, camera_sigma_robot_position**2, camera_sigma_robot_angle**2])
Q_ball = 1*np.diag([.001**2,.001**2,.01**2,.01**2, .1**2, .1**2, 10**2, 10**2])
R_ball = np.diag([camera_sigma_ball**2, camera_sigma_ball**2])


# # parameters for opponent Kalman filter
# A_opponent = np.zeros((9,9))

# C_opponent = [np.eye(3), np.zeros((3,6))]

# # parameters for ball Kalman filter
# A_ball = [...
#             zeros(2,2), eye(2), zeros(2,2), zeros(2,2);...
#             zeros(2,2), zeros(2,2), eye(2), zeros(2,2);...
#             zeros(2,2), zeros(2,2), zeros(2,2), eye(2);...
#             zeros(2,2), zeros(2,2), zeros(2,2), zeros(2,2);...
#             ];
# C_ball = [eye(2), zeros(2,6)];

