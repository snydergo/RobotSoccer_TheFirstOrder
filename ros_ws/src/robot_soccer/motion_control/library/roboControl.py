#!/usr/bin/env python
import mlib
import sample_pid as pid
import globals

vx_valid = 0;
vy_valid = 0;
omega_valid = 0;
count = 0; # For use in timeouts

# Parses input 'data' and does appropriate action.
def roboControl(data):
## Decisions ##
    # print(data.cmdType)
    if data.cmdType == 'mov':
        if data.x == data.x and data.y == data.y and data.theta == data.theta: # check for NaN
            count = 0
            vx, vy, omega = pid.robot_ctrl(data)
           # store last valid values in case of NaN
            vx_valid = vx
            vy_valid = vy
            omega_valid = omega
            # Go, baby, go
            mlib.goXYOmegaWorld(vx,vy,omega,mlib.deg2rad(data.theta))
        else:
            # print("NaN - stopping")
            mlib.stop()



    elif data.cmdType == 'kick':
        mlib.kick()
    elif data.cmdType == 'kickinit':
        mlib.init_kick()
        print("kick inited")
    elif data.cmdType == 'kickuninit':
        mlib.uninit_kick()
    elif data.cmdType == 'idle' and (data.x != data.x or data.y != data.y):
        count = count + 1
        if count > 50: # Only try and go backwards for a little bit
            mlib.goXYOmegaWorld(-vx_valid,-vy_valid,-omega_valid,mlib.deg2rad(data.theta))
        else: # if you get NaNs for a while, just stop moving. There clearly a bigger issue.
            mlib.stop()
    elif data.cmdType == 'idle':
        mlib.stop()
