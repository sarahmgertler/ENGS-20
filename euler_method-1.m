% NOTE: THIS FILE GOES WITH GERTLER-LA-3
function euler_method(x0, xn, h, choice)
% This function plots a function y' = f(x,y) using the Euler method
% This function loops through the h-vector input by user and for each step
% creates the appropriate x and y vectors. It then  plots the euler 
% approximation of the inputted f(x,y) by looping through the x and y vectors 
% (in a nested loop). It decides which f(x,y) to approximate based on
% 'choice' (1 or 2)
% To check the robustness of the Euler method, this function also plots the
% function using the  backwards euler method, the ODE45 solved version of 
% the equation, and the actual function, in order to compare the euler 
% method to these other approximations.
% Finally, this function calculates the average distance between each
% approximation and the actual function, and displays it on a legend.

if choice==1
    y0 = 1;
    % assign an anonymous function for the actual function
    f = @(x) 1+(exp(-4.*x)./2) - (exp(-2.*x)./2);
    % assign an anonymous function for the euler approximation
    g = @(x,y,k,h) y(k) + h * (2 - exp(-4*x(k)) - 2*y(k));
    % backwards euler approximation (solving for y(k+1))
        % y(k+1) = y(k) + h*(2-exp(-4*x(k+1))-2*y(k+1))
        % y(k+1) = y(k) + 2h - h*exp(-4*x(k+1)) -2*h*y(k+1)
        % (1+2h)y(k+1) = y(k) + 2h-h*exp(-4*x(k+1))
        % y(k+1) = [y(k) + 2h - h*exp(-4*x(k+1))]/(1+2h)
    
    b = @(x,y,k,h) (y(k) + h*(2 - exp(-4*x(k+1))))/(1+2*h);

else
    y0=0; %for choice=2 or default
    
    % assign an anonymous function for the actual function
    f = @(x) exp(x./2).*sin(5.*x);
    
    % assign an anonymous function for the euler approximation
    g = @(x,y,k,h) y(k)+ h*(y(k) - (1/2)*exp(x(k)/2)*sin(5*x(k)) + 5*exp(x(k)/2)*cos(5*x(k))) ;
    
    %backwards euler approximation (solving for y(k+1))
        % y(k+1) = y(k)+ h*(y(k+1) - (1/2)*exp(x(k+1)/2)*sin(5*x(k+1)) + 5*exp(x(k+1)/2)*cos(5*x(k+1)))
        % y(k+1) - h*y(k+1) = y(k) + h*(-(1/2)*exp(x(k+1)/2)*sin(5*x(k+1)) + 5*exp(x(k+1)/2)*cos(5*x(k+1)))
        % y(k+1) = (y(k)+h*(-(1/2)*exp(x(k+1)/2)*sin(5*x(k+1))+5*exp(x(k+1)/2)*cos(5*x(k+1))))/(1-h)
    b = @(x,y,k,h) (y(k)+h*(-(1/2)*exp(x(k+1)/2)*sin(5*x(k+1))+5*exp(x(k+1)/2)*cos(5*x(k+1))))/(1-h) ;

end
legend OFF

