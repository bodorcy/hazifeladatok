function Globopt

x=-100:1:100;
y=-100:1:100;

[X Y]=meshgrid(x,y);
for i=1:length(x)
    for j=1:length(y)
        Z(i,j)=griewank([x(i) y(j)]);
    end
end

surf(X,Y,Z)

XX=[intval('[-100,101]'),intval('[-100,101]')];
BB(@griewank,XX)

end

function BB(F,X)
Vx=X; % search domain stack
Vf=inf(F(X)); % function value stack
              % we only look at the infinum because we are aiming to find
              % the global infinum, if at any point of the iterations a
              % function value interval's infinum is higher than the
              % current f value we discard said function value's search
              % domain

f=sup(F(intval(mid(X)))); % initialize global maximum f hat
% if any function value's inf is larger than f (f is sup of a midpoint's function value interval), ( inf(F(tempX)) > f ), then we
% can discard tempX

% f plays the role of bound in b&b

while (1)
    tempX=Vx(1,:); % pop domain from stack
    tempf=Vf(1); % pop interval of corresponding function values form stack
    Vx(1,:)=[];
    Vf(1)=[];

    if diam(F(tempX))<0.01 % if we reached epsilon wide interval
        f-tempf
        tempX
        Vx
        break;
    end
    
    % here comes the branch part
    [u v]=max(diam(tempX)); %widest dimension u = width, v = index
    m=mid(tempX(v)); %widest dimension's midpoint
    tempX1=tempX;
    tempX1(v)=infsup(inf(tempX(v)),m); % first half of split interval along the widest dimension
    tempX2=tempX;
    tempX2(v)=infsup(m,sup(tempX(v))); % second half of split interval along the widest dimension
    Vx=[Vx;tempX1;tempX2]; % place the two new split domain intervals on the stack

    tempf1=inf(F(tempX1)); % infinum of the first half's function value interval
    tempf2=inf(F(tempX2)); % infinum of the second half's function value interval

    Vf=[Vf,tempf1,tempf2]; % place the two split domain's function values interval's infinum on the stack

    % check if we find a lower upper bound (f)
    f=min(f,sup(F(intval(mid(tempX1))))); % see if the new intervals midpoints function values are smaller than the current global maximum
    f=min(f,sup(F(intval(mid(tempX2))))); % rinse and repeat

    [Vf i]=sort(Vf);
    Vx=Vx(i,:);

    i=Vf>f; % delete all the domains and fucntion values (both intervals) where the fucntion value intervals inf is larger than f hat (smallest sup of the middle values)
    Vx(i,:)=[];
    Vf(i)=[];
end

    

end

function y=griewank(xx)
d=length(xx);
if class(xx(1))=='intval'
    sum=intval(0.0);
    prod=intval(1.0);
else
    sum=0.0;
    prod=1.0;
end
for ii=1:d
    xi=xx(ii);
    sum=sum+xi^2/4000;
    prod=prod*cos(xi/sqrt(ii));
end
y=sum-prod+1;
end