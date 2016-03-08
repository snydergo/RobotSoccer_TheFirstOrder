%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%  FirstOrder  %%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function v=skill_goalie(robot, ball, x_pos, P)

    % control x position to stay on current line
    % unless ball gets behind you]
    if(ball(1)<robot(1)),
        vx = -P.control_k_vx*(robot(1)-ball(1));
    else
        vx = -P.control_k_vx*(robot(1)-x_pos);
    end
    
    
    
    % control y position to match the ball's y-position
    if (abs(ball(2))<P.field_width/4),
        vy = -P.control_k_vy*(robot(2)-ball(2));
    else
        vy = 0;
    end
    

    % control angle to -pi/2
    theta_d = atan2(ball(2)-robot(2), ball(1)-robot(1));
    
    omega = -P.control_k_phi*(robot(3) - theta_d); 
    
    v = [vx; vy; omega];
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%% Vektor Krum  %%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function v_c = strategy_switch_offense_and_defense(robot, opponent, ball, P, t)
    persistent player_roles;
    persistent robot1_loc;
    persistent robot2_loc;
    
    % player role states
    normal = 0;
    reversed = 1;

    %initialize persistent variables
    if(t == 0)
        player_roles = normal;
        robot1_loc = robot(:,1);
        robot2_loc = robot(:,2);
    end
    
    % initialize player roles
    if(player_roles == normal)
        attacker = robot(:,1);
        defender = robot(:,2);
    else
        defender = robot(:,1);
        attacker = robot(:,2);
    end
    
    % Every second, compare robots location to its location 1 second ago
    if(mod(t, 1) == 0.0)
        old1_loc = robot1_loc;
        old2_loc = robot2_loc;
        robot1_loc = robot(:,1);
        robot2_loc = robot(:,2);
        dist1 = utility_calc_distance(old1_loc, robot1_loc);
        dist2 = utility_calc_distance(old2_loc, robot2_loc);
        
        % If neither robot has moved much in a while then switch roles
        if(dist1 < 0.15 && dist2 < 0.15)
            if(player_roles == normal)
                player_roles = reversed;
                defender = robot(:,1);
                attacker = robot(:,2);
            else
                player_roles = normal;
                attacker = robot(:,1);
                defender = robot(:,2);
            end
        end
    end
    
    
    % if the ball gets behind the attacker then switch roles
    if(ball(1) < (attacker(1)-((P.robot_radius)/2)))
        if(player_roles == normal)
            player_roles = reversed;
            defender = robot(:,1);
            attacker = robot(:,2);
        else
            player_roles = normal;
            attacker = robot(:,1);
            defender = robot(:,2);
        end
    end
    
    defense = 0;
    offense = 1;
    playtype = offense;
    
    % If the ball gets behind 3/4 field then go on defense
    if(ball(1) < (P.field_width/8)) %P.field_width/8
        playtype = defense;
    end
    
    % play chosen based on role state and if we are on offense or defense
    if(player_roles == normal)
        if(playtype == offense)
            v1 = play_rush_goal(attacker, ball, P);
            %v1 = play_rush_goal_avoid_obstacles(attacker, defender, opponent, ball, P);
            v2 = skill_follow_ball_on_line(defender, ball, 0, P);
        else
            v1 = play_rush_goal(attacker, ball, P);
            %v1 = play_rush_goal_avoid_obstacles(attacker, defender, opponent, ball, P);
            v2 = skill_guard_goal(defender, ball, P);
            %v2 = play_rush_goal(defender, ball, P);
        end
    else
        if(playtype == offense)
            v2 = play_rush_goal(attacker, ball, P);
            %v2 = play_rush_goal_avoid_obstacles(attacker, defender, opponent, ball, P);
            v1 = skill_follow_ball_on_line(defender, ball, 0, P);
        else
            v2 = play_rush_goal(attacker, ball, P);
            %v2 = play_rush_goal_avoid_obstacles(attacker, defender, opponent, ball, P);
            v1 = skill_guard_goal(defender, ball, P);
            %v1 = play_rush_goal(defender, ball, P);
        end
    end
    
    % output velocity commands to robots
    v1 = utility_saturate_velocity(v1,P);
    v2 = utility_saturate_velocity(v2,P);
    v_c = [v1; v2];
