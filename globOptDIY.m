function y = griewank(x)
    sum = intval(0.0);
    prod = intval(1.0);

    for dimension = 1 : length(x)
        sum = sum + x(dimension) ^ 2 / 4000;
        prod = prod * cos(x(dimension)/sqrt(dimension));
    end
    y = sum - prod + 1;
end

x=[intval('[-100,101]'),intval('[-100,101]')];

griewank(x)
function BB(fun, X, eps)
    domain_stack = [X];
    value_stack = [inf(fun(X))];
    f = fun(mid(X));
    dim = length(X);

    while(1)
        current_domain = domain_stack(end,:);
        domain_stack = domain_stack(1:end-1,:);
        current_value = value_stack(end);
        value_stack = value_stack(1:end - 1);

        if(eps > diam(fun(current_domain)))
            disp("Epsilon wide function value inteval reached!");
            current_value
            current_domain
            %value_stack
            break;
        end
        [wideness, index] = max(diam(current_domain));
        tempX1 = current_domain;
        tempX2 = current_domain;

        tempX1(index) = infsup(inf(current_domain(index)), mid(current_domain(index)));
        tempX2(index) = infsup(mid(current_domain(index)), sup(current_domain(index)));

        domain_stack = [domain_stack; tempX1; tempX2];
        value_stack = [value_stack, inf(fun(tempX1)), inf(fun(tempX2))];

        tempf1 = fun(mid(tempX1));
        tempf2 = fun(mid(tempX2));
        f = min(f, min(sup(tempf1), sup(tempf2)));

        domain_stack(value_stack > f, :) = [];
        value_stack(value_stack > f) = [];
    end
end

BB(@griewank, x, 0.01)