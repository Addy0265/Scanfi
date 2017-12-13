function leap_of_faith (x_init,y_init,arena_setup)
x_init
y_init
d_initial = norm([x_init,y_init]-[800,800])
initial_lof = 100;
angle = 2*pi*rand; 
angle_in_degrees = angle*180/pi 
RSSI = arena_setup;

x_lof = x_init + floor(initial_lof*cos(angle));
y_lof = y_init + floor(initial_lof*sin(angle));
x_nor = x_init - floor(initial_lof*sin(angle));
y_nor = y_init + floor(initial_lof*cos(angle));
x_nor_opp = x_init + floor(initial_lof*sin(angle));
y_nor_opp = y_init - floor(initial_lof*cos(angle));
x_lof_opp = x_init - floor(initial_lof*cos(angle));
y_lof_opp = y_init - floor(initial_lof*sin(angle));

delta_x = RSSI(x_lof,y_lof) - RSSI(x_init,y_init) %now we move on the newly defined axis%
delta_x_opp = RSSI(x_lof_opp,y_lof_opp) - RSSI(x_init,y_init) %now we move on the newly defined axis in the opposite direction%
delta_x_nor = RSSI(x_nor,y_nor) - RSSI(x_init,y_init)
delta_x_nor_opp = RSSI(x_nor_opp,y_nor_opp) - RSSI(x_init,y_init)

a = [delta_x, delta_x_opp, delta_x_nor, delta_x_nor_opp];
b = max(a);
c = [abs(delta_x), abs(delta_x_opp), abs(delta_x_nor), abs(delta_x_nor_opp)];
d = max(c);

lambda_x = lambda_calculator(x_init,y_init,arena_setup)
% lambda_x_new_lof = lambda_calculator(x_lof,y_lof,arena_setup);%x_new is the new axis defined along the lof
% delta_lambda = -(lambda_x - lambda_x_new_lof); % to determine if we moved in the correct direction or not 

%sign_delta_x = sign(delta_x);
% sign_delta_lambda = sign(delta_lambda);


if b == delta_x
    x_next = x_init + floor(d*lambda_x*cos(angle));
    y_next = y_init + floor(d*lambda_x*sin(angle));
    z = 'x'
end
if b == delta_x_opp
    x_next = x_init - floor(d*lambda_x*cos(angle));
    y_next = y_init - floor(d*lambda_x*sin(angle));
    z = '-x'
end
if b == delta_x_nor
    x_next = x_init - floor(d*lambda_x*sin(angle));
    y_next = y_init + floor(d*lambda_x*cos(angle));
    z = 'y'
end
if b == delta_x_nor_opp
    x_next = x_init + floor(d*lambda_x*sin(angle));
    y_next = y_init - floor(d*lambda_x*cos(angle));
    z = '-y'
end 
% put in the code to turn 90 degrees if sign of delta_x == sign of delta_x_opp

d_final = norm([x_next,y_next]-[800,800])
end