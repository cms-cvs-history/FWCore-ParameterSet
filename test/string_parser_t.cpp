#include "FWCore/ParameterSet/interface/parse.h"
#include <iterator>
#include <algorithm>
#include <iostream>

//------------------------------------------------------------
// This test is now obsolete, because the configuration file parser
// always obtains its input from a string. parser_t exercises all the
// features of the parser that this test had exercised.
//------------------------------------------------------------

int main() { return 0; }

// int main(argc, char** argv)
// {
//   if (argc == 1)
//     {
//       cerr << "Must specify input to be parsed" << endl;
//       return 1;
//     }
//   for (int i = 1; i < argc; ++i) {
//     ParseResults pr = parse(argv[i]);

//     if(!pr) {
//       cerr << "Null output from parser" << endl;
//       return -1;
//     }

//     copy(pr->begin(),
//        pr->end(),
//        ostream_iterator<NodePtr>(cout,"\n"));
//   }

//   return 0;
// }