end

function v_c = strategy_strong_offense(robot, opponent, ball, P, t)
    defense = 0;
    offense = 1;
    playtype = offense;
    
    % If the ball gets behind 3/4 field then go on defense
    if(ball(1) < (3*P.field_length/8)) %P.field_width/8
        playtype = defense;
    end
    
    v1 = play_rush_goal(robot(:,1), ball, P);
    
    if(playtype == offense)
        v2 = skill_follow_ball_on_line(robot(:,2), ball, P.field_length/4, P);
    else
        v2 = play_rush_goal(robot(:,2), ball, P);
    end
    
    v1 = utility_saturate_velocity(v1,P);
    v2 = utility_saturate_velocity(v2,P);
    v_c = [v1; v2];
end

%%%%%%%%%%%%%%%%%%%%%%%%% Plays %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function v = play_rush_goal_avoid_obstacles(robot, defender, opponent, ball, P)
  
  % normal vector from ball to goal
  n = P.goal-ball;
  n = n/norm(n);
  % compute position 10cm behind ball, but aligned with goal.
  position = ball - 0.2*n;
    
  obstacle_info = utility_detect_obstacle(robot, defender, opponent, ball, P)
    
  if norm(position-robot(1:2))<.21,
      v = skill_go_to_point_avoid_obstacles(ball, robot, position, P, obstacle_info);
  else
      v = skill_go_to_point(robot, position, P);
  end

end

%%%%%%%%%%%%%%%%%%%%%%% Skills %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%------------------------------------------
% skill - follow ball on line in front of goal, never leaving goal
%   follows the y-position of the ball, while maintaining x position in
%   front of the goal. Angle always faces the ball. Does not leave the
%   area of the goal.

function v=skill_guard_goal(robot, ball, P)
    % control x position to stay on -15/16 the field length
    vx = -P.control_k_vx*(robot(1)-(-15*P.field_width/16));
    
    % control y position to match the ball's y-position while ball is
    % within the goal. Otherwise, stay at edges.
    if ball(2) > P.field_width/6
        vy = -P.control_k_vy*(robot(2)-(P.field_width/6));
    elseif ball(2) < -P.field_width/6
        vy = -P.control_k_vy*(robot(2)-(-P.field_width/6));
    else
        vy = -P.control_k_vy*(robot(2)-ball(2));
    end

    % control angle to face ball, but not exceeding +/- 90 degrees.
    theta_d = atan2(ball(2)-robot(2), ball(1)-robot(1));
    if theta_d >= pi/2
       theta_d = pi/2.25;  % angle pushes ball out (more effective)
    elseif theta_d <= -pi/2
        theta_d = -pi/2.25;
    end
    omega = -P.control_k_phi*(robot(3) - theta_d); 
    v = [vx; vy; omega];
end

function v=skill_go_to_front_of_ball_and_face_ball(robot, ball, ballvel, P)
    persistent first_run;
    persistent mag;
    persistent dir;
    
    if isempty(first_run)
        mag = 0;
        dir = 0;
        first_run = 0;
    end
    
    if ballvel(3) == 1
        mag = ballvel(1);
        dir = ballvel(2);
    end
    
    point_in_front_of_ball = (P.robot_radius+P.ball_radius)*...
        [cos(dir);sin(dir)] + ball;
    angle_face_ball = dir + pi/4;
    
    v = skill_go_to_point_and_angle(robot, point_in_front_of_ball,...
        angle_face_ball, P);
