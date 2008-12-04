#include <sstream>
#include <iostream>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp> 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/Registry.h"

  template<class T> void encode(std::string & rep, const T & val)
  {
    std::stringstream stst;
    boost::archive::binary_oarchive ar(stst);
    ar << val;
    rep = stst.str();
  }
 template<class T> void decode(T & val, const std::string & rep)
  {
    std::stringstream stst(rep);
    boost::archive::binary_iarchive ar(stst);
    ar >> val;
  }

int main() {
  return 0;
}
/*
void testSerialization() {
  double d = 0.5, d2 = 0.;
  std::string v;
  encode(v, d);
  decode(d2, v);
  assert(d == d2);
std::cout << "BADHASH " << edm::detail::InvalidHash() << std::endl;
   std::vector<std::string> vs;
  vs.push_back("");
  vs.push_back("1");
  encode(v, vs);
  std::vector<std::string> vs2;
  decode(vs2, v);
  assert(vs2.size() == vs.size());

  std::string s;
  std::map<std::string, int> myMap;
  encode(s, myMap);
  edm::ParameterSet ps;
  edm::ParameterSet psEmbedded;
  psEmbedded.addUntrackedParameter<std::string>("p1", "wham");
  psEmbedded.addParameter<std::string>("p2", "bam");
  edm::InputTag it("label", "instance");
  std::vector<edm::InputTag> vit;
  vit.push_back(it);
  psEmbedded.addParameter<edm::InputTag>("it", it);
  psEmbedded.addParameter<std::vector<edm::InputTag> >("vit", vit);
  ps.addParameter<edm::ParameterSet>("psEmbedded", psEmbedded);

// edm::pset::Registry* reg = pset::Registry::instance();
// edm::pset::loadAllNestedParameterSets(reg, ps);

  ps.fillID();
  edm::ParameterSetEntry entry(ps, false);
  entry.updateID();
edm::ParameterSetID id;
encode(s, id);
edm::ParameterSetID id2;
decode(id2, s);
  encode(s, entry);
std::cout << "BADHASH8 " << edm::detail::InvalidHash() << std::endl;

  edm::ParameterSetEntry entry2;
  //decode(entry2, s);
  std::stringstream stst(s);
std::cout << "BADHASH85 " << edm::detail::InvalidHash() << std::endl;
  boost::archive::binary_iarchive ar(stst);
  ar >> entry2;

  std::cout <<  "DECODED ID " << entry2.id() << std::endl;
assert( entry.pset() == entry2.pset());
std::cout << "ps\n " << ps << std::endl;

  encode(s, ps);
std::cout << "ENCODING " << ps.getParameter<edm::ParameterSet>("psEmbedded") << std::endl;

  edm::ParameterSet defrosted;
std::cout << "DECODING " << std::endl;
  decode(defrosted, s);
std::cout << "defrosted\n " << defrosted.getParameter<edm::ParameterSet>("psEmbedded") << std::endl;
  assert(ps.getParameter<edm::ParameterSet>("psEmbedded").getUntrackedParameter<std::string>("p1") == "wham");
std::cout << " COMPARE " << std::endl;
  assert(defrosted == ps);
std::cout << "SECONDFRREEZ" << std::endl;
  // encode and decode together
  edm::ParameterSet trackedPart(ps.toStringOfTracked());
std::cout << "trackedPart " << trackedPart << std::endl;
  assert(trackedPart.exists("psEmbedded"));
  assert(trackedPart.getParameter<edm::ParameterSet>("psEmbedded").exists("p2"));
  // FAILS.  Still in registry
  assert(!trackedPart.getParameter<edm::ParameterSet>("psEmbedded").exists("p1"));
  
}
*/
