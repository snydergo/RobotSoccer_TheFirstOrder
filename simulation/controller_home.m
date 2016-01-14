% main control code - assumes full state knowledge
%
%
% Modified: 
%   2/11/2014 - R. Beard
%   2/18/2014 - R. Beard
%   2/24/2014 - R. Beard
%   1/4/2016  - R. Beard
%

% this first function catches simulink errors and displays the line number
function v_c=controller_home(uu,P)
    try
        v_c=controller_home_(uu,P);
    catch e
        msgString = getReport(e);
        fprintf(2,'\n%s\n',msgString);
        rethrow(e);
    end
end

% main control function
function v_c=controller_home_(uu,P)

    % process inputs to function
    % robots - own team
    for i=1:P.num_robots,
        robot(:,i)   = uu(1+3*(i-1):3+3*(i-1));
    end
    NN = 3*P.num_robots;
    % robots - opponent
    for i=1:P.num_robots,
        opponent(:,i)   = uu(1+3*(i-1)+NN:3+3*(i-1)+NN);
    end
    NN = NN + 3*P.num_robots;
    % ball
    ball = [uu(1+NN); uu(2+NN)];
    NN = NN + 2;
    % score: own team is score(1), opponent is score(2)
    score = [uu(1+NN); uu(2+NN)];
    NN = NN + 2;
    % current time
    t      = uu(1+NN);

    % robot #1 positions itself behind ball and rushes the goal.
    v1 = play_rush_goal(robot(:,1), ball, P);
%     v = strategy_split_field(robot(:,1), robot(:,2), ball, P);
%     v1 = v(1:3);
%     v2 = v(4:6);
    %v1 = skill_follow_ball_on_line(robot(:,1), ball, -P.field_width/3, P);
 
    % robot #2 stays on line, following the ball, facing the goal
%     v2 = skill_follow_ball_on_line(robot(:,2), ball, -2*P.field_width/3, P);

    vb = utility_get_ball_info(ball, P);
    
    persistent position
    goallie = 1;
    offense = 2;
    regroup = 3;
    if isempty(position),
        position = goallie;
    end;
    
    switch position
        case goallie
            v2 = skill_goalie(robot(:,2), ball, -9*P.field_length/24, P);
        case offense
            v2 = play_rush_goal(robot(:,2), ball, P);
            v = strategy_split_field(robot(:,1), robot(:,2), ball, P);
            v1 = v(1:3);
            v2 = v(4:6);
        case regroup
            v2 = skill_go_to_point(robot(:,2), -P.goal, P);
    end;
    
    if (vb(2)>0 && ball(1)>-P.field_length/4),
        position = offense;
    elseif (position==offense) && (robot(1,2)<0 || vb(1)<0.2),
        position = regroup;
    elseif (position==regroup && robot(1,2)<-9*P.field_length/24)
        position = goallie;
    end;

    if (vb(2)>0 && ball(1)>-P.field_length/4)
        v2 = play_rush_goal(robot(:,2), ball, P);
    elseif robot(1,2)>=-9*P.field_length/24,
        v2 = skill_go_to_point(robot(:,2), -P.goal, P);
    else
        v2 = skill_goalie(robot(:,2), ball, -9*P.field_length/24, P);
    end;
    
    % output velocity commands to robots
    v1 = utility_saturate_velocity(v1,P);
    v2 = utility_saturate_velocity(v2,P);
    v_c = [v1; v2];
end

%-----------------------------------------
% strategy - both robots rush goals
% Robot1 stays in the top 75% of the field and Robot 2 stays in the bottom 75%
function v=strategy_split_field(robot1, robot2, ball, P)

% normal vector from ball to goal
n = P.goal-ball;
n = n/norm(n);
% compute position 10cm behind ball, but aligned with goal.
position = ball - 0.2*n;

temp = position;

if norm(position-robot1(1:2))<.21,
    v1 = skill_go_to_point(robot1, P.goal, P);
else
    if position(2) <= -P.field_width/16,
       temp(2) = -P.field_width/16;
    end
    v1 = skill_go_to_point(robot1, temp, P);
end

if norm(position-robot2(1:2))<.21,
    v2 = skill_go_to_point(robot2, P.goal, P);
else
    if position(2) >= P.field_width/16,
        position(2) = P.field_width/16;
    end
    v2 = skill_go_to_point(robot2, position, P);
end
v = [v1; v2];
end


%-----------------------------------------
% play - rush goal
%   - go to position behind ball
%   - if ball is between robot and goal, go to goal
% NOTE:  This is a play because it is built on skills, and not control
% commands.  Skills are built on control commands.  A strategy would employ
% plays at a lower level.  For example, switching between offense and
% defense would be a strategy.
function v = play_rush_goal(robot, ball, P)
  
  % normal vector from ball to goal
  n = P.goal-ball;
  n = n/norm(n);
  % compute position 10cm behind ball, but aligned with goal.
  position = ball - 0.2*n;
    
  if norm(position-robot(1:2))<.21,
      v = skill_go_to_point(robot, P.goal, P);
  else
      v = skill_go_to_point(robot, position, P);
  end

end

