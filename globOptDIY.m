function y = griewank(in)
    sum = intval(0.0);
    prod = intval(1.0);

    for dimension = 1 : length(in)
        sum = sum + in(dimension) ^ 2 / 4000;
        prod = prod * cos(in(dimension)/sqrt(dimension));
    end
    y = sum - prod + 1;
end

function y = rohadek(x) % így csak 1 dimenzióban működik
    y = log(abs(3 * (intval(1) - x) + 1)) / 80 + x ^ 2 + 1;
end

function BB(fun, X, eps)
    domain_stack = [X]; % 
    value_stack = [inf(fun(X))]; % ebbe gyűjtjük az intervallumokon kiértékel függvényértékek infínumát
                                 % (az infínum gyakorlatilag azt jelenti mi a legjobb (minimalizálás szempontjából) érték amit itt felvehet a függvény)

    f = fun(mid(X)); % ez a bound a b&b-ben, ha bármikor a value_stack-ben olyan intervallum van aminek az infínuma az **f**-nél nagyobb,
                     % ott biztosan nem lesz a minimum
    dim = length(X);

    while(1)
        current_domain = domain_stack(end,:); % POP, minden sorban egy pár domain intervallum van (2d-ben téglalapok felett nézelődünk)
        domain_stack = domain_stack(1:end-1,:);
        current_value = value_stack(end); % POP
        value_stack = value_stack(1:end - 1);

        if(eps > diam(fun(current_domain))) % ha a függvényérték intervallum már elég kicsi
            disp("Epsilon wide function value inteval reached!");
            fun(current_domain)
            current_domain
            value_stack % mindenféle kiiratás
            break;
        end
        
        % még nem elég kicsi a függvényérték intervallum, jöhet a branching
        % 

        [wideness, index] = max(diam(current_domain)); % legszélesebb oldala mentén fogjuk félbevágni a téglalapot
                                                       % index == amelyik dimenzió mentén a legszélesebb a téglalap

        tempX1 = current_domain; % egyszerűség kedvéért current_domainre inicializáljuk
        tempX2 = current_domain; % ezekből lesz a két fél

        tempX1(index) = infsup(inf(current_domain(index)), mid(current_domain(index))); % a legszélesebb oldal mentén current_domain téglalap egyik fele
        tempX2(index) = infsup(mid(current_domain(index)), sup(current_domain(index))); % és a másik fele ;)

        domain_stack = [domain_stack; tempX1; tempX2]; % PUSHoljuk a stackre a két új intervallumot
        value_stack = [value_stack, inf(fun(tempX1)), inf(fun(tempX2))]; % PUSHoljuk a stackre a két új intervallumon vett függvényérték infínumait

        tempf1 = sup(fun(mid(tempX1))); % a két új intervallumbox közepén kiértékeljük a függvényt
        tempf2 = sup(fun(mid(tempX2))); % és vesszük ezeknek az értékeknek a suprenumát (kvázi legrosszabb esetben "max" ennyi lehet a függvényérték)
        f = min(f, min(tempf1, tempf2)); % a legkisebb kell legyen az **f**, minimumot keresünk elvégre

        domain_stack(value_stack > f, :) = []; % szórjuk ki az összes olyan értéket a domain-ek közül amelyek függvényértékeinek infinuma > mint az **f**
        value_stack(value_stack > f) = []; % szórjuk ki az összes olyan függvényérték infunumot ami > mint **f**

        [value_stack, sorted_indecies] = sort(value_stack, 'descend'); % rendezzük a domaineket a függvényértékük szerint csökkenő sorrendbe (így a stack tetjén lesz a legkisebb érték)
        domain_stack = domain_stack(sorted_indecies, :);
    end
end

x1=[intval('[-100,101]'),intval('[-100,101]')];
x2=infsup(0, 5);
BB(@rohadek, x2, 0.01)
