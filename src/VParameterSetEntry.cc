#include "FWCore/ParameterSet/interface/VParameterSetEntry.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/split.h"
#include <sstream>
#include <iostream>
namespace edm {

  VParameterSetEntry::VParameterSetEntry()
  : tracked(false),
    theVPSet(),
    thePSetEntries()
  {
  }

  VParameterSetEntry::VParameterSetEntry(std::vector<ParameterSet> const& vpset, bool isTracked)
  : tracked(isTracked),
    theVPSet(),
    thePSetEntries()
  {
    for (std::vector<ParameterSet>::const_iterator i = vpset.begin(), e = vpset.end(); i != e; ++i) {
      thePSetEntries.push_back(ParameterSetEntry(*i, isTracked));
    }
  }

  VParameterSetEntry::VParameterSetEntry(std::string const& rep)
  : tracked(rep[0] == '+'),
    theVPSet(),
    thePSetEntries()
  {
    assert(rep[0] == '+' || rep[0] == '-');
    std::vector<std::string> temp;
    split(std::back_inserter(temp), rep, '{', ',', '}');
    for (std::vector<std::string>::const_iterator i = temp.begin(), e = temp.end(); i != e; ++i) {
      thePSetEntries.push_back(ParameterSetEntry(*i));
    }
  }
    
  VParameterSetEntry::~VParameterSetEntry() {}

  void VParameterSetEntry::updateIDs() const {
    for (std::vector<ParameterSetEntry>::const_iterator i = psetEntries().begin(), e = psetEntries().end();
        i != e; ++i) {
      i->updateID();
    }
    // clear theVPSet
    value_ptr<VParameterSet> empty;
    swap(theVPSet, empty);
  }

  std::string VParameterSetEntry::toString() const {
    std::string result = tracked ? "+q" : "-q";
    std::stringstream str;
    str << '{';
    std::string start;
    std::string const between(",");
    for (std::vector<ParameterSetEntry>::const_iterator i = thePSetEntries.begin(), e = thePSetEntries.end(); i != e; ++i) {
      str << start << i->toString();
      start = between;
    }
    str << '}';
    result += str.str();
    return result;
  }
  
  int VParameterSetEntry::sizeOfString() const {
    return toString().size();
  }

  std::vector<ParameterSet>& VParameterSetEntry::vpset() {
    if (!theVPSet) {
      theVPSet = value_ptr<std::vector<ParameterSet> >(new std::vector<ParameterSet>);
      theVPSet->reserve(thePSetEntries.size());
      for (std::vector<ParameterSetEntry>::const_iterator i = thePSetEntries.begin(), e = thePSetEntries.end(); i != e; ++i) {
        theVPSet->push_back(i->pset());
      }
    }
    return *theVPSet;
  }

  std::vector<ParameterSet> const& VParameterSetEntry::vpset() const {
    if (!theVPSet) {
      theVPSet = value_ptr<std::vector<ParameterSet> >(new std::vector<ParameterSet>);
      theVPSet->reserve(thePSetEntries.size());
      for (std::vector<ParameterSetEntry>::const_iterator i = thePSetEntries.begin(), e = thePSetEntries.end(); i != e; ++i) {
        theVPSet->push_back(i->pset());
      }
    }
    return *theVPSet;
  }

  std::ostream & operator<<(std::ostream & os, VParameterSetEntry const& vpsetEntry) {
    std::vector<ParameterSet> const& vps = vpsetEntry.vpset();
    os << "{" << std::endl;
    std::string start;
    std::string const between(",\n");
    for(std::vector<ParameterSet>::const_iterator i = vps.begin(), e = vps.end(); i != e; ++i) {
      os << start << *i;
      start = between;
    }
    if (!vps.empty()) {
      os << std::endl;
    }
    os << "}";
    return os;
  }
}


