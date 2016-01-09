% Parameter file for robot soccer simulator
%
% Modified: 
%   2/11/2014 - R. Beard
%   1/8/2015 - R. Beard
%

% number of robots per team
P.num_robots = 2;

% field characteristics
P.field_length  = 3.048; % meters (5 ft)
P.field_width   = 1.52; % meters (10 ft)
P.field_color = [16,92,1]/256;
P.home_team_color = 'b';
P.away_team_color = 'g';
P.ball_color = 'y';
P.goal_width = P.field_width/3;
P.goal_color = [252 148 3]/256;
P.display_rate = 0.1;

% constants that govern ball dynamics
P.ball_radius = 0.03;
P.ball_mu = 0.1;  % coefficient of friction for ball
P.ball_spring = 500;% spring constant that models wall and robot interactions

% robot parameters and geometry
P.wheel_radius = 0.03; % m (this is a guess)
P.robot_radius = 0.1;  % 4 inches
P.robot_max_vx = .8; % (m/s) max speed in x direction
P.robot_max_vy = .5; % (m/s) max speed in y direction
P.robot_max_omega = 2*pi; % 360 degrees/sec
% the geometry assumes 60 degree equally distributed wheels at distance 5cm
% from center
phi = 60*pi/180;
r1 = P.robot_radius*[cos(phi); sin(phi)];
r2 = P.robot_radius*[-cos(phi); sin(phi)];
r3 = P.robot_radius*[0;1];
s1 = [-sin(phi); cos(phi)];
s2 = [-sin(phi); -cos(phi)];
s3 = [1; 0];
% kinematic matrix relating wheel velocity to body velociy
P.M3 = 1/P.wheel_radius*[...
    s1(1), s1(2), (s1(2)*r1(1)-s1(1)*r1(2));...
    s2(1), s2(2), (s2(2)*r2(1)-s2(1)*r2(2));...
    s3(1), s3(2), (s3(2)*r3(1)-s3(1)*r3(2))];
P.M3inv = inv(P.M3);

% initial states for robots and ball
P.home_team_initial_configurations = [...
    -P.field_length/6; 0;  0;... % robot 1, position and orientation
    -P.field_length/3; 0;  0;...  % robot 2, position and orientation
    ];
P.away_team_initial_configurations = [...
    -P.field_length/6; 0; 0;... % robot 1, position and orientation
    -P.field_length/3; 0; 0;...   % robot 2, position and orientation
    ];
P.ball_initial_position = [ 0; 0];     % in frame of home team
P.ball_initial_velocity = [0.3*randn; 0.5*randn];  % in frame of home team

% goal positions
P.goal = [P.field_length/2; 0];

% controller gains
P.control_sample_rate = 0.01;
P.control_k_vx  = 5;  % gain for proportional control of x-position
P.control_k_vy  = 5;  % gain for proportional control of y-position
P.control_k_phi = 2;  % gain for proportional angle control


% camera parameters
P.camera_sample_rate = 10*P.control_sample_rate;
% noise levels on the camera
P.camera_sigma_ball = 0.01; % units are meters
P.camera_sigma_robot_position = 0.01; % units are meters
P.camera_sigma_robot_angle = 1*pi/180; % units are radians


% parameters for the Kalman filter
P.Q_ownteam = diag([1^2,1^2,(2*pi/180)^2]);
P.R_ownteam = diag([P.camera_sigma_robot_position^2; P.camera_sigma_robot_position^2; P.camera_sigma_robot_angle^2]);
P.Q_opponent = diag([1^2,1^2,(2*pi/180)^2]);
P.R_opponent = diag([P.camera_sigma_robot_position^2; P.camera_sigma_robot_position^2; P.camera_sigma_robot_angle^2]);
P.Q_ball = diag([10^2,10^2,.1^2]);
P.R_ball = diag([P.camera_sigma_ball^2; P.camera_sigma_ball^2]);
  
 

    

