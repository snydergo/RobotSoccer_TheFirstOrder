import numpy as np
from numpy import matlib

kp_x	 = 0.02
ki_x	 = 0.0005
kd_x	 = 0.005
kp_y	 = kp_x
ki_y	 = ki_x
kd_y	 = kd_x
kp_theta = 0.02
ki_theta = 0.00
kd_theta = 0.00

xy_limit	 = 0.3 #0.5
th_limit	 = 0.2 #5

xy_thresh	 = 5
theta_thresh = 5

loop_rate = 20 # Hz (get value from publisher)
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

camera_sigma_robot_position = 2
camera_sigma_robot_position = 3
camera_sigma_robot_angle = np.pi/9

Q_ownteam = np.diag([1**2,1**2,(2*np.pi/180)**2])
R_ownteam = np.diag([camera_sigma_robot_position**2, camera_sigma_robot_position**2, camera_sigma_robot_angle**2])


# kp_x   = 0.05
# ki_x   = 0.00
# kd_x   = 0.02
# kp_y   = kp_x
# ki_y   = ki_x
# kd_y   = kd_x
# kp_theta  = 0.02
# ki_theta  = 0.00
# kd_theta  = 0 #0.01
# loop_rate = 20 # Hz (get value from publisher)
# Ts     = 1.00/loop_rate
# tau    = 0.005
# camera_sample_rate = 30
# lpf_alpha          = 0.7 
# a1      = (2*tau-camera_sample_rate)/(2*tau-tau+camera_sample_rate)
# a2      = 2/(2*tau+camera_sample_rate)

# xy_limit = .3#0.5
# th_limit = 2#5

# xy_thresh = 5
# theta_thresh = 5
