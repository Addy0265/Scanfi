function scaling_model_path(linear_scale,x_init,np)

% this model has been developed for one axis only. for a 2 - d system
% another model has been developed

to_plot = zeros(20,3);

Pr = (linear_scale);
x_start = x_init
exp = 3; 
step = 13;
scale = 1;
direction = randi ([0,1]) % direction = 0 or -1 ---> -ve direction
if direction == 0;
    direction = -1;
end

x_lof_1 = x_start + direction*step
delta_x = scale*Pr(x_lof_1) - Pr(x_start)
%lambda_x = -3.5*(1*10^(-exp*Pr(x_start)/(2.5*10)) - 5*10^(-(exp-1)*Pr(x_start)/(2.5*10)))
%lambda_x = 10^(-exp*Pr(x_start)/(2.5*10))
% if delta_x > 0
   % x_next_ = floor(x_start + direction*(delta_x)*lambda_x)
% else 
%     x_next_ = floor(x_start - (delta_x)*lambda_x)
% end      

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%        
% loop to model lambda
lambda_array = zeros(1,1600);
for i = 1 : 1 : 1600
  lambda_array(i) = -12.5*(1*10^(-(exp)*(Pr(i))/(np*10)) - 5*10^(-(exp-1)*(Pr(i))/(np*10)));
  %lambda_array(i) = 1*10^(-1*(Pr(i)+128)/(np*10)+0.4001); %equation for distance from rssi
  %lambda_array(i) = 1*10^(-exp*(Pr(i)+128)/(2.5*10));
end 

plot(lambda_array);
hold on
plot(Pr);
%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%plot(linear_scale);
%plot (lambda_x);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% loop to model number of iterations required to reach the final destination
%
% for s = 1 : 1 : 120 %number of iterations
%     x_start = x_next_;
%     direction = randi ([0,1]);
%     if direction == 0;
%         direction = -1;
%     end
%     x_lof_1 = x_start + direction*step;
%     delta_x = scale*Pr(x_lof_1) - Pr(x_start);
%     %lambda_x = 10^(-exp*Pr(x_start)/(2.5*10));
%     lambda_x = -3.9*(1*10^(-exp*Pr(x_start)/(2.5*10)) - 5*10^(-(exp-1)*Pr(x_start)/(2.5*10)));
% 
%     x_next_ = floor(x_start + direction*delta_x*lambda_x);
% 
% end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%x_next_
end