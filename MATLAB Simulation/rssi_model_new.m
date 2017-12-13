function rssi = rssi_model_new(distance, tp, np)
% tp is transmit power in watts
%rssi = -10*log10(tp*((distance).^np)); % if the SSD one does not work, use this
% we are assuming all the other gains(Gt, Gr etc) to be equal to 1%
%rssi = (30 + 10*log10(tp)) - (128.1 + 10*np*log10(distance)); %ssd waala
rssi = floor(floor(2*(30 + 10*log10(tp) - 10*np*log10(distance)))/2); % realistic model of rssi

%path loss model; the whole equation is in dBm; 
%received power = transmitpower - pathloss
end