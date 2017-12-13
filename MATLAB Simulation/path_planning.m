function path_planning (trans_x,trans_y,arena_rssi_data,x_init,y_init)
%for initialising, keep angle = 0, x_init = 0, y_init = 0, d_next = 0, count = 1%

Pr = (arena_rssi_data);
x_start = x_init
y_start = y_init
exp = 3.5; 
step = 50;
scale = 100;

coordinates = zeros(5,6);
coordinates(1,1) = x_start;
coordinates(1,2) = y_start;
coordinates(1,4) = trans_x;
coordinates(1,5) = trans_y;
coordinates(1,6) = norm([trans_x,trans_y]-[x_start,y_start]);

angle = pi/2*rand; 
initial_angle_in_degrees = angle*180/pi

coordinates(1,3) = initial_angle_in_degrees;
x_lof_1 = floor(x_start + step*cos(angle)) %initial leap of faith is of 50 cm
y_lof_1 = floor(y_start + step*sin(angle))
%perpendicular leap of faith%
x_lof_2 = floor(x_start - step*sin(angle))
y_lof_2 = floor(y_start + step*cos(angle))

delta_x = scale*(Pr(x_lof_1 ,y_lof_1 ) - Pr(x_start ,y_start ))
delta_y = scale*(Pr(x_lof_2 ,y_lof_2 ) - Pr(x_start ,y_start ))

lambda_x = 10^(-exp*(Pr(x_start,y_start)-rssi_model(1,10,2.5))/(2.5*10))
lambda_y = 10^(-exp*(Pr(x_start,y_start)-rssi_model(1,10,2.5))/(2.5*10))

x_next_ = floor(x_start + delta_x*lambda_x)
y_next_ = floor(y_start + delta_y*lambda_y)

coordinates(2,1) = x_next_;
coordinates(2,2) = y_next_;
coordinates(2,4) = trans_x;
coordinates(2,5) = trans_y;
coordinates(2,6) = norm([trans_x,trans_y]-[x_next_,y_next_]);


for s = 1 : 1 : 50
    x_start = x_next_;
    y_start = y_next_;
    %first leap of faith%
    angle = 2*pi*rand;
    angle_in_degrees = angle*180/pi; 
    x_lof_1 = floor(x_start + step*cos(angle)); %initial leap of faith is of 40 cm
    y_lof_1 = floor(y_start + step*sin(angle));
        
    %perpendicular leap of faith%
    x_lof_2 = floor(x_start - step*sin(angle));
    y_lof_2 = floor(y_start + step*cos(angle));

delta_x = scale*(Pr(x_lof_1 ,y_lof_1 ) - Pr(x_start ,y_start ))
delta_y = scale*(Pr(x_lof_2 ,y_lof_2 ) - Pr(x_start ,y_start ))

lambda_x = 10^(-exp*(Pr(x_start,y_start)-rssi_model(1,10,2.5))/(2.5*10));
lambda_y = 10^(-exp*(Pr(x_start,y_start)-rssi_model(1,10,2.5))/(2.5*10));

 x_next_ = floor(x_start + 20*delta_x*lambda_x);
 y_next_ = floor(y_start + 20*delta_y*lambda_y);
 coordinates(s+2,1) = x_next_;
 coordinates(s+2,2) = y_next_;
 coordinates(s+2,3) = angle_in_degrees;
 coordinates(s+2,4) = trans_x;
 coordinates(s+2,5) = trans_y;
 coordinates(s+2,6) = norm([trans_x,trans_y]-[x_next_,y_next_]);

end
disp(coordinates);
plot(coordinates);

end