// CREATED BY A SCRIPT - DO NOT EDIT!
gm = 0.000005;


// Boundary parameters
bminy = -0.008;
bmaxy = 0.012;
bmaxx = 0.015;


bm = 0.001;
cm = 0.0004;
clm = 0.0002;
am = 0.0004;
gm = 0.0001;

// Air gap parameters
gw = 0.00025;
//gw = 0.0005;

// Points and lines of the outer boundary
Point(1) = {0, bminy, 0, 1.5*bm};
Point(2) = {0, -gw, 0, 0.5*bm};
Point(3) = {0, 0, 0, 0.5*bm};
Point(4) = {0, bmaxy, 0, 1.5*bm};
Point(5) = {bmaxx, bmaxy, 0, 3*bm};
Point(6) = {bmaxx, 0, 0, 3*bm};
Point(7) = {bmaxx, -gw, 0, 3*bm};
Point(8) = {bmaxx, bminy, 0, 3*bm};

Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 5};
Line(5) = {5, 6};
Line(6) = {6, 7};
Line(7) = {7, 8};
Line(8) = {8, 1};

// Armature
Point(9)  = {0.001, -gw-0.002, 0, am};
Point(10) = {0.001, -gw, 0, gm};
Point(11) = {0.005, -gw, 0, gm};
Point(12) = {0.005, -gw-0.001, 0, am};
Point(13) = {0.0025, -gw-0.002, 0, am};
Line(9) = {9, 10};
Line(10) = {10, 11};
Line(11) = {11, 12};
Line(12) = {12, 13};
Line(13) = {13, 9};

// Core
Point(14) = {0.001, 0, 0, gm*2};
Point(15) = {0.001, 0.006, 0, cm};
Point(16) = {0.005, 0.006, 0, cm};
Point(17) = {0.005, 0, 0, gm*2};
Point(18) = {0.004, 0, 0, 1.2*gm};
Point(19) = {0.004, 0.004, 0, 0.75*cm};
Point(20) = {0.0025, 0.004, 0, 0.75*cm};
Point(21) = {0.0025, 0.000, 0, 1.2*gm};

Line(14) = {14, 15};
Line(15) = {15, 16};
Line(16) = {16, 17};
Line(17) = {17, 18};
Line(18) = {18, 19};
Line(19) = {19, 20};
Line(20) = {20, 21};
Line(21) = {21, 14};

// Coil
Point(22) = {0.003, 0.001, 0, clm};
Point(23) = {0.003, 0.003, 0, clm};
Point(24) = {0.0035, 0.003, 0, clm};
Point(25) = {0.0035, 0.001, 0, clm};

Line(22) = {22, 23};
Line(23) = {23, 24};
Line(24) = {24, 25};
Line(25) = {25, 22};

// Noneuclidian region

Line(26) = {3, 14};
Line(27) = {21, 18};
Line(28) = {17, 6};
Line(29) = {7, 11};
Line(30) = {10, 2};

// Outer Boundary
Line Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8};
// Armature
Line Loop(2) = {9, 10, 11, 12, 13};
// Core
Line Loop(3) = {14, 15, 16, 17, 18, 19, 20, 21};
// Coil
Line Loop(4) = {22, 23, 24, 25};
// Air 1
Line Loop(5) = {1, -30, -9, -13, -12, -11, -29, 7, 8};
// Air 2
Line Loop(6) = {3, 4, 5, -28, -16, -15, -14, -26};
// Outer boundary of the air region surrounding the coil
Line Loop(7) = {-20, -19, -18, -27};
// Noneuclidian air region
Line Loop(8) = {2, 26, -21, 27, -17, 28, 6, 29, -10, 30};

Plane Surface(1) = {2}; // Armature
Plane Surface(2) = {3}; // Core
Plane Surface(3) = {4}; // Coil

Plane Surface(5) = {5}; // Air1
Plane Surface(6) = {6}; // Air2
Plane Surface(7) = {7, 4}; // Air3
Plane Surface(8) = {8}; // Noneucl

Physical Surface(201) = {1};
Physical Surface(202) = {2};
Physical Surface(203) = {3};
Physical Surface(204) = {5, 6, 7};
Physical Surface(205) = {8};
Physical Line(206) = {1,2,3, 4, 5, 6, 7, 8};
