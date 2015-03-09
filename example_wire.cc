#include <iostream>
#include <math.h>
#include "assembly_triangle.h"

using namespace std;

int
main(int argc, char **argv) {
    double mu0 = 4*M_PI*1e-7;

    Region region_Cu ("Coil", 1/(mu0), 0, 1e7);
    Region region_Air("Air", 1/(mu0), 0, 0);
    map <int, Region> regions;

    regions[1] = region_Air;
    regions[2] = region_Cu;

    MeshFile *meshfile = new MeshFile("wire.msh");
    Mesh *mesh = new Mesh(meshfile);

    Assembly_Triangle *assembly_global = new Assembly_Triangle(mesh, regions);

    map <int, int> partition;
    for (int ind_node=0; ind_node < mesh->num_nodes; ind_node++) {
        partition[ind_node] = 0;
    }
    for (int ind_line = 0; ind_line < mesh->num_lines; ind_line++) {
        int ind_elem = mesh->lines[ind_line];
        Mesh_Element *elem = mesh->elements[ind_elem];
        int node1 = elem->nodes[0];
        int node2 = elem->nodes[1];
        if (elem->physical == 3) {
            partition[node1-1] = 1;
            partition[node2-1] = 1;
        }
    }

    vector <int> nodes_bnd;
    vector <int> nodes_free;
    for (map<int, int>::iterator it = partition.begin(); it!= partition.end(); ++it) {
        int part = it->second;
        int node = it->first;
        if (part == 0) nodes_free.push_back(node);
        if (part == 1) nodes_bnd.push_back(node);
    }
    int num_free = nodes_free.size();
    int num_bnd  = nodes_bnd.size();

    Matrix *S_FF = assembly_global->stiff_global->submatrix(nodes_free, nodes_free);
    Matrix *S_FI = assembly_global->stiff_global->submatrix(nodes_free, nodes_bnd);
    Matrix *S_IF = assembly_global->stiff_global->submatrix(nodes_bnd, nodes_free);
    Matrix *S_II = assembly_global->stiff_global->submatrix(nodes_bnd, nodes_bnd);

    vector <int> vone;
    vone.push_back(0);

    Matrix *F_F = assembly_global->excitation_global->submatrix(nodes_free, vone);
    double f_F[num_free];

    for (int ind=0;ind<num_free;ind++) {
        f_F[ind] = F_F->value(ind, 0);
    }

    Matrix L = Matrix(num_free, num_free);
    Matrix U = Matrix(num_free, num_free);
    Matrix P = Matrix(num_free, num_free);
    S_FF->LU(L, U, P);
    double *sol;
    sol = S_FF->LU_solve(L, U, P, f_F);

    for (int ind=0; ind < num_free; ind++) {
        int node = nodes_free[ind];
        cout << mesh->nodes[node*3] << " " << mesh->nodes[node*3+1] << " " <<sol[ind] << endl;
    }

    delete [] sol;
    delete F_F;

    delete S_FF;
    delete S_FI;
    delete S_IF;
    delete S_II;

    delete assembly_global;
    delete mesh;
    delete meshfile;

}
