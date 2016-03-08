% Dynamics of two robot soccer teams including the ball.  Ball interactions
% with the wall and other robots are modeled as a spring.  Robot to wall,
% and robot to robot interactions are modeled by nulling the velocity
% vector of the robot in the direction of the obstacle.
%
% Modified: 
%   2/11/2014 - R. Beard
%   2/18/2014 - R. Beard
%
function [sys,x0,str,ts,simStateCompliance] = soccer_dynamics(t,x,u,flag,P)

switch flag,

  %%%%%%%%%%%%%%%%%%
  % Initialization %
  %%%%%%%%%%%%%%%%%%
  case 0,
    [sys,x0,str,ts,simStateCompliance]=mdlInitializeSizes(P);

  %%%%%%%%%%%%%%%
  % Derivatives %
  %%%%%%%%%%%%%%%
  case 1,
    sys=mdlDerivatives(t,x,u,P);

  %%%%%%%%%%
  % Update %
  %%%%%%%%%%
  case 2,
    sys=mdlUpdate(t,x,u,P);

  %%%%%%%%%%%
  % Outputs %
  %%%%%%%%%%%
  case 3,
    sys=mdlOutputs(t,x,P);

  %%%%%%%%%%%%%%%%%%%%%%%
  % GetTimeOfNextVarHit %
  %%%%%%%%%%%%%%%%%%%%%%%
  case 4,
    sys=mdlGetTimeOfNextVarHit(t,x,u);

  %%%%%%%%%%%%%
  % Terminate %
  %%%%%%%%%%%%%
  case 9,
    sys=mdlTerminate(t,x,u);

  %%%%%%%%%%%%%%%%%%%%
  % Unexpected flags %
  %%%%%%%%%%%%%%%%%%%%
  otherwise
    DAStudio.error('Simulink:blocks:unhandledFlag', num2str(flag));

end

% end sfuntmpl

%
%=============================================================================
% mdlInitializeSizes
% Return the sizes, initial conditions, and sample times for the S-function.
%=============================================================================
%
function [sys,x0,str,ts,simStateCompliance]=mdlInitializeSizes(P)

%
% call simsizes for a sizes structure, fill it in and convert it to a
% sizes array.
%
% Note that in this example, the values are hard coded.  This is not a
% recommended practice as the characteristics of the block are typically
% defined by the S-function parameters.
%
sizes = simsizes;

sizes.NumContStates  = 6*P.num_robots + 4;
sizes.NumDiscStates  = 6*P.num_robots + 4 + 2; % last two are score 
sizes.NumOutputs     = 6*P.num_robots + 2 + 2;
sizes.NumInputs      = 6*P.num_robots;
sizes.DirFeedthrough = 0;
sizes.NumSampleTimes = 1;   % at least one sample time is needed

sys = simsizes(sizes);

%
% initialize the initial conditions
%
x0  = [...
    P.home_team_initial_configurations;... % +x-axis to right, +y-axis up
    P.away_team_initial_configurations;... % +x-axis to left,  +y-axis down
    P.ball_initial_position;... % home team coordinate system
    P.ball_initial_velocity;... % home team coordinate system
    zeros(size(P.home_team_initial_configurations));
    zeros(size(P.away_team_initial_configurations));
    zeros(size(P.ball_initial_position));
    zeros(size(P.ball_initial_velocity));
    0; 0; % initial score is zero-zero
    ];

%
% str is always an empty matrix
%
str = [];

%
% initialize the array of sample times
%
ts  = [0 0];

% Specify the block simStateCompliance. The allowed values are:
%    'UnknownSimState', < The default setting; warn and assume DefaultSimState
%    'DefaultSimState', < Same sim state as a built-in block
%    'HasNoSimState',   < No sim state
%    'DisallowSimState' < Error out when saving or restoring the model sim state
simStateCompliance = 'UnknownSimState';

% end mdlInitializeSizes

