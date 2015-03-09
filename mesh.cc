#include "mesh.h"

vector<string> *
Mesh::strsplit(const string &s, char delim) {
  stringstream ss(s);
  string item;

  vector<string> *elems;
  elems = new vector<string>;

  while (getline(ss, item, delim)) {
    elems->push_back(item);
  }
  return elems;
}

Mesh::Mesh(MeshFile *meshfile_in) {
  meshfile = meshfile_in;

  assert(meshfile->num_fields == 3);
  assert(meshfile->field_names[0] == "MeshFormat");
  assert(meshfile->field_names[1] == "Nodes");
  assert(meshfile->field_names[2] == "Elements");

  vector <string> *nodefield    = &meshfile->fields[1];

  num_nodes = nodefield->size()-1;
  assert(num_nodes > 0);
  nodes = new double[num_nodes*3];

  string nodestr = (*nodefield)[0];
  assert(num_nodes == atoi(nodestr.c_str()));

  for (int ind_node=0;ind_node < num_nodes; ind_node++) {
    string line;
    line = (*nodefield)[ind_node+1];
    
    vector <string> *node_strlist = strsplit(line, ' ');
    assert(node_strlist->size() == 4);

    double x = atof((*node_strlist)[1].c_str());
    double y = atof((*node_strlist)[2].c_str());
    double z = atof((*node_strlist)[3].c_str());

    //cout << "'" << line << "'" << endl;
    //cout << node_strlist->size() << endl;
    //cout << x << "," << y << "," << z << endl;

    nodes[ind_node*3 + 0] = x;
    nodes[ind_node*3 + 1] = y;
    nodes[ind_node*3 + 2] = z;
    delete node_strlist;
  }

  vector <string> *elementfield = &meshfile->fields[2];

  num_elements = elementfield->size()-1;
  assert(num_elements > 0);
  string elemstr = (*elementfield)[0];
  assert(num_elements == atoi(elemstr.c_str()));

  elements.clear();
  lines.clear();
  triangles.clear();
  num_lines = 0;
  num_triangles = 0;
  for (int ind_element=0;ind_element < num_elements; ind_element++) {
    string elementstr;
    elementstr = (*elementfield)[ind_element+1];

    Mesh_Element *elem = new Mesh_Element(elementstr);
    //    elem->disp();
    elements.push_back(elem);

    if (elem->type == ELEMENT_TRIANGLE3 || elem->type == ELEMENT_TRIANGLE6) {
      num_triangles++;
      triangles.push_back(ind_element);
    }
    if (elem->type == ELEMENT_LINE2 || elem->type == ELEMENT_LINE3) {
      num_lines++;
      lines.push_back(ind_element);
    }
  }
}

Mesh::~Mesh() {
  delete nodes;
  for (int ind_element=0;ind_element < num_elements; ind_element++) {
    delete elements[ind_element];
  }

}

#ifdef MESH_TEST

int
main(int argc, char **argv) {
//  char filename[] = "srm_rotor3.msh";
  char filename[] = "foo.msh";

  MeshFile meshfile(filename);

  for (int ind=0; ind < 1000; ind ++){
      cout << ind << endl;
      Mesh mesh(&meshfile);
  }


  for (int ind_field = 0; ind_field < meshfile.num_fields; ind_field++) {
    // cout << "Field " << ind_field << " '" 
    //	 << meshfile.field_names[ind_field] << "'" << endl;

    vector<string> *fielddata = &meshfile.fields[ind_field];
      
  }
}

#endif
