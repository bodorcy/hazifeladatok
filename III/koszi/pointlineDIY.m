function drawBnB(Px, Py, color)
    plot([inf(Px),sup(Px),sup(Px),inf(Px),inf(Px)],...
            [inf(Py),inf(Py),sup(Py),sup(Py),inf(Py)],color);
end

function d = pointLineDistance(point, lineP1, lineP2)
    % távolság meghatározása háromszög determinánsos módszerrel
    % (igazából itt a háromszög területének kétszeresét kapjuk, de csak az a fontos, hogy benne van-e a 0 a terület intervallumában)
    d = point(1) * lineP1(2) + point(2) * lineP2(1) + lineP1(1) * lineP2(2) - lineP1(2) * lineP2(1) - lineP2(2) * point(1) - point(2) * lineP1(1);
end

function pointLineBnB(point, lineP1, lineP2, epsilon)
    res = 0;
    stack = [point];
    while(size(stack, 2) > 0) % amíg van a stackben elem
        interval_to_be_checked = [stack(end-1), stack(end)]; % pop legfelső
        stack = stack(1:end-2); % töröljük a legfelsőt mert pop

        distance_interval = pointLineDistance(interval_to_be_checked, lineP1, lineP2);
        res = interval_to_be_checked; % lehet valahol el van csúszva az hogy mit poppol
        if(inf(distance_interval) > 0 || 0 > sup(distance_interval))
            drawBnB(interval_to_be_checked(1), interval_to_be_checked(2), 'g');

        else

            % legszélesebb oldal hossza és indexe az oldalakon belül
            [max_width, index] = max(diam(interval_to_be_checked));

            if(epsilon > max_width)
                res = 0;
                disp('Interval is smaller than the given epsilon, point might be on line...')
                interval_to_be_checked
                return;
            end
        
            % ha a poppolt intervallum olyan távolságra van a vonaltól hogy
            % benne van a 0, akkor kettévágjuk és belerakjuk a két új
            % intervallumot a stackbe

            % a legszélesebb oldala mentén vágjuk ketté
            temp_interval1 = interval_to_be_checked;
            temp_interval1(index) = infsup(inf(interval_to_be_checked(index)), mid(interval_to_be_checked(index)));
            temp_interval2 = interval_to_be_checked;
            temp_interval2(index) = infsup(mid(interval_to_be_checked(index)), sup(interval_to_be_checked(index)));

            stack = [stack, temp_interval1]; % push
            stack = [stack, temp_interval2]; % push
        end
    end
    disp('A pont nincs rajta az egyenesen!')
    res
end
    

point = [infsup(0.3,0.4),infsup(0.4,2.501)];
lineP1 = [intval(0), sqrt(intval(2)/3+midrad(0,0.01))];
lineP2 = [intval(1),intval('pi')];

hold on;
plot([inf(lineP1(1)), inf(lineP2(1))], [inf(lineP1(2)), inf(lineP2(2))], 'b');
plot([sup(lineP1(1)), sup(lineP2(1))], [sup(lineP1(2)), sup(lineP2(2))], 'r');

drawBnB(point(1), point(2), 'g');

pointLineBnB(point, lineP1, lineP2, 1e-12)