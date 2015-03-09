#include <iostream>
#include "region.h"
#include <map>

Region::Region(string _name, double _nu, double _sigma, double _Js) {
   name = _name;
   nu = _nu;
   sigma = _sigma;
   Js = _Js;
}

Region::Region() {
   name = "";
   nu = sigma = Js = 0;
}

void
Region::disp() {
    cout << name << endl;
    cout << "nu=" << nu
         << ", sigma=" << sigma
         << ", Js=" << Js
         << endl;
}

#ifdef REGION_TEST

int
main(int argc, char ** argv) {
    Region regFe("Core", 1, 2);
    Region regCu("Copper", 2, 3);

    map <int, Region> regions;
    regions[23] = regFe;
    regions[92] = regCu;
    cout << regions.size() << endl;

    cout << "23-" << regions.count(23) << endl;
    cout << "24-" << regions.count(24) << endl;
//    regions[229834].disp();
    cout << regions.size() << endl;

    map <int, Region> tmp;
    tmp = regions;
    tmp[23].disp();
}
#endif
