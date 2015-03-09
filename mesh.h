#ifndef MESH_H
#define MESH_H

#include "mesh_file.h"
#include "mesh_element.h"

class Mesh {
 private:
  MeshFile *meshfile;
  vector<string> * strsplit(const string &s, char delim);
 public:
  vector <Mesh_Element*> elements;

  double * nodes;
  //unsigned int * lines;
  //unsigned int * triangles;
  vector <int> lines;
  vector <int> triangles;

  int num_elements;
  int num_nodes, num_lines, num_triangles;

  Mesh(MeshFile *meshfile_in);
  ~Mesh();
};

#endif