%
%=============================================================================
% mdlDerivatives
% Return the derivatives for the continuous states.
%=============================================================================
%
function sys=mdlDerivatives(t,x,u, P)
        NN = 0;
    home_robot = reshape(x(1+NN:3*P.num_robots+NN),3,P.num_robots);
        NN = NN + 3*P.num_robots;
    away_robot = reshape(x(1+NN:3*P.num_robots+NN),3,P.num_robots);
        NN = NN + 3*P.num_robots;
    ball = x(1+NN:2+NN);
        NN = NN + 2;
    ball_vel = x(1+NN:2+NN);
        NN = NN + 2;
    home_robot0 = reshape(x(1+NN:3*P.num_robots+NN),3,P.num_robots);
        NN = NN + 3*P.num_robots;
    away_robot0 = reshape(x(1+NN:3*P.num_robots+NN),3,P.num_robots);
        NN = NN + 3*P.num_robots;
    ball0 = x(1+NN:2+NN);
        NN = NN + 2;
    ball_vel0 = x(1+NN:2+NN);
        NN = NN + 2;
    home_Omega = reshape(u(1:3*P.num_robots),3,P.num_robots);
        NN = 3*P.num_robots;
    away_Omega = reshape(u(1+NN:3*P.num_robots+NN),3,P.num_robots);
    
    % correct for re-set dynamics
    home_robot = home_robot-home_robot0;
    away_robot = away_robot-away_robot0;
    ball = ball-ball0;
    ball_vel = ball_vel-ball_vel0;
    
    % home robot dynamics
    for i=1:P.num_robots,
        R = [...
            cos(home_robot(3,i)), -sin(home_robot(3,i)), 0;...
            sin(home_robot(3,i)), cos(home_robot(3,i)), 0;...
            0, 0, 1];
        v = R*P.M3inv*home_Omega(:,i);
        v = robot_wall_interaction(home_robot(1:2,i),v,P);
        v = robot_robot_interaction(home_robot,i,v,away_robot,P);
        v = saturate_velocity(v,P);
        home_robotdot(:,i) = v;
    end
    
    % home robot dynamics
    for i=1:P.num_robots,
        R = [...
            cos(away_robot(3,i)), -sin(away_robot(3,i)), 0;...
            sin(away_robot(3,i)), cos(away_robot(3,i)), 0;...
            0, 0, 1];
        v = R*P.M3inv*away_Omega(:,i);
        v = robot_wall_interaction(away_robot(1:2,i),v,P);
        v = robot_robot_interaction(away_robot,i,v,home_robot,P);
        v = saturate_velocity(v,P);
        away_robotdot(:,i) = v;
    end

    
    % ball dynamics including interactions
    F_walls = force_on_ball_from_walls(ball,P);
    F_home_robots = force_on_ball_from_home_robots(ball,ball_vel,home_robot,P);
    F_away_robots = force_on_ball_from_away_robots(ball,ball_vel,away_robot,P);
    ball_max_speed = 1;
    if norm(ball_vel)>ball_max_speed,
        F_max_velocity_hack = -5*ball_vel;
    else
        F_max_velocity_hack = [0;0];
    end
    ball_accel = -P.ball_mu*ball_vel + F_walls + F_home_robots + F_away_robots + F_max_velocity_hack;

sys = [reshape(home_robotdot,3*P.num_robots,1); reshape(away_robotdot,3*P.num_robots,1); ball_vel; ball_accel];

% end mdlDerivatives

