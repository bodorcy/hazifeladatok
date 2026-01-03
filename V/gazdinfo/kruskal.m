function kruskal

table =...
  [NaN     0     0     0     0     0     0     0     0     0     0     0;
   231   NaN     0     0     0     0     0     0     0     0     0     0;
   158   136   NaN     0     0     0     0     0     0     0     0     0;
   126   354   280   NaN     0     0     0     0     0     0     0     0;
   189   417   344   203   NaN     0     0     0     0     0     0     0;
    81   182   171   205   188   NaN     0     0     0     0     0     0;
   177   108    71   299   362   190   NaN     0     0     0     0     0;
   205   353   359   301    64   171   378   NaN     0     0     0     0;
   165   227   257   288   219    86   276   182   NaN     0     0     0;
    67   295   221    88   127   142   240   158   226   NaN     0     0;
   111   338   265    81   123   186   284   154   270    45   NaN     0;
   233   460   387   155   124   307   406   186   391   170   126   NaN]

N = size(table, 2)
E = [];
V = 1:N; % minden csúcs külön fába
V = V';

for i=1:N
    for j=1:i-1
        E = [E; i j table(i,j)];
    end
end

[~, I] = sort(E(:,3));
E = E(I,:);

mst = [];
opt = 0;

for i=1:size(E,1)
    
    if V(E(i,1)) ~= V(E(i,2))
        V(V == V(E(i,2))) = V(E(i,1));
        mst = [mst; E(i,:)];
        opt = opt + E(i,end);
    end

    if length(mst) == N-1
        break;
    end
end

mst
opt

end
