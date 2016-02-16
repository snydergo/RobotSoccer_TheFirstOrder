class ControlVar:
    def __init__(self):
        self.integrator = 0
        self.velocity = 0
        self.prev_error = 0
        self.prev_pos = 0

class Param:
    def __init__(self):
        self.kp_x   = 1
        self.ki_x   = 0.5
        self.kd_x   = 0.25

        self.kp_y   = 1.25
        self.ki_y   = 0.5
        self.kd_y   = 0.25

        self.kp_th  = 1.25
        self.ki_th  = 0.5
        self.kd_th  = 0.25

        self.Ts     = 0.033
        self.tau    = 0.005



x_g     = ControlVar()
y_g     = ControlVar()
theta_g = ControlVar()
P       = Param()

def robot_cntrl(robot):
    x       = robot.x
    y       = ronbot.y 
    theta   = robot.theta
    
    # compute the desired angled angle using the outer loop control
    vx  = PID(x_cmd,x,P.kp_x,P.ki_x,P.kd_x,2,P.Ts,P.tau)
    vy  = PID(y_cmd,y,P.kp_y,P.ki_y,P.kd_y,2,P.Ts,P.tau)
    vth = PID(theta_cmd,theta,P.kp_th,P.ki_th,P.kd_th,2,P.Ts,P.tau)

# PID control for position
def PID(cmd_pos,pos,kp,ki,kd,limit,Ts,tau):
   
    # compute the error
    error = cmd_pos-pos;
    # update integral of error
    cntrl_vars.integrator = cntrl_vars.integrator + (Ts/2)*(error+cntrl_vars.prev_error);
    # update derivative of z
    cntrl_vars.velocity = (2*tau-Ts)/(2*tau+Ts)*cntrl_vars.velocity + 2/(2*tau+Ts)*(pos-cntrl_vars.prev_pos);
    # update delayed variables for next time through the loop
    cntrl_vars.prev_error  = error;
    cntrl_vars.prev_pos    = pos;

    # compute the pid control signal
    u_unsat = kp*error + ki*cntrl_vars.integrator - kd*cntrl_vars.velocity;
    u = sat(u_unsat,limit);
    
    # integrator anti-windup
    if (ki!=0):
        cntrl_vars.integrator = cntrl_vars.integrator + Ts/ki*(u-u_unsat)

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