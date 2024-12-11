function tehetseges_zh
    X = infsup(-2.0, 2.2);


    BnB(X, @f, 0.001)
end

function y = f(X)
    y = 2 + intval(0.003) * X ^ 2 - 2 * sin(2 * intval('pi') * X);
end

function m = f_prime(X)
    two = [intval(2), 0];
    x_intv = [X, 1];
    pi_intv = [intval('pi'), 0];
    three_per_thousand = [intval(0.003), 0];

    ret = mul(pi_intv, x_intv);

    two_times_pi_times_x = mul(two, mul(pi_intv, x_intv));
    x_sqrd_times_three_per_thousand = mul(three_per_thousand, pow(x_intv, 2));

    utolso_tag = mul([intval(-2), 0], isin(two_times_pi_times_x));

    sum = add(two, add(x_sqrd_times_three_per_thousand, utolso_tag));

    m = sum(2);
end

function m = h(X)
    m = pow([X, 1], 2);
end

function BnB(X, fun, eps)
    domain_stack = X;

    while(size(domain_stack) > 0)
        current_domain = domain_stack(end, :)
        domain_stack = domain_stack(1 : end-1, :);

        if(inf(fun(current_domain)) > 0)
            continue;
        end
        if(diam(current_domain) < eps)
            disp('Kisebb az intervallum mint a megadott epszilon, lehet vesz fel negatív értéket a függvény')
            current_domain
            break;
        end

        tempX1 = infsup(inf(current_domain), mid(current_domain));
        tempX2 = infsup(mid(current_domain), sup(current_domain));

        m1 = f_prime(tempX1);
        m2 = f_prime(tempX2);

        if(inf(m1) > 0 || sup(m1) < 0)
            if(inf(f_prime(tempX1)) > 0 && fun(inf(tempX1)) < 0 || sup(f_prime(tempX1)) < 0 && fun(sup(tempX1)) < 0)
                domain_stack = [domain_stack; tempX1];
            end
        else
            domain_stack = [domain_stack; tempX1];
        end
        if(inf(m2) > 0 || sup(m2) < 0)
            if(inf(f_prime(tempX2)) > 0 && fun(inf(tempX2)) < 0 || sup(f_prime(tempX2)) < 0 && fun(sup(tempX2)) < 0)
                domain_stack = [domain_stack; tempX2];
            end
        else
            domain_stack = [domain_stack; tempX2];
        end

    end
end

function F = add(f, g)
    F(1) = f(1,1) + g(1,1);
    F(2) = f(1,2) + g(1,2);
end

function F = mul(f, g)
    F(1) = f(1,1) * g(1,1);
    F(2) = f(1,1) * g(1,2) + f(1,2) * g(1,1);
end

function F = isin(f)
    F(1) = sin(f(1,1));
    F(2) = cos(f(1,1)) * f(1, 2);
end

function F = pow(f, n)
    F(1) = f(1,1) ^ n;
    F(2) = n * f(1,1) ^ (n - 1) * f(1, 2);
end