end
%-----------------------------------------
% skill - go to point and angle
%   follows the y-position of the ball, while maintaining x-position at
%   x_pos.  Angle always faces the goal.

function v=skill_go_to_point_and_angle(robot, point, angle, P)

    % control x position to stay on current line
    vx = -P.control_k_vx*(robot(1)-point(1));
    
    % control y position to match the ball's y-position
    vy = -P.control_k_vy*(robot(2)-point(2));

    % control angle to -pi/2
    theta_d = angle;
    omega = -P.control_k_phi*(robot(3) - theta_d); 
    
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

function v=skill_go_to_point_angle_corrected(ball, robot, point, P)

    % control x position to stay on current line
    vx = -P.control_k_vx*(robot(1)-point(1));
    
    % control y position to match the ball's y-position
    vy = -P.control_k_vy*(robot(2)-point(2));
    
    y_delta = robot(2) - ball(2);

    % control angle to -pi/2
    theta_d = atan2((P.goal(2)-(y_delta*60))-ball(2), P.goal(1)-ball(1));
    omega = -P.control_k_phi*(robot(3) - theta_d); 
    
    v = [vx; vy; omega];
end

function v=skill_go_to_point_avoid_obstacles(ball, robot, point, P, obstacle_info)
%     % control x position to stay on current line
%     vx = -P.control_k_vx*(robot(1)-point(1));
%     
%     % control y position to match the ball's y-position
%     vy = -P.control_k_vy*(robot(2)-point(2));
% 
%     % control angle to -pi/2
%     theta_d = atan2(P.goal(2)-robot(2), P.goal(1)-robot(1));
%     omega = -P.control_k_phi*(robot(3) - theta_d); 

    left_obstacle = obstacle_info(1);
    right_obstacle = obstacle_info(4);
    if(left_obstacle == 1)
        % intermediate is a point to the side of the obstacle
        intermediate = [obstacle_info(2);obstacle_info(3)];
        v = skill_go_to_point(robot, intermediate, P);
    elseif(right_obstacle == 1)
        % intermediate is a point to the side of the obstacle
        intermediate = [obstacle_info(5);obstacle_info(6)];
        v = skill_go_to_point(robot, intermediate, P);
    else
        v = skill_go_to_point_angle_corrected(ball, robot, point, P)
    end
end