for i = h
    % create x vector, starting at x0 end ending at x_n, and with steps of
    % h (which are looped through)
    % turn hold on for second iteration
     x = [x0:i:xn];
     
     % determine length of x, and construct y the same size
     n = length(x);
     y = zeros(1,n);
     
     % construct vector p for backwards
     p = zeros(1,n);
    
     % initialize first item in y vector to y0
     y(1) = y0;
     p(1) = y0;
     
     % loop through the x,y vectors and calculate the value for each
     % corresponding yn using Euler's method
    for k = 1:n-1
        % euler method
        y(k+1) = g(x,y,k,i);
        
        % backward euler method
        p(k+1) = b(x,p,k,i);
    end
    % plot each time, grid is on so will keep overlaying
    
    % calculate dif from actual function for both euler and backward euler
    % this comes out to a vector
        % euler
    dif_actual = y-f(x);
        % backward euler
    dif_backward = p-f(x);
    
    % plot extra detail on smallest h, because this will be closest to accurate
   
    if i==min(h)
        % select every 1/ith * 0.1 (i.e. 1000th if i=0.0001) to plot marker otherwise line would be really   
        
        % plot backwards euler first so it is below euler
            % put in legend: mean of diff_backwards vector (avg dist btw backward euler and actual function) 
        plot(x(1:(1/i)*.1:end),p(1:(1/i)*.1:end), '-.d', 'MarkerSize', 9.5, 'MarkerEdgeColor', 'g', 'MarkerFaceColor', 'g', 'linewidth', 0.75, 'DisplayName', ['(b.euler) h=' num2str(i) '(d\_actual=' num2str(mean(dif_backward), 5) ')'])

        % turn hold on after 1st plot
        if i==h(1)
            hold on
        end
       
        % plot especially large marker on smallest h for euler method
              % put in legend: mean of diff_actual vector (avg dist btw euler method and actual function) 
        plot(x(1:(1/i)*.1:end),y(1:(1/i)*.1:end), '-.*', 'MarkerSize', 15, 'MarkerEdgeColor', 'm', 'linewidth', 0.75,  'DisplayName', ['h=' num2str(i) '(d\_actual=' num2str(mean(dif_actual), 5) ')'])
        

    else
        % don't plot extra large marker on rest, and don't plot backwards
        % for rest
             % put in legend: mean of diff_actual vector (avg dist btw euler method and actual function) 
        plot(x,y, 'linewidth', 0.75, 'DisplayName', ['h=' num2str(i) '(d\_actual=' num2str(mean(dif_actual), 5) ')'])
    
        % turn hold on after 1st plot
        if i==h(1)
            hold on
        end
    end

end

% plot actual
if choice==1
    
    % built in ode solver - ode45
    [x,y] = ode45(@(x,y) 2 - exp(-4.*x) - 2.*y, [x0 xn], y0);
       % calculate dif of ode solver from actual
    dif_ode = y-f(x);
    
    plot(x, y, ':o', 'linewidth', 0.75, 'MarkerFaceColor', 'c', 'DisplayName', ['ODE Solver (d\_actual=' num2str(mean(dif_ode), 5) ')'])
    
    % plot solved equation in black dotted line so it is a clear contrast
    % but it doesnt over power everything else
    
        % note: plot(x,f(x) is in if-else because i want it on top of ODE solver
        % but before title/legend (specific to equations 1 or 2), so i
        % figured repeating the plot(x,f(x)) would be less lines of code
    plot(x, f(x), 'k:', 'linewidth', 2, 'DisplayName', 'Actual')
    
    % label axes with x and y initial values (input by user/given from function)
    xlabel(['x values (x_0=' num2str(x0) ', x_n=' num2str(xn) ')'])
    ylabel(['y values (y_0=' num2str(y0) ')'])
    
    % title is equation 1
    title(['Equation 1: f(x,y) = 2 - e^{', num2str(-4), 'x} - 2y'])
    
    % legend in bottom right, works best with function
    legend(gca, 'show', 'Location', 'southeast')
    hold off

    
else %choice==2 and default
     
    % use ode45 to solve and plot
    [x,y] = ode45(@(x,y) y - (1/2)*exp(x./2)*sin(5.*x) + 5*exp(x./2)*cos(5.*x), [x0 xn], y0);
         % calculate dif from actual
    dif_ode = y-f(x);
    
    plot(x, y, ':o', 'linewidth', 0.75, 'MarkerFaceColor', 'c', 'DisplayName', ['ODE Solver d\_actual=' num2str(mean(dif_ode), 3) ')'])

    % plot solved equation
    plot(x, f(x), 'k:', 'linewidth', 2, 'DisplayName', 'Actual')

    % label axes with x and y initial values (input by user/given from function)
    xlabel(['x values (x_0=' num2str(x0) ', x_n=' num2str(xn) ')'])
    ylabel(['y values (y_0=' num2str(y0) ')'])
    
    % title is equation 2
    title(['Equation 2: f(x,y) = y + ', num2str(1/2), 'e^{', num2str(1/2), 'x}sin(5x) + 5e^{', num2str(1/2), 'x}cos(5x)'])
 
    % legend in top left, works best with funtion
    legend(gca, 'show', 'Location', 'northwest')
    hold off
    
end

end

