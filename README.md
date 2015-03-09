This is a quick implementation of a very simple and self-contained FE solver. 

Mesh files generated with GMsh are imported with mesh.cc, mesh_element.cc,
mesh_file.cc and mesh.cc. Material parameters are specified with Region-
objects and assembled into a map according to the "physical numbers" 
(see region.cc and region.h). The element stiffness and mass matrices for 
first order basis functions are computed with analytical formulas and 
assembled into global stiffness and mass matrices in assembly_triangle.cc. 

A simple solver based on the use of O(n^3) LU decomposition for dense matrices
is implemented in matrix.cc. Application of this code is feasible only to 
small problems with significantly less than 10k degrees of freedom.

The code is applied to two simple problems in example_wire.cc and 
example_valve.cc.