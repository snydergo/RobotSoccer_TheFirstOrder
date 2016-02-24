class ControlVar:
    def __init__(self):
        self.integrator = 0
        self.velocity = 0
        self.prev_error = 0
        self.prev_pos = 0

class Param:
    def __init__(self):
        self.kp_x   = .02
        self.ki_x   = .001
        self.kd_x   = .000

        self.kp_y   = .02
        self.ki_y   = .001
        self.kd_y   = .000

        self.kp_th  = .01
        self.ki_th  = .0001
        self.kd_th  = 0

        self.Ts     = 1.00/loop_rate
        self.tau    = 0.005


loop_rate = 50 # Hz (get value from publisher)
x_g     = ControlVar()
y_g     = ControlVar()
theta_g = ControlVar()
P       = Param()


def robot_ctrl(message):
    x       = message.x
    y       = message.y 
    theta   = message.theta

    x_cmd       = message.x_cmd
    y_cmd       = message.y_cmd
    theta_cmd   = message.theta_cmd
    
    xy_limit = 0.50
    th_limit = 0.15
    # compute the desired angled angle using the outer loop control
    vx  = PID(x_cmd,x,x_g,P.kp_x,P.ki_x,P.kd_x,xy_limit,P.Ts,P.tau)
    vy  = PID(y_cmd,y,y_g,P.kp_y,P.ki_y,P.kd_y,xy_limit,P.Ts,P.tau)
    vth = PID(theta_cmd,theta,theta_g,P.kp_th,P.ki_th,P.kd_th,th_limit,P.Ts,P.tau)

    return vx, vy, vth

# PID control for position
def PID(cmd_pos,pos,ctrl_vars,kp,ki,kd,limit,Ts,tau):
    # compute the error
    error = cmd_pos - pos
    print("Error: " + str(error))
    
    # update derivative of z
    ctrl_vars.velocity = (2*tau-Ts)/(2*tau+Ts)*ctrl_vars.velocity + 2/(2*tau+Ts)*(pos-ctrl_vars.prev_pos)
    # update delayed variables for next time through the loop
    ctrl_vars.prev_error  = error;
    ctrl_vars.prev_pos    = pos;

    # compute the pid control signal
    u_unsat = kp*error + ki*ctrl_vars.integrator - kd*ctrl_vars.velocity;
    u = sat(u_unsat,limit)
    
    # integrator anti-windup
    if abs(error) > 30:
        # update integral of error
        ctrl_vars.integrator = ctrl_vars.integrator + (Ts/2)*(error+ctrl_vars.prev_error)

    if (ki!=0):
        ctrl_vars.integrator = ctrl_vars.integrator + Ts/ki*(u-u_unsat)

    return u

#-----------------------------------------------------------------
# saturation function
def sat(f,limit):

    if (f > limit):
        out = limit
    elif (f < -limit):
        out = -limit
    else:
        out = f

    return out
