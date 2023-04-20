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

param N := 52;

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
    27 1320
    28 1250
    29 660
    30 410
    31 420
    32 575
    33 1150
    34 700
    35 685
    36 685
    37 770
    38 795
    39 720
    40 760
    41 475
    42 95
    43 875
    44 700
    45 555
    46 830
    47 1170
    48 830
    49 605
    50 595
    51 1340
    52 1740
    ;

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
    27 315
    28 400
    29 180
    30 250
    31 555
    32 665
    33 1160
    34 580
    35 595
    36 610
    37 610
    38 645
    39 635
    40 650
    41 960
    42 500
    43 920
    44 500
    45 815
    46 485
    47 65
    48 610
    49 625
    50 360
    51 725
    52 245
    ;