%
%=============================================================================
% mdlUpdate
% Handle discrete state updates, sample time hits, and major time step
% requirements.
%=============================================================================
%
function sys=mdlUpdate(t,x,u,P)
        NN = 0;
    home_robot = x(1+NN:3*P.num_robots+NN);
        NN = NN + 3*P.num_robots;
    away_robot = x(1+NN:3*P.num_robots+NN);
        NN = NN + 3*P.num_robots;
    ball = x(1+NN:2+NN);
        NN = NN + 2;
    ball_vel = x(1+NN:2+NN);
        NN = NN + 2;
    home_robot0 = x(1+NN:3*P.num_robots+NN);
        NN = NN + 3*P.num_robots;
    away_robot0 = x(1+NN:3*P.num_robots+NN);
        NN = NN + 3*P.num_robots;
    ball0 = x(1+NN:2+NN);
        NN = NN + 2;
    ball_vel0 = x(1+NN:2+NN);
        NN = NN + 2;
    score = x(1+NN:2+NN);
    
    % reset the game if a goal has been scored,
    if (ball(1)-ball0(1)>P.field_length/2+.05 && (ball(2)-ball0(2) < P.goal_width/2) && (ball(2)-ball0(2) > -P.goal_width/2)) % home team scored
        home_robot0 = home_robot - P.home_team_initial_configurations;
        away_robot0 = away_robot - P.away_team_initial_configurations;
        ball0 = ball - P.ball_initial_position;
        ball_vel0 = ball_vel - P.ball_initial_velocity;
        score(1)=score(1)+1;  
    elseif (ball(1)-ball0(1)<-P.field_length/2-.05 && (ball(2)-ball0(2) < P.goal_width/2) && (ball(2)-ball0(2) > -P.goal_width/2)), % away team scored
        home_robot0 = home_robot - P.home_team_initial_configurations;
        away_robot0 = away_robot - P.away_team_initial_configurations;
        ball0 = ball - P.ball_initial_position;
        ball_vel0 = ball_vel - P.ball_initial_velocity;
        score(2)=score(2)+1;
    end
    
    
sys = [home_robot0; away_robot0; ball0; ball_vel0; score];


% end mdlUpdate

%
%=============================================================================
% mdlOutputs
% Return the block outputs.
%=============================================================================
%
function sys=mdlOutputs(t,x,P)
    states = x(1:6*P.num_robots+2);
    NN = 6*P.num_robots+2+2;
    states0 = x(1+NN:6*P.num_robots+2+NN);
    NN = NN + 6*P.num_robots+4;
    score = x(1+NN:2+NN);

    % output all states except the ball velocity - corrected states
    sys = [states-states0; score];

% end mdlOutputs

%
%=============================================================================
% mdlGetTimeOfNextVarHit
% Return the time of the next hit for this block.  Note that the result is
% absolute time.  Note that this function is only used when you specify a
% variable discrete-time sample time [-2 0] in the sample time array in
% mdlInitializeSizes.
%=============================================================================
%
function sys=mdlGetTimeOfNextVarHit(t,x,u)

sampleTime = 1;    %  Example, set the next hit to be one second later.
sys = t + sampleTime;

% end mdlGetTimeOfNextVarHit

%
%=============================================================================
% mdlTerminate
% Perform any end of simulation tasks.
%=============================================================================
%
function sys=mdlTerminate(t,x,u)

sys = [];

% end mdlTerminate

%
%=============================================================================
% force_on_ball_from_walls
% Model walls with strong spring force.
%=============================================================================
%
function F_walls = force_on_ball_from_walls(ball,P)
    
    F_walls = [0;0];
    % right wall
    if ball(1)>=P.field_length/2-P.ball_radius,
        if (ball(2)>=P.goal_width/2)||(ball(2)<=-P.goal_width/2), % allow to go through goal
            n = [-1; 0];
            F_walls = F_walls + P.ball_spring*(ball(1)-P.field_length/2+P.ball_radius)*n;
        end
    end
    % left wall
    if ball(1)<=-P.field_length/2+P.ball_radius,
        if (ball(2)>=P.goal_width/2)||(ball(2)<=-P.goal_width/2), % allow to go through goal
            n = [1; 0];
            F_walls = F_walls + P.ball_spring*(-ball(1)-P.field_length/2+P.ball_radius)*n;
        end
   end
    % top wall
    if ball(2)>=P.field_width/2-P.ball_radius,
        n = [0; -1];
        F_walls = F_walls + P.ball_spring*(ball(2)-P.field_width/2+P.ball_radius)*n;
    end
    % bottom wall
    if ball(2)<=-P.field_width/2+P.ball_radius,
        n = [0; 1];
        F_walls = F_walls + P.ball_spring*(-ball(2)-P.field_width/2+P.ball_radius)*n;
    end

