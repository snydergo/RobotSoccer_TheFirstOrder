% Models vision system
% The output has the following format
%   out = [robot1; robot2; ... robotN], where
%   roboti = [ball; own_team; opponent; markers], where 
%   ball = [range; bearing]
%   own_team = [range1; bearing1; range2; bearing2; ...]
%     etc.
%   For each robot, the range and bearing to itself is 0, 0
%   When the range or bearing is outside the camera field of view, the
%   value is P.camera_out_of_range.
%
% Modified: 
%        2/11/2014 - R. Beard
%        3/5/2015 - R. Beard (fixed numerous bugs in original code)   
%        2/11/2016 - revised to overhead camera that only adds zero mean
%        Gaussian noise.
%

function vision_out=camera(uu,P)
  NN = 0;
  home = reshape(uu(1+NN:3*P.num_robots+NN),3,P.num_robots);
  NN = NN + 3*P.num_robots;
  away = reshape(uu(1+NN:3*P.num_robots+NN),3,P.num_robots);
  NN = NN + 3*P.num_robots;
  ball = uu(1+NN:2+NN);
  NN = NN + 2;
    
  % add zero mean Gaussian noise to all measurements
  home(1:2,:) = home(1:2,:)+P.camera_sigma_robot_position*randn(2,P.num_robots);
  home(3,:)   = home(3,:)+P.camera_sigma_robot_angle*randn(1,P.num_robots);
  away(1:2,:) = away(1:2,:)+P.camera_sigma_robot_position*randn(2,P.num_robots);
  away(3,:)   = away(3,:)+P.camera_sigma_robot_angle*randn(1,P.num_robots); 
  ball = ball + P.camera_sigma_ball*randn(2,1);
  
  vision_out = [...
      reshape(home,P.num_robots*3,1);...
      reshape(away,P.num_robots*3,1);...
      ball(1); ball(2);...
      ];
end

