#include <sstream>
#include <iostream>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


  template<class T> void encode(std::string & rep, const T & val)
  {
    std::stringstream stst;
    boost::archive::text_oarchive ar(stst);
    ar << val;
    rep = stst.str();
  }
 template<class T> void decode(T & val, const std::string & rep)
  {
    std::stringstream stst(rep);
    boost::archive::text_iarchive ar(stst);
    ar >> val;
  }


int main() {
  double d = 0.5, d2 = 0.;
  std::string v;
  encode(v, d);
  decode(d2, v);
  std::cout << d << std::endl;

   std::vector<std::string> vs;
  vs.push_back("");
  vs.push_back("1");
  encode(v, vs);
  std::vector<std::string> vs2;
  decode(vs2, v);
  std::cout << "NV2 " << vs2.size() <<std::endl;
}

