param n, integer, >= 3;

set V := 1..n;

set E := {(i,j) in V cross V};

#coordiante of node
param coord_x{V};
param coord_y{V};

param c{(i,j) in E}:= if sqrt((coord_x[i]-coord_x[j])^2 + (coord_y[i]-coord_y[j])^2) > 0 then sqrt((coord_x[i]-coord_x[j])^2 + (coord_y[i]-coord_y[j])^2) else 9999999999999;
/* distance from node i to node j using euclidian distance*/

var x{(i,j) in E}, binary;/*choose the path*/

minimize total: sum{(i,j) in E} c[i,j] * x[i,j];

/*be sure that each node is visited only once*/

s.t. leave{i in V}: sum{(i,j) in E} x[i,j] = 1;

s.t. enter{j in V}: sum{(i,j) in E} x[i,j] = 1;


/*be sure that the tour is a cycle using a countdown*/
var y{(i,j) in E}, >= 0;

s.t. cap{(i,j) in E}: y[i,j] <= (n-1) * x[i,j];

s.t. node{i in V}: sum{(j,i) in E} y[j,i] + (if i = 1 then n) = sum{(i,j) in E} y[i,j] + 1;

solve;

printf "Optimal tour has length %f\n",
   sum{(i,j) in E} c[i,j] * x[i,j];
printf("From node   To node   Distance\n");
printf{(i,j) in E: x[i,j]} "      %3d       %3d   %8g\n",
   i, j, c[i,j];

data;

param n := 26;

param coord_x :=
    1 565
    2 25
    3 345
    4 945
    5 845
    6 880
    7 25
    8 525
    9 580
    10 650
    11 1605
    12 1220
    13 1465
    14 1530
    15 845
    16 725
    17 145
    18 415
    19 510
    20 560
    21 300
    22 520
    23 480
    24 835
    25 975
    26 1215
    ;

param coord_y :=
    1 575
    2 185
    3 750
    4 685
    5 655
    6 660
    7 230
    8 1000
    9 1175
    10 1130
    11 620
    12 580
    13 200
    14 5
    15 680
    16 370
    17 665
    18 635
    19 875
    20 365
    21 465
    22 585
    23 415
    24 625
    25 580
    26 245
    ;
