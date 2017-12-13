function scaling_model(tp, np)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% instructions to use : in the command prompt, define the variables  %
% resolution = 100; np = 2.5; tp = 10                                %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% this model has been developed for one axis only. for a 2 - d system
% another model has been developed
linear_scale = zeros(1,1600);
check = 0;
while check ~= 1
    x_init = unidrnd(1200);
    if(x_init < 400 || x_init > 800)
        check = 1;
    else  
        check = 0;
    end 
end  
        
trans_x = 600;
padding_constant = 200;
x_init = x_init + padding_constant;
trans_x = trans_x + padding_constant;

for i = 1 : 1 : 1600
% 1 unit = 1cm %
            linear_scale (i) = rssi_model_new(abs(i - trans_x)/100, tp, np);% rssi is being calculated by taking distance in meters
end

linear_scale (trans_x) = linear_scale(trans_x - 1);

%figure;

scaling_model_path(linear_scale,x_init,np)
end