%-----------------------------------------
% skill - follow ball on line
%   follows the y-position of the ball, while maintaining x-position at
%   x_pos.  Angle always faces the goal.

function v=skill_follow_ball_on_line(robot, ball, x_pos, P)

    % control x position to stay on current line
    vx = -P.control_k_vx*(robot(1)-x_pos);
    
    % control y position to match the ball's y-position
    vy = -P.control_k_vy*(robot(2)-ball(2));

    % control angle to -pi/2
    theta_d = atan2(P.goal(2)-robot(2), P.goal(1)-robot(1));
    omega = -P.control_k_phi*(robot(3) - theta_d); 
    
    v = [vx; vy; omega];
end


function v=skill_goalie(robot, ball, x_pos, P)

    % control x position to stay on current line
    % unless ball gets behind you]
    if(ball(1)<robot(1)+0.2),
        vx = -P.control_k_vx*(robot(1)-ball(1)+0.1);
    else
        vx = -P.control_k_vx*(robot(1)-x_pos);
    end
    
    
    
    % control y position to match the ball's y-position
    if (abs(ball(2))<P.field_width/3),
        vy = -P.control_k_vy*(robot(2)-ball(2));
    else
        vy = -P.control_k_vy*(robot(2)-sign(ball(2))*P.field_width/3);
    end
    

    % control angle to -pi/2
    theta_d = atan2(ball(2)-robot(2), ball(1)-robot(1));
    
    omega = -P.control_k_phi*(robot(3) - theta_d);
    if robot(3) > 3*pi/8 && omega > 0,
        omega = 0;
    elseif robot(3) < -3*pi/8 && omega < 0,
         omega = 0;
    end;
    
    if abs(ball(2))<P.field_width/4,
        omega =  -P.control_k_phi*(robot(3));
    end;
    
    v = [vx; vy; omega];
end


%-----------------------------------------
% skill - go to point
%   follows the y-position of the ball, while maintaining x-position at
%   x_pos.  Angle always faces the goal.

function v=skill_go_to_point(robot, point, P)

    % control x position to stay on current line
    vx = -P.control_k_vx*(robot(1)-point(1));
    
    % control y position to match the ball's y-position
    vy = -P.control_k_vy*(robot(2)-point(2));

    % control angle to -pi/2
    theta_d = atan2(P.goal(2)-robot(2), P.goal(1)-robot(1));
    omega = -P.control_k_phi*(robot(3) - theta_d); 
    
    v = [vx; vy; omega];
end


function v = utility_get_ball_info(ball, P)
    % Persisitent Variables
    persistent first_run;
    persistent position_x_prev;
    persistent position_y_prev;
    persistent velocity_x;
    persistent velocity_y;
    persistent magnitude_prev;
    persistent direction_prev;
    
    %parsing ball
    position_x = ball(1);
    position_y = ball(2);
    
    %initialize variables
    if isempty(first_run),
        position_x_prev = position_x;
        position_y_prev = position_y;
        velocity_x = 0;
        velocity_y = 0;
        magnitude_prev = 0;
        direction_prev = 0;
        first_run = 0;
    end
    
    % Estiamate the x and y velocity of the ball
    tau = 1/(30*2*pi);
    velocity_x = (2*tau -P.control_sample_rate)/...
        (2*tau+P.control_sample_rate)*velocity_x +...
        2/(2*tau+P.control_sample_rate)*(position_x - position_x_prev);
    velocity_y = (2*tau -P.control_sample_rate)/...
        (2*tau+P.control_sample_rate)*velocity_y +...
        2/(2*tau+P.control_sample_rate)*(position_y - position_y_prev);
    
    % Calculate Velocity
    magnitude = norm([velocity_x;velocity_y]);
    
    % Calculate Direction
    direction = atan2(velocity_y,velocity_x);
    
    % validate the estimated data
    estimated_magnitude = -P.ball_mu*magnitude_prev*...
        P.control_sample_rate + magnitude_prev;
    magdiff = abs(magnitude - estimated_magnitude);
    dirdiff = abs(direction - direction_prev);
        
    information_valid = 1;
    
    if dirdiff > .01 * abs(direction_prev) || ...
            magdiff > .01 * abs(magnitude_prev),
        information_valid = 0;
    end
    
    % Save variables
    position_x_prev = position_x;
    position_y_prev = position_y;
    magnitude_prev = magnitude;
    direction_prev = direction;
    
    % Set output
    v(1) = magnitude;
    v(2) = direction;
    v(3) = information_valid;
end


%------------------------------------------
% utility - saturate_velocity
% 	saturate the commanded velocity 
%
function v = utility_saturate_velocity(v,P)
    if v(1) >  P.robot_max_vx,    v(1) =  P.robot_max_vx;    end
    if v(1) < -P.robot_max_vx,    v(1) = -P.robot_max_vx;    end
    if v(2) >  P.robot_max_vy,    v(2) =  P.robot_max_vy;    end
    if v(2) < -P.robot_max_vy,    v(2) = -P.robot_max_vy;    end
    if v(3) >  P.robot_max_omega, v(3) =  P.robot_max_omega; end
    if v(3) < -P.robot_max_omega, v(3) = -P.robot_max_omega; end
end


  