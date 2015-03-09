#include "mesh_file.h"

using namespace std;

MeshFile::MeshFile(char *filestr) {
  filename = string(filestr);
  inputfile = new ifstream(filename.c_str(), ios_base::in);

  string line;
  strlist.clear();
  while(getline(*inputfile, line, '\n')) {
    strlist.push_back(line);
  }

  int    field_ind = 0;
  bool   field_on  = false;
  string field_name;
  vector <string> field_data;
  assert(strlist.size() > 1);

  for (int ind_line=0; ind_line < strlist.size(); ind_line++) {
    line = strlist[ind_line];

    if (line[0] == '$') {
      if (field_on) {
        // End of a field
        assert(line.length() > 4);
        field_on = false;
        fields.push_back(field_data);
        field_names.push_back(field_name);
        field_ind++;
      } else {
        // Start of a field
        field_on = true;
        assert(line.length() > 1);
        field_name = line.substr(1);
        field_data.clear();
      }
    } else {
      field_data.push_back(line);
      assert(field_on);
    }
  }
  num_fields = field_ind;
}

MeshFile::~MeshFile() {
  delete inputfile;
}

#ifdef MESH_FILE_TEST

int
main(int argc, char **argv) {
  char filename[] = "foo.msh";

  // Test for leakage.
  for (int ind=0; ind<1000; ind++) {
      cout << ind << endl;
      MeshFile meshfile(filename);
  }
}

#endif
