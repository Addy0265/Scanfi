function lambda = lambda_calculator(coordinate_x,coordinate_y,arena_setup)
x = coordinate_x;
y = coordinate_y;
RSSI = arena_setup;
exp = 3.0; 

lambda =  -15*(1*10^(-exp*RSSI(x,y)/(2.5*10)) - 5*10^(-(exp-1)*RSSI(x,y)/(2.5*10)));
end