%
%=============================================================================
% force_on_ball_from_home_robots
% Model walls with strong spring force.
%=============================================================================
%
function F_robots = force_on_ball_from_home_robots(ball,ball_vel,robot,P)
    
    F_robots = [0;0];
    for i=1:P.num_robots,
       r = ball - robot(1:2,i);
       if norm (r)<=P.robot_radius+P.ball_radius,
            n = [cos(robot(3,i)); sin(robot(3,i))];
            if (n'*r>0)% && (n'*ball_vel<=0), % ball in front of robot and moving toward robot
                if n'*r<=P.ball_radius,
                     F_robots = F_robots + P.ball_spring*(P.ball_radius+n'*r)*n;
                end
            else %if (n'*r<=0)&& (r'*ball_vel<=0), % ball is behind robot and moving toward robot
               F_robots = F_robots + P.ball_spring*(P.ball_radius+P.robot_radius-norm(r))*r/norm(r);      
            end
       end
    end

    %
%=============================================================================
% force_on_ball_from_away_robots
% Model walls with strong spring force.
%=============================================================================
%
function F_robots = force_on_ball_from_away_robots(ball,ball_vel,robot,P)
    
    F_robots = [0;0];
    for i=1:P.num_robots,
       r = ball + robot(1:2,i);
       if norm (r)<=P.robot_radius+P.ball_radius,
            n = [cos(robot(3,i)+pi); sin(robot(3,i)+pi)];
            if (n'*r>0)% && (n'*ball_vel<0), % ball in front of robot and moving toward robot
                if n'*r<=P.ball_radius,
                     F_robots = F_robots + P.ball_spring*(P.ball_radius+n'*r)*n;
                end
            else %if (n'*r<=0)&& (r'*ball_vel<0), % ball is behind robot and moving toward robot
               F_robots = F_robots + P.ball_spring*(P.ball_radius+P.robot_radius-norm(r))*r/norm(r);      
            end
       end
     end
    
    %
%=============================================================================
% robot_wall_interaction
% robot interactions with wall
%=============================================================================
%
function v = robot_wall_interaction(robot,v,P)
    
    % right wall
    if robot(1)>=P.field_length/2-P.robot_radius,
        n = [-1; 0; 0];
        if v'*n<0,
            v = (eye(3)-n*n')*v;
        end
    end
    % left wall
    if robot(1)<=-P.field_length/2+P.robot_radius,
        n = [1; 0; 0];
        if v'*n<0,
            v = (eye(3)-n*n')*v;
        end
   end
    % top wall
    if robot(2)>=P.field_width/2-P.robot_radius,
        n = [0; -1; 0];
        if v'*n<0,
            v = (eye(3)-n*n')*v;
        end
    end
    % bottom wall
    if robot(2)<=-P.field_width/2+P.robot_radius,
        n = [0; 1; 0];
        if v'*n<0,
            v = (eye(3)-n*n')*v;
        end
    end

    
            
%=============================================================================
% robot_robot_interaction
% robot interactions with other robots
%=============================================================================
%
function v = robot_robot_interaction(robot,i,v,opponent,P)

    % interactions with own team
    for j = 1:P.num_robots,
        if j~=i,
            r = robot(1:2,j)-robot(1:2,i);
            if norm(r)<=2*P.robot_radius,
                n = [r/norm(r); 0];
                if v'*n>0,
                    v = (eye(3)-n*n')*v;
                end
            end
        end
    end
    
    % interactions with opponent
    for j = 1:P.num_robots,
        r = -opponent(1:2,j)-robot(1:2,i);
        if norm(r)<=2*P.robot_radius,
%        if norm(r)<=P.robot_radius,
            n = [r/norm(r); 0];
            if v'*n>0,
                v = (eye(3)-n*n')*v;
            end
        end
    end
    
%=============================================================================
% saturate_velocity
% 	saturate the velocity of the robot to mimic real world constraints
%=============================================================================
%
function v = saturate_velocity(v,P)
    if v(1) >  P.robot_max_vx,    v(1) =  P.robot_max_vx;    end
    if v(1) < -P.robot_max_vx,    v(1) = -P.robot_max_vx;    end
    if v(2) >  P.robot_max_vy,    v(2) =  P.robot_max_vy;    end
    if v(2) < -P.robot_max_vy,    v(2) = -P.robot_max_vy;    end
    if v(3) >  P.robot_max_omega, v(3) =  P.robot_max_omega; end
    if v(3) < -P.robot_max_omega, v(3) = -P.robot_max_omega; end

