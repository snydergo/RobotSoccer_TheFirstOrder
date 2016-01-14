% convert coordinate frame of opponent to own frame
%
% Modified: 
%   2/18/2014 - R. Beard
%
function opponent = robot_change_frames(opponent,P)
    for i=1:P.num_robots,
        opponent(1+(i-1)*3)=-opponent(1+(i-1)*3);
        opponent(2+(i-1)*3)=-opponent(2+(i-1)*3);
        opponent(3+(i-1)*3)= opponent(3+(i-1)*3)+pi;
    end
end
  

  