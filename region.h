#ifndef REGION_H
#define REGION_H

#include <string>

using namespace std;

class Region {
public:
    void disp();
    Region();
    Region(string _name, double _nu, double _sigma, double _Js);

    string name;
    double nu, sigma, Js;
};

#endif // REGION_H
