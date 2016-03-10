kp_x   = 0.1
ki_x   = 0.00
kd_x   = 0.02
kp_y   = kp_x
ki_y   = ki_x
kd_y   = kd_x
kp_theta  = 0.03
ki_theta  = 0.000
kd_theta  = 0.01
loop_rate = 20 # Hz (get value from publisher)
Ts     = 1.00/loop_rate
tau    = 0.005
camera_sample_rate = 30
lpf_alpha          = 0.7 
a1      = (2*tau-camera_sample_rate)/(2*tau-tau+camera_sample_rate)
a2      = 2/(2*tau+camera_sample_rate)

xy_limit = 0.5
th_limit = 5

xy_thresh = 5
theta_thresh = 10
