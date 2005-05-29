
#include <iostream>

#include "Nodes.hh"

using namespace std;

int main()
{
  NodePtrListPtr value(new NodePtrList);
  NodePtr entry(new EntryNode("int32","i","2",false));
  value->push_back(entry);
  NodePtr pset(new PSetNode("PSet","",value));
  NodePtrListPtr npl(new NodePtrList);
  npl->push_back(pset);
  VPSetNode vp("VPSet","",npl);

  cout << vp << endl;

  return 0;
}
