% utility - kalman filter for own team
%
function robot = utility_kalman_filter_robot(robot,v_command,t,P)
    persistent xhat
    persistent S
    for n=1:P.num_robots,    
        if t==0,  % initialize filter
            xhat(:,n) = [...
                0;... % initial guess at x-position of ownteam i
                0;... % initial guess at y-position of ownteam i
                0;... % initial guess at angle of ownteam i
                ];
        S(:,:,n) = diag([...
            P.field_width/2;... % initial variance of x-position of ownteam i
            P.field_width/2;... % initial variance of y-position of ownteam i 
            (5*pi/180)^2;... % initial variance of angle of ownteam i 
            ]);
        end
    
        % prediction step between measurements
        N = 10;
        for i=1:N,
            f = v_command(:,n);
            xhat(:,n) = xhat(:,n) + (P.control_sample_rate/N)*f;
            S(:,:,n) = S(:,:,n) + (P.control_sample_rate/N)*(P.Q_ownteam);
        end
 
        % correction step at measurement
        if robot(n).camera_flag,  % only update when the camera flag is one indicating a new measurement
            y_pred = xhat(:,n);  % predicted measurement
            L = S(:,:,n)/(P.R_ownteam+S(:,:,n));
            S(:,:,n) = (eye(3)-L)*S(:,:,n);
            xhat(:,n) = xhat(:,n) + L*( [robot(n).position_camera;robot(n).angle_camera]-y_pred);
        end
    
        % output current estimate of state
        robot(n).position = xhat(1:2,n);
        robot(n).angle    = xhat(3,n);
        robot(n).S        = S(:,:,n);
    end    
end