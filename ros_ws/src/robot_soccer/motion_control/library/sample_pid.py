import numpy as np
from numpy import matlib
from numpy import matrix
import globals
#import globals

class ControlVar:
    def __init__(self):
        self.integrator = 0
        self.velocity   = 0
        self.prev_error = 0
        self.prev_pos   = 0

x_g     = ControlVar()
y_g     = ControlVar()
theta_g = ControlVar()

def robot_ctrl(message):
    x       = message.x
    y       = message.y 
    theta   = message.theta

    x_cmd       = message.x_cmd
    y_cmd       = message.y_cmd

    angle = theta - message.theta_cmd
    if angle < -180:
	angle = -(360 - abs(angle))
    elif angle > 180:
	angle = 360 - angle
    else:
	angle = -angle

    theta_cmd = angle + theta
    # compute the desired angled angle using the outer loop control
    vx  = PID(x_cmd,x,x_g,globals.kp_x,globals.ki_x,globals.kd_x,globals.xy_limit,globals.Ts,globals.tau,globals.xy_thresh)
    vy  = PID(y_cmd,y,y_g,globals.kp_y,globals.ki_y,globals.kd_y,globals.xy_limit,globals.Ts,globals.tau,globals.xy_thresh)
    vth = PID(theta_cmd,theta,theta_g,globals.kp_theta,globals.ki_theta,globals.kd_theta,globals.th_limit,globals.Ts,globals.tau,globals.theta_thresh)

    if globals.print_speeds:
        print("==Speeds==")
        print("vx: ", vx)
        print("vy: ", vy)
        print("om: ", omega)

    return vx, vy, vth

def reset():
   x_g.velocity = 0
   x_g.integrator = 0
   x_g.prev_error = 0
   x_g.prev_pos = 0
   y_g.velocity = 0
   y_g.integrator = 0
   y_g.prev_error = 0
   y_g.prev_pos = 0
   theta_g.velocity = 0
   theta_g.integrator = 0
   theta_g.prev_error = 0
   theta_g.prev_pos = 0
    

# PID control for position
def PID(cmd_pos,pos,ctrl_vars,kp,ki,kd,limit,Ts,tau,thresh):
    # compute the error
    error = cmd_pos - pos
 #    if abs(error) < thresh:
 #        error = 0
 #    elif error > 0:
	# error = error - thresh
 #    else:
	# error = error + thresh
    # print("Error: " + str(error))
    
    # update derivative of z
    ctrl_vars.velocity = (2*tau-Ts)/(2*tau+Ts)*ctrl_vars.velocity + 2/(2*tau+Ts)*(pos-ctrl_vars.prev_pos)
    # update delayed variables for next time through the loop
    ctrl_vars.prev_error  = error;
    ctrl_vars.prev_pos    = pos;

    # compute the pid control signal
    u_unsat = kp*error + ki*ctrl_vars.integrator - kd*ctrl_vars.velocity;
    u = sat(u_unsat,limit)
    
    # integrator anti-windup
    if abs(error) < 10:
        # update integral of error
        ctrl_vars.integrator = ctrl_vars.integrator + (Ts/2)*(error+ctrl_vars.prev_error)
    else: # if we aren't close enough, don't use the integrator
        ctrl_vars.integrator = 0

    # if (ki!=0):
    #     ctrl_vars.integrator = ctrl_vars.integrator + Ts/ki*(u-u_unsat)

    return u

#-----------------------------------------------------------------
# saturation function
def sat(f,limit):

    if (f > limit):
        out = limit
        # print("sat at {}".format(limit))
    elif (f < -limit):
        out = -limit
        # print("sat at {}".format(limit))
    else:
        out = f

    return out