%%%%%%%%%%%%%%%%%%% Utilities %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function result = utility_detect_obstacle(attacker, defender, opponents, ball, P)
    % Set up names for all players
    me = attacker(1:2);
    teammate = defender(1:2);
    enemy1 = opponents(1:2,1);
    enemy2 = opponents(1:2,2);
    
    number_of_samples = 10;
    
    % find angle of slope perpendicular to slope of vector to the ball
    vector = ball-me;
    slope = vector(2)/vector(1);
    perp_slope = (-1/slope);
    angle = tan(perp_slope);
    
    % find coordinates of robot face corners when facing the ball
    me_left_x = me(1) + (P.robot_radius*cos(angle));
    me_left_y = me(2) - (P.robot_radius*sin(angle));
    me_right_x = me(1) - (P.robot_radius*cos(angle));
    me_right_y = me(2) + (P.robot_radius*sin(angle));
    
    ball_left_x = ball(1) + (P.robot_radius*cos(angle));
    ball_left_y = ball(2) - (P.robot_radius*sin(angle));
    ball_right_x = ball(1) - (P.robot_radius*cos(angle));
    ball_right_y = ball(2) + (P.robot_radius*sin(angle));
    
    % find points along line followed by robot corners if it went straight
    % to the ball
    left_line_x = linspace(me_left_x, ball_left_x, number_of_samples);
    left_line_y = linspace(me_left_y, ball_left_y, number_of_samples);
    right_line_x = linspace(me_right_x, ball_right_x, number_of_samples);
    right_line_y = linspace(me_right_y, ball_right_y, number_of_samples);
    
    left_line = [left_line_x;left_line_y];
    right_line = [right_line_x;right_line_y];
    
    % state variables: obstacle is 1 if detected and 0 otherwise, sample
    % num is the number of the sample that finds obstacle
    left_obstacle = 0;
    right_obstacle = 0;
    left_sample_num = 1;
    right_sample_num = 1;
    
    for i = 1:number_of_samples
        if(utility_calc_distance(left_line(:,i), teammate) < P.robot_radius)
            left_obstacle = 1;
            left_sample_num = i;
            break;
        elseif(utility_calc_distance(left_line(:,i), enemy1) < P.robot_radius)
            left_obstacle = 1;
            left_sample_num = i;
            break;
        elseif(utility_calc_distance(left_line(:,i), enemy2) < P.robot_radius)
            left_obstacle = 1;
            left_sample_num = i;
            break;
        end
    end
    
    for i = 1:number_of_samples
        if(utility_calc_distance(right_line(:,i), teammate) < P.robot_radius)
            right_obstacle = 1;
            right_sample_num = i;
            break;
        elseif(utility_calc_distance(right_line(:,i), enemy1) < P.robot_radius)
            right_obstacle = 1;
            right_sample_num = i;
            break;
        elseif(utility_calc_distance(right_line(:,i), enemy2) < P.robot_radius)
            right_obstacle = 1;
            right_sample_num = i;
            break;
        end
    end
    
    % the first item in each row (left_obstacle) says whether there was an
    % obstacle detected (value=1) if so, the next two numbers are valid
    % coordinates of the point on the opposite line next to the point that
    % detected a robot
    
    result = [left_obstacle, right_line_x(left_sample_num), right_line_y(left_sample_num); right_obstacle, left_line_x(right_sample_num), left_line_y(right_sample_num)];
end

function distance = utility_calc_distance(point1, point2)
    distance = sqrt((point1(1) - point2(1))^2 + (point1(2) - point2(2))^2);
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







%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%% REAL TUCANOS %%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


if ball(1) < 0,         % activate defense
    temp = strategy_sweeper_keeper(robot(:,1), robot(:,2), ball, P);
    v1 = temp(1:3);
    v2 = temp(4:6);
else                    %activate offense
    temp = strategy_defensive_midfielder(robot(:,1), robot(:,2), ball, P);
    v1 = temp(1:3);
    v2 = temp(4:6);
end

    % output velocity commands to robots
    v1 = utility_saturate_velocity(v1,P);
    v2 = utility_saturate_velocity(v2,P);
    v_c = [v1; v2];
end

%---------------------------------------------------------------------------
% STRATEGIES!!!!!!!
%---------------------------------------------------------------------------

%-----------------------------------------
% strategy - one goalee and one sweeper
% Robot1 tries to block the ball at the goal, Robot2 plays sweeper in the
% entire defensive half
function v=strategy_sweeper_keeper(robot1, robot2, ball, P)

% normal vector from ball to goal
n = P.goal-ball;
n = n/norm(n);
% compute position 10cm behind ball, but aligned with goal.
position = ball - 0.2*n;

v1 = play_get_behind_ball(robot1, ball, P);

if ball(1) > -P.field_length/6,
    v2 = skill_follow_ball_on_line(robot2, ball, -P.field_length/4, P);
else
    v2 = play_get_behind_ball(robot2, ball, P);
end

v = [v1; v2];
end

%-----------------------------------------
% strategy - one striker and one defensive midfielder
% Robot1 tries to score, Robot2 stays to block the counter attack
function v=strategy_defensive_midfielder(robot1, robot2, ball, P)

% normal vector from ball to goal
n = P.goal-ball;
n = n/norm(n);
% compute position 10cm behind ball, but aligned with goal.
position = ball - 0.2*n;

v1 = play_get_behind_ball(robot1, ball, P);

if ball(1) > P.field_length/4,
    v2 = skill_follow_ball_on_line(robot2, ball, P.field_length/6, P);
