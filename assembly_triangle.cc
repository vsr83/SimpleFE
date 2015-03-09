#include "assembly_triangle.h"
#include <math.h>

double
Assembly_Triangle::triangle_area(int ind_triangle) {
    int ind_elem = mesh->triangles[ind_triangle];
    Mesh_Element *elem = mesh->elements[ind_elem];
    double x[elem->nnodes], y[elem->nnodes], z[elem->nnodes];

    for (int ind_node=0; ind_node<elem->nnodes; ind_node++) {
        int nodenum = elem->nodes[ind_node];

        // Note that the node, element and triangle numbers used by GMsh differ by
        // one from the corresponding array location:
        x[ind_node] = mesh->nodes[(nodenum-1)*3];
        y[ind_node] = mesh->nodes[(nodenum-1)*3+1];
        z[ind_node] = mesh->nodes[(nodenum-1)*3+2];
    }

    double x1 = x[0], x2 = x[1], x3 = x[2],
           y1 = y[0], y2 = y[1], y3 = y[2];

    return 0.5*fabs((x3-x1)*(y2-y1) - (y3-y1)*(x2-x1));
}

Matrix
Assembly_Triangle::stiff_element_analytical(double *x, double *y, double nu) {
    double x1 = x[0], x2 = x[1], x3 = x[2],
           y1 = y[0], y2 = y[1], y3 = y[2];

    double area = 0.5*fabs((x3-x1)*(y2-y1) - (y3-y1)*(x2-x1));
    double    f = nu/(4*area);
    double Se11 = ((y2-y3)*(y2-y3) + (x3-x2)*(x3-x2))*f,
           Se22 = ((y3-y1)*(y3-y1) + (x1-x3)*(x1-x3))*f,
           Se33 = ((y1-y2)*(y1-y2) + (x2-x1)*(x2-x1))*f,
           Se12 = ((y2-y3)*(y3-y1) + (x3-x2)*(x1-x3))*f,
           Se13 = ((y2-y3)*(y1-y2) + (x3-x2)*(x2-x1))*f,
           Se23 = ((y3-y1)*(y1-y2) + (x1-x3)*(x2-x1))*f;
    double Sedata[] = {Se11, Se12, Se13,
                       Se12, Se22, Se23,
                       Se13, Se23, Se33};
    Matrix Se(3, 3, Sedata);
    return Se;
}

Matrix
Assembly_Triangle::mass_element_analytical(double *x, double *y, double sigma) {
    double x1 = x[0], x2 = x[1], x3 = x[2],
           y1 = y[0], y2 = y[1], y3 = y[2];

    double area = 0.5*fabs((x3-x1)*(y2-y1) - (y3-y1)*(x2-x1));
    double    f = sigma*area/(12);
    double Me11 = 2*f, Me12 = f;
    double Medata[] = {Me11, Me12, Me12,
                       Me12, Me11, Me12,
                       Me12, Me12, Me11};
    Matrix Me(3, 3, Medata);
    return Me;
}


Assembly_Triangle::Assembly_Triangle(Mesh *_mesh, map <int, Region> _regions) {
    mesh = _mesh;
    regions = _regions;

#ifdef DEBUG
    cout << "MESH: " << endl
         << mesh->num_triangles << " triangles " << endl
         << mesh->num_lines << " lines " << endl
         << mesh->num_nodes << " nodes" << endl;
#endif

    // Build stiffness and mass element matrices.
    stiff_elem.clear();
    mass_elem.clear();
    tri_Jacobians.clear();
    for (int ind_triangle=0; ind_triangle < mesh->num_triangles; ind_triangle++) {
        int ind_elem = mesh->triangles[ind_triangle];
        Mesh_Element *elem = mesh->elements[ind_elem];

        double x[elem->nnodes], y[elem->nnodes], z[elem->nnodes];

        for (int ind_node=0; ind_node<elem->nnodes; ind_node++) {
            int nodenum = elem->nodes[ind_node];

            // Note that the node, element and triangle numbers used by GMsh differ by
            // one from the corresponding array location:
            x[ind_node] = mesh->nodes[(nodenum-1)*3];
            y[ind_node] = mesh->nodes[(nodenum-1)*3+1];
            z[ind_node] = mesh->nodes[(nodenum-1)*3+2];
        }

        double Je_tri[] = {x[1]-x[0], x[2]-x[0], y[1]-y[0], y[2]-y[0]};
        Matrix Je(2, 2, Je_tri);
        tri_Jacobians.push_back(Je);

        double nu = regions[elem->physical].nu;
        Matrix Se = stiff_element_analytical(x, y, nu);
        stiff_elem.push_back(Se);

        double sigma = regions[elem->physical].sigma;
        Matrix Me = mass_element_analytical(x, y, sigma);
        mass_elem.push_back(Me);
    }

    // Assembly of the global stiffness and mass matrices.
    int n = mesh->num_nodes;
    double *data_stiff = new double[n*n],
           *data_mass  = new double[n*n];

    fill(data_stiff, data_stiff + n*n, 0);
    fill(data_mass,  data_mass  + n*n, 0);

    for (int ind_triangle=0; ind_triangle < mesh->num_triangles; ind_triangle++) {
        int ind_elem = mesh->triangles[ind_triangle];
        Mesh_Element *elem = mesh->elements[ind_elem];

        Matrix Se = stiff_elem[ind_triangle];
        Matrix Me = mass_elem[ind_triangle];
        int node_row, node_col, elemnum;

        for (int ind_row = 0; ind_row < 3; ind_row++) {
            node_row = elem->nodes[ind_row];
            for (int ind_col = 0; ind_col < 3; ind_col++) {
                node_col = elem->nodes[ind_col];
                elemnum = (node_col-1) + (node_row-1)*n;
//                cout << elemnum << " " << n*n << endl;
                data_stiff[elemnum] += Se.value(ind_row, ind_col);
                data_mass[elemnum]  += Me.value(ind_row, ind_col);
            }
        }
    }


    stiff_global = new Matrix(mesh->num_nodes, mesh->num_nodes, data_stiff);
    mass_global  = new Matrix(mesh->num_nodes, mesh->num_nodes, data_mass);

    delete [] data_stiff;
    delete [] data_mass;

    // Assembly of the excitation vector.
    double *excitation_data = new double[mesh->num_nodes];
    fill(excitation_data, excitation_data+n, 0);

    for (int ind_triangle=0; ind_triangle < mesh->num_triangles; ind_triangle++) {
        int ind_elem = mesh->triangles[ind_triangle];
        Mesh_Element *elem = mesh->elements[ind_elem];
        double Js = regions[elem->physical].Js;
        double area = triangle_area(ind_triangle);

        for (int ind_node=0; ind_node<elem->nnodes; ind_node++) {
            if (ind_node < 3) {
                excitation_data[elem->nodes[ind_node]] += area*Js/3;
            }
        }
    }
    excitation_global = new Matrix(n, 1, excitation_data);
    delete [] excitation_data;

    //cout << stiff_global->cols << endl;
}

Assembly_Triangle::~Assembly_Triangle() {
    delete excitation_global;
    delete stiff_global;
    delete mass_global;
}

