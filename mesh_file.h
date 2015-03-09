#ifndef MESH_FILE_H
#define MESH_FILE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
//#include <sstream>
#include <assert.h>

using namespace std;

class MeshFile {
 private:
  vector<string> strlist;
  ifstream *inputfile;
 public:
  int num_fields;

  vector< vector<string> > fields;
  vector<string> field_names;

  string version;
  string filename;

  MeshFile(char *filestr);
  ~MeshFile();
};

#endif