else
    v2 = play_get_behind_ball(robot2, ball, P);
end

v = [v1; v2];
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
    if position(2) <= -P.field_width/4,
       temp(2) = -P.field_width/4;
    end
    v1 = skill_go_to_point(robot1, temp, P);
end

if norm(position-robot2(1:2))<.21,
    v2 = skill_go_to_point(robot2, P.goal, P);
else
    if position(2) >= P.field_width/4,
        position(2) = P.field_width/4;
    end
    v2 = skill_go_to_point(robot2, position, P);
end
v = [v1; v2];
end


%---------------------------------------------------------------------------
% PLAYS!!!!!!!
%---------------------------------------------------------------------------


function v=play_get_behind_ball(robot, ball, P)
  % normal vector from ball to goal
  n = P.goal-ball;
  n = n/norm(n);
  % compute position 10cm behind ball, but aligned with goal.
  position = ball - 0.2*n;
  
  if ball(1) > robot(1)
      v = play_rush_goal(robot, ball, P);
  else
      if ball(2) >= robot(2)
          position(1) = position(1)-0.1;
          position(2) = position(2)-0.1;
          v = skill_go_to_point(robot, position, P);
      else
          position(1) = position(1)-0.1;          
          position(2) = position(2)+0.1;
          v = skill_go_to_point(robot, position, P);
      end
  end
  
end

%-----------------------------------------
% play - face each other
function v=play_face_each_other(robot1, robot2, P)
    
    v1 = play_face_teammate(robot1, robot2, P);
    v2 = play_face_teammate(robot2, robot1, P);
    
    v = [v1; v2];
end


%-----------------------------------------
% play - face teammate
% robot1 rotates toward robot2
function v=play_face_teammate(robot1, robot2, P)
    
    vx = 0;
    vy = 0;

    % control angle to -pi/2
    theta_d = atan2(robot2(2)-robot1(2), robot2(1)-robot1(1));
    omega = -P.control_k_phi*(robot1(3) - theta_d); 
    
    v = [vx; vy; omega];
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



%---------------------------------------------------------------------------
% SKILLS !!!!!!!
%---------------------------------------------------------------------------


%-----------------------------------------
% skill - spin counter clockwise
% speed is how fast it spins. range between 0.01 - 1
function v=skill_spin_ccw(robot, speed, P)

    vx = 0;
    vy = 0;

    % control angle to -pi/2
    theta_d = robot(3)+speed*180/pi;
    omega = -P.control_k_phi*(robot(3) - theta_d); 
    
    v = [vx; vy; omega];
end

%-----------------------------------------
% skill - spin clockwise
% speed is how fast it spins. range between 0.01 - 1
function v=skill_spin_cw(robot, speed, P)

    vx = 0;
    vy = 0;

    % control angle to -pi/2
    theta_d = robot(3)-speed*180/pi;
    omega = -P.control_k_phi*(robot(3) - theta_d); 
    
    v = [vx; vy; omega];
end


%-----------------------------------------
% skill - follow ball on line
%   follows the y-position of the ball, while maintaining x-position at
%   x_pos.  Angle always faces the goal.

function v=skill_follow_ball_on_line(robot, ball, x_pos, P)
   
persistent ball_previous;    
        
    % control x position to stay on current line
    vx = -P.control_k_vx*(robot(1)-x_pos);
    
    if ball_previous - ball(2) > 0, % ball is moving down
        % control y position to match the ball's y-position
        vy = -P.control_k_vy*(robot(2)-ball(2))-0.3;
    else                            % ball is moving up
        vy = -P.control_k_vy*(robot(2)-ball(2))+0.3;
    end

    % control angle to -pi/2
    theta_d = atan2(P.goal(2)-robot(2), P.goal(1)-robot(1));
    omega = -P.control_k_phi*(robot(3) - theta_d); 
    
    ball_previous = ball(2);
   
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


  
