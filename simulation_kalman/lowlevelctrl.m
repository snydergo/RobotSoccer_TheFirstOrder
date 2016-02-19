% converts world frame velocity commands [vx; vy; omega] to wheel speed
% commands [Omega1; Omega2; Omega3]
%
% Modified: 2/11/2014 - R. Beard
%
function Omega=lowlevelctrl(uu,P)
    % world frame velocity commands [vx, vy, omega]
    v_c = reshape(uu(1:3*P.num_robots),3,P.num_robots);  
    % robot positions;
    robot = reshape(uu(3*P.num_robots+1:end),3,P.num_robots); 
    
    for i=1:P.num_robots,
        R = [...
            cos(robot(3,i)), sin(robot(3,i)), 0;...
            -sin(robot(3,i)), cos(robot(3,i)), 0;...
            0, 0, 1];
        Omega(:,i) = P.M3*R*v_c(:,i);
    end
    
    Omega = reshape(Omega,3*P.num_robots,1);
end
  

  