% NOTE: this file goes with Gertler-LA-3 and euler-method

function [x0, xn, h] = user_input
% This function prompts user for starting x position, ending x position,
% and vector of h (step) values.

% prompt user for input
x0 = input('Please enter the initial x value:\n')
xn = input('Please enter the final x value:\n')
%ASK: input vector?
h = input('Please enter the steps you''d like to test (list them in []''s):\n')
end
