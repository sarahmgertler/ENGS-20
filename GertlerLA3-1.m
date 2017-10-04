% SARAH GERTLER ENGS 20 LA3
% This is the main script file that calls euler_method and user_input
% Make side by side plots of equations 1 and 2 with parameters set by the
% user, as many times as the user wants.

% have user choose how many times they want to test it
ntest = input('How many times do you want to try approximating? \n')

% loop through ntest*2 times and plot (so ntest plots for each function)
for i = 1:(ntest*2)
    % call user input function for every other time
    if mod(i,2)==1 
        [x0, xn, h] = user_input();
    end
    % do side by side graphs: one with equation 1, and one with equation 2
    subplot(ntest,2,i,'replace')
    % call euler method function (alternating between eqs 1 and 2)
    euler_method(x0, xn, h, 2-mod(i,2))
    % add grid to graph
    grid on
    grid minor
end

%title over entire plot
suptitle('Sarah Gertler Long Assignment 3')

% ANSWER TO QUESTION 
% ------------------------------------------------------------------------
% I think that the euler method works pretty well (which makes sense, because
% it was Euler after all who came up with it). It works best with the smallest 
% step, so h=0.0001. At higher values of h (say, 0.1) it doesn't work quite 
% as well. My plots (and the legends that indicate distances between each 
% approximation and the actual function) illustrate that the approximation 
% using h=0.0001, Matlab's ODE45 calculator, the backwards Euler approximation, 
% and the actual y=f(x) function all come pretty close. 

% What is interesting is that the Euler approximation comes closer to the 
% actual function in the 1st example (in terms of distance) but doesn't do 
% as good of a job as the ODE solver or the backwards euler. In the 2nd 
% example the Euler approximation does a better job than the backwards 
% euler, but its average distance from the actual function is greater 
% than in the 1st equation.
