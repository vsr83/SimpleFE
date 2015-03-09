#ifndef ASSEMBLY_TRIANGLE_H
#define ASSEMBLY_TRIANGLE_H

#include <map>
#include <vector>

#include "matrix.h"
#include "mesh.h"
#include "region.h"

using namespace std;

class Assembly_Triangle {
private:
    Matrix stiff_element_analytical(double *x, double *y, double nu);
    Matrix mass_element_analytical (double *x, double *y, double sigma);
public:
    Assembly_Triangle(Mesh *_mesh, map <int, Region> _regions);
    ~Assembly_Triangle();

    double triangle_area(int ind_triangle);

    Mesh   *mesh;
    Matrix *stiff_global;
    Matrix *mass_global;
    Matrix *excitation_global;

    int num_DoFs;
    map <int, Region> regions;
    vector <Matrix> tri_Jacobians;
    vector <Matrix> stiff_elem;
    vector <Matrix> mass_elem;
};

#endif // DOF_GLOBAL_H
