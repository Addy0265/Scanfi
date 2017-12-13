function arena_simulator_new_scaled(tp,np) % setup of the arena %%%%%%%%%%%%%             
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% instructions to use : in the command prompt, define the variables  %
% resolution = 100; np = 2.5; transmit_power = .1                    %
% call the function 'arena_simulator_new(tp, np)'                    %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
arena_new = zeros(160);
trans_x = 60;
trans_y = 60;
check = 0;

while check ~= 1
    x_init = unidrnd(120);
    y_init = unidrnd(120);
    e_d =  norm ([x_init,y_init] - [trans_x,trans_y]); % 1 unit = 1cm %
    if e_d < 60 && e_d > 20
        check = 1;
    else  
        check = 0;
    end 
end 
padding_constant = 20;

x_init = x_init + padding_constant;
y_init = y_init + padding_constant;
trans_x = trans_x + padding_constant;
trans_y = trans_y + padding_constant;

euclidian_dist = zeros(160);
noise = floor(randn (160));
for i = 1 : 1 : 160
    for j = 1 : 1 : 160
            euclidian_dist(j,i) = norm ([j,i]-[trans_x,trans_y]); % 1 unit = 10cm %
            arena_new (j,i) = rssi_model_new(euclidian_dist(j,i)/10, tp, np);% rssi is being calculated by taking distance in meters
    end 
end

 arena_new (trans_x,trans_y) = arena_new(trans_x - 1,trans_y);
 arena_new = arena_new + noise;
 arena_new = arena_new - 47 ;

%figure;
surf((arena_new)');
title ('RSSI Surface Plot On The X-Y Axes');
xlabel('X - Distance (1 unit = 10 cm)');
ylabel('Y - Distance (1 unit = 10 cm)');
zlabel('RSSI');
% figure;
% surf(euclidian_dist);
%path_planning(trans_x,trans_y,arena_new,x_init,y_init)

%leap_of_faith (x_init,y_init,arena_new)
end