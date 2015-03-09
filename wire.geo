
w= 1;
h= 0.5;
mb = 0.05;
mw = 0.01;

x = 0.2;
r = 0.025;

Point(1) = {0, -h, 0, mb};
Point(2) = {0,  h, 0, mb};
Point(3) = {w,  h, 0, mb};
Point(4) = {w, -h, 0, mb};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};

Point(5) = {x-r, 0, 0, mw};
Point(6) = {  x, 0, 0, mw};
Point(7) = {x+r, 0, 0, mw};
Circle(5) = {5, 6, 7};
Circle(6) = {7, 6, 5};

Line Loop(1) = {1, 2, 3, 4};
Line Loop(2) = {5, 6};

Plane Surface(1) = {1, 2};
Physical Surface(1) = {1};
Plane Surface(2) = {2};
Physical Surface(2) = {2};
Physical Line(3) = {1, 2, 3, 4};