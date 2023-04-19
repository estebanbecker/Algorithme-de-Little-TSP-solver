param N, integer, >= 3;

set V := 1..N;
var order{V} >= 0;

#coordiante of node
param x{V};
param y{V};

param distance{i in V, j in V} := if sqrt((x[i]-x[j])^2 + (y[i]-y[j])^2) > 0 then sqrt((x[i]-x[j])^2 + (y[i]-y[j])^2) else 9999999999999;

var choice{V,V} binary;

minimize lengh : sum{i in V, j in V} choice[i,j] * distance[i,j];

s.t. oneout{i in V} : sum{j in V} choice[i,j] = 1;
s.t. onein{i in V} : sum{j in V} choice[j,i] = 1;

#subtour elimination using order
s.t. subtour{i in V, j in V: i != j and i > 1 and j > 1}: order[i] - order[j] + N * choice[i,j] <= N-1;

solve;

display lengh;

data;

param N := 10;

param x :=
    1 565
    2 25
    3 345
    4 945
    5 845
    6 880 
    7 25
    8 525
    9 580
    10 650;

param y :=
    1 575
    2 185
    3 750
    4 685
    5 655
    6 660 
    7 230
    8 1000
    9 1175
    10 1130;
