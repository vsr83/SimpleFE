#include "mesh_element.h"

vector<string> *
Mesh_Element::strsplit(const string &s, char delim) {
  stringstream ss(s);
  string item;

  vector<string> *elems;
  elems = new vector<string>;

  while (getline(ss, item, delim)) {
    elems->push_back(item);
  }
  return elems;
}

Mesh_Element::Mesh_Element(string &line) {
  vector <string> *wordlist;
  wordlist = strsplit(line, ' ');
  
  list_raw.clear();
  for (int ind_word=0;ind_word<wordlist->size();ind_word++) {
    string word = (*wordlist)[ind_word];
    int word_int = atoi(word.c_str());

    list_raw.push_back(word_int);
  }
  delete wordlist;

  assert(list_raw.size() > 2);
  number = list_raw[0];
  type   = list_raw[1];
  ntags  = list_raw[2];

  //cout << ntags << " " << list_raw.size() << endl;

  assert(type > 0 && type < sizeof(ElementData)/sizeof(elementdata));
  assert(list_raw.size() >= 3+ntags);
  assert(ntags >= 2);

  tags = new int[ntags];
  for (int ind_tag=0;ind_tag<ntags;ind_tag++) {
    tags[ind_tag] = list_raw[3+ind_tag];
  }
  physical = tags[0];
  geometrical = tags[1];

  nnodes = list_raw.size() - 3 - ntags;
  assert(nnodes > 0 && nnodes == ElementData[type].num_nodes);
  nodes = new int[nnodes];

  for (int ind_node=0;ind_node<nnodes;ind_node++) {
    nodes[ind_node] = list_raw[3+ntags+ind_node];
  }
}

Mesh_Element::~Mesh_Element() {
  delete [] tags;
  delete [] nodes;
}

void
Mesh_Element::disp() {
  cout << ElementData[type].name << " ";

  cout << "no. " << number << endl;
  cout << "Physical " << physical << endl 
       << "Geometrical " << geometrical << endl
       << "Number of tags " << ntags << endl
       << "Tags ";

  for (int ind_tag = 0; ind_tag < ntags; ind_tag++) {
    cout << tags[ind_tag] << " ";
  }
  cout << endl << "Number of nodes " << nnodes << endl
       << "Nodes ";
  for (int ind_node = 0; ind_node < nnodes; ind_node++) {
    cout << nodes[ind_node] << " ";
  }
  cout << endl;
  cout << "Hierarchical (nodes/edges/faces/volumes) "
       << ElementData[type].hnode << "/"
       << ElementData[type].hedges << "/"
       << ElementData[type].hfaces << "/"
       << ElementData[type].hvolumes << endl;
}

#ifdef MESH_ELEMENT_TEST

int
main(int argc, char **argv) {
  string testline = string("27568 9 2 2071 2070 584 585 50303 1727 54794 56804");
  Mesh_Element me(testline);
  me.disp();

  cout << sizeof(ElementData)/sizeof(struct elementdata) << endl;
}
#endif
