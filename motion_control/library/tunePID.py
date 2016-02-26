#!/usr/bin/env python

import sample_pid as pid
from getch import _Getch

# Default commands
x_cmd = '0'
y_cmd = '0'
theta_cmd = '0'
x_g     = pid.ControlVar()
y_g     = pid.ControlVar()
theta_g = pid.ControlVar()
P = Param()
loop_rate = 10 # Hz (get value from publisher)
tuningVariable = 'x'
tuningSensitivity = .001

def get_action():
    getch = _Getch()
    pressed = getch()
    # Show commands (help)
    if pressed == 'h':
        return 'HELP'
    # Switch tuning variable
    elif pressed == '1':
        return 'X'
    elif pressed == '2':
        return 'Y'
    elif pressed == '3':
        return 'THETA'

    # change tuning paramaters for current tuning variable
    elif pressed == ',':
        return 'KP_DOWN'
    elif pressed == '.':
        return 'KP_UP'
    elif pressed == 'k':
        return 'KD_DOWN'
    elif pressed == 'l':
        return 'KD_UP'
    elif pressed == 'i':
        return 'KI_DOWN'
    elif pressed == 'o':
        return 'KI_UP'

    # Tools
    elif pressed == 'l':
        return 'LIST'
    elif pressed == 's':
        return 'SENSITIVITY'

    # change cmd position
    elif pressed == ' ':
        return 'CHANGE_CMD_POS'

def do_action(message):
    action = get_action()
    # Display commands (help)
    if action == 'HELP':
        print("Change tuning variable with 1,2,3")
        print("Tune kp with < >")
        print("Tune kd with k l")
        print("Tune ki with i o")
        print("Press l to show current gains")
        print("Press s to change tuning sensitivity")
        print("Press Space to change commanded position")
        print("Press h to bring up the commands (this menu)")

    # Switch tuning variable
    elif action == 'X':
        tuningVariable = 'x'
        print("changed tuning variable to x")
    elif action == 'Y':
        tuningVariable = 'y'
        print("changed tuning variable to y")
    elif action == 'THETA':
        tuningVariable = 'theta'
        print("changed tuning variable to theta")

    # change tuning paramaters for current tuning variable
    elif action == 'KP_DOWN':
        if tuningVariable == 'x':
            P.kp_x = P.kp_x - tuningSensitivity
            print("kp_x: " + str(P.kp_x))
        elif tuningVariable == 'y':
            P.kp_y = P.kp_y - tuningSensitivity
            print("kp_y: " + str(P.kp_y))
        elif tuningVariable == 'theta':
            P.kp_theta = P.kp_theta - tuningSensitivity
            print("kp_theta: " + str(P.kp_theta))
    elif action == 'KP_UP':
        if tuningVariable == 'x':
            P.kp_x = P.kp_x + tuningSensitivity
            print("kp_x: " + str(P.kp_x))
        elif tuningVariable == 'y':
            P.kp_y = P.kp_y + tuningSensitivity
            print("kp_y: " + str(P.kp_y))
        elif tuningVariable == 'theta':
            P.kp_theta = P.kp_theta + tuningSensitivity
            print("kp_theta: " + str(P.kp_theta))  
    elif action == 'KD_DOWN':
        if tuningVariable == 'x':
            P.kd_x = P.kd_x - tuningSensitivity
            print("kd_x: " + str(P.kd_x))
        elif tuningVariable == 'y':
            P.kd_y = P.kd_y - tuningSensitivity
            print("kd_y: " + str(P.kd_y))
        elif tuningVariable == 'theta':
            P.kd_theta = P.kd_theta - tuningSensitivity
            print("kd_theta: " + str(P.kd_theta))
    elif action == 'KD_UP':
         if tuningVariable == 'x':
            P.kd_x = P.kd_x + tuningSensitivity
            print("kd_x: " + str(P.kd_x))
        elif tuningVariable == 'y':
            P.kd_y = P.kd_y + tuningSensitivity
            print("kd_y: " + str(P.kd_y))
        elif tuningVariable == 'theta':
            P.kd_theta = P.kd_theta + tuningSensitivity
            print("kd_theta: " + str(P.kd_theta))       
    elif action == 'KI_DOWN':
         if tuningVariable == 'x':
            P.ki_x = P.ki_x - tuningSensitivity
            print("ki_x: " + str(P.ki_x))
        elif tuningVariable == 'y':
            P.ki_y = P.ki_y - tuningSensitivity
            print("ki_y: " + str(P.ki_y))
        elif tuningVariable == 'theta':
            P.ki_theta = P.ki_theta - tuningSensitivity
            print("ki_theta: " + str(P.ki_theta))       
    elif action == 'KI_UP':
         if tuningVariable == 'x':
            P.ki_x = P.ki_x + tuningSensitivity
            print("ki_x: " + str(P.ki_x))
        elif tuningVariable == 'y':
            P.ki_y = P.ki_y + tuningSensitivity
            print("ki_y: " + str(P.ki_y))
        elif tuningVariable == 'theta':
            P.ki_theta = P.ki_theta + tuningSensitivity
            print("ki_theta: " + str(P.ki_theta)) 


    elif action == 'LIST':
        print("kp_x: " + str(P.kp_x))
        print("kd_x: " + str(P.kd_x))
        print("ki_x: " + str(P.ki_x))
        print("\n")
        print("kp_y: " + str(P.kp_y))
        print("kd_y: " + str(P.kd_y))
        print("ki_y: " + str(P.ki_y))
        print("\n")
        print("kp_theta: " + str(P.kp_theta))
        print("kd_theta: " + str(P.kd_theta))
        print("ki_theta: " + str(P.ki_theta))
    elif action == 'SENSITIVITY':
        tuningSensitivity = input("Tuning Sensitivity: ")
        
    # change cmd position
    elif action == 'CHANGE_CMD_POS':
        x_cmd = input("x: ")
        y_cmd = input("y: ")
        theta_cmd = input("theta: ")

    # Current positions
    x       = message.x
    y       = message.y 
    theta   = message.theta

    
    xy_limit = 0.50
    th_limit = 0.25
    # compute the desired angled angle using the outer loop control
    vx  = pid.PID(x_cmd,x,x_g,P.kp_x,P.ki_x,P.kd_x,xy_limit,P.Ts,P.tau)
    vy  = pid.PID(y_cmd,y,y_g,P.kp_y,P.ki_y,P.kd_y,xy_limit,P.Ts,P.tau)
    vth = pid.PID(theta_cmd,theta,theta_g,P.kp_th,P.ki_th,P.kd_th,th_limit,P.Ts,P.tau)

    return vx, vy, vth