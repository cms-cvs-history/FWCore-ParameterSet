// ----------------------------------------------------------------------
// $Id: ParameterSet.cc,v 1.39.2.1 2008/12/04 02:05:06 rpw Exp $
//
// definition of ParameterSet's function members
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// prerequisite source files and headers
// ----------------------------------------------------------------------

#include "FWCore/Utilities/interface/Digest.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/Registry.h"

#include "FWCore/ParameterSet/interface/split.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Utilities/interface/Algorithms.h"

#include "boost/bind.hpp"

#include <algorithm>
#include <iostream>

#include <sstream>

// ----------------------------------------------------------------------
// class invariant checker
// ----------------------------------------------------------------------

namespace edm {

  void
  ParameterSet::checkIfFrozen() const
  {
    if(frozen_)
    {
      throw edm::Exception(errors::Configuration,"InsertFailure")
        << "This ParameterSet has been frozen, so may not be changed";
    }
  }


  // ----------------------------------------------------------------------
  // constructors
  // ----------------------------------------------------------------------

  ParameterSet::ParameterSet() :
    tbl_(),
    psetTable_(),
    frozen_(false),
    id_()
  {
  }

  ParameterSet::~ParameterSet() {}

  void ParameterSet::freeze() const
  {
    // there are three steps in freezing
    if (!frozen_) {
      for(psettable::const_iterator psetItr = psetTable_.begin();
          psetItr != psetTable_.end(); ++psetItr)
      {
        psetItr->second.pset().freeze();
      }

      frozen_ = true;
    }
  }
 
  void ParameterSet::fillID() const
  {
    freeze();
    if(!id_.isValid())
    {
      calculateID();
    }
  }

  void ParameterSet::calculateID() const
  {
    // make sure contained PSets are updated
    for(psettable::const_iterator psetItr = psetTable_.begin();
        psetItr != psetTable_.end(); ++psetItr)
    {
      psetItr->second.updateID();
    }
    // tracked parts always have a correct ID, and always have their registries updated
    std::string stringrep = toString();
    cms::Digest md5alg(stringrep);
    id_ = ParameterSetID(md5alg.digest().toString());
    assert(id_.isValid());
    updateRegistry();
  }

  
  void ParameterSet::updateRegistry() const
  {
    edm::pset::Registry* reg = edm::pset::Registry::instance();
    reg->insertMapped(*this);
  }

  // ----------------------------------------------------------------------
  // identification
  ParameterSetID
  ParameterSet::id() const
  {
    // checks if frozen and valid
    fillID();
    return id_;
  }

  // ----------------------------------------------------------------------
  // coded string

  ParameterSet::ParameterSet(std::string const& code) :
    tbl_(),
    psetTable_(),
    frozen_(false),
    id_()
  {
    if(! fromString(code))
      throw edm::Exception(errors::Configuration,"InvalidInput")
	<< "The encoded configuration string "
	<< "passed to a ParameterSet during construction is invalid:\n"
	<< code;

  }



  // ----------------------------------------------------------------------
  // Entry-handling
  // ----------------------------------------------------------------------

  Entry const*
  ParameterSet::getEntryPointerOrThrow_(char const* name) const {
    return getEntryPointerOrThrow_(std::string(name));
  }

  Entry const*
  ParameterSet::getEntryPointerOrThrow_(std::string const& name) const {
    Entry const* result = retrieveUntracked(name);
    if (result == 0)
      throw edm::Exception(errors::Configuration, "MissingParameter:")
	<< "The required parameter '" << name
	<< "' was not specified.\n";
    return result;
  }

  template <class T, class U> T first(std::pair<T,U> const& p)
  { return p.first; }

  template <class T, class U> U second(std::pair<T,U> const& p)
  { return p.second; }

  Entry const&
  ParameterSet::retrieve(char const* name) const {
    return retrieve(std::string(name));
  }

  Entry const&
  ParameterSet::retrieve(std::string const& name) const {
    table::const_iterator  it = tbl_.find(name);
    if (it == tbl_.end()) {
	throw edm::Exception(errors::Configuration,"MissingParameter:")
	  << "Parameter '" << name
	  << "' not found.";
    }
    if (it->second.isTracked() == false) {
      if (name[0] == '@') {
	throw edm::Exception(errors::Configuration,"StatusMismatch:")
	  << "Framework Error:  Parameter '" << name
	  << "' is incorrectly designated as tracked in the framework.";
      } else {
	throw edm::Exception(errors::Configuration,"StatusMismatch:")
	  << "Parameter '" << name
	  << "' is designated as tracked in the code,\n"
          << "but is designated as untracked in the configuration file.\n"
          << "Please remove 'untracked' from the configuration file for parameter '"<< name << "'.";
      }
    }
    return it->second;
  }  // retrieve()

  Entry const* const
  ParameterSet::retrieveUntracked(char const* name) const {
    return retrieveUntracked(std::string(name));
  }

  Entry const* const
  ParameterSet::retrieveUntracked(std::string const& name) const {
    table::const_iterator  it = tbl_.find(name);

    if (it == tbl_.end()) return 0;
    if (it->second.isTracked()) {
      if (name[0] == '@') {
	throw edm::Exception(errors::Configuration,"StatusMismatch:")
	  << "Framework Error:  Parameter '" << name
	  << "' is incorrectly designated as untracked in the framework.";
      } else {
	throw edm::Exception(errors::Configuration,"StatusMismatch:")
	  << "Parameter '" << name
	  << "' is designated as untracked in the code,\n"
          << "but is not designated as untracked in the configuration file.\n"
          << "Please change the configuration file to 'untracked <type> " << name << "'.";
      }
    }
    return &it->second;
  }  // retrieve()


  ParameterSetEntry const&
  ParameterSet::retrieveParameterSet(std::string const& name) const {
    psettable::const_iterator it = psetTable_.find(name);
    if (it == psetTable_.end()) {
        throw edm::Exception(errors::Configuration,"MissingParameter:")
          << "ParameterSet '" << name
          << "' not found.";
    }
    if (it->second.isTracked() == false) {
      if (name[0] == '@') {
        throw edm::Exception(errors::Configuration,"StatusMismatch:")
          << "Framework Error:  ParameterSet '" << name
          << "' is incorrectly designated as tracked in the framework.";
      } else {
        throw edm::Exception(errors::Configuration,"StatusMismatch:")
          << "ParameterSet '" << name
          << "' is designated as tracked in the code,\n"
          << "but is designated as untracked in the configuration file.\n"
          << "Please remove 'untracked' from the configuration file for parameter '"<< name << "'.";
      }
    }
    return it->second;
  }  // retrieve()

  ParameterSetEntry const* const
  ParameterSet::retrieveUntrackedParameterSet(std::string const& name) const {
    psettable::const_iterator  it = psetTable_.find(name);

    if (it == psetTable_.end()) return 0;
    if (it->second.isTracked()) {
      if (name[0] == '@') {
        throw edm::Exception(errors::Configuration,"StatusMismatch:")
          << "Framework Error:  ParameterSet '" << name
          << "' is incorrectly designated as untracked in the framework.";
      } else {
        throw edm::Exception(errors::Configuration,"StatusMismatch:")
          << "ParameterSet '" << name
          << "' is designated as untracked in the code,\n"
          << "but is not designated as untracked in the configuration file.\n"
          << "Please change the configuration file to 'untracked <type> " << name << "'.";
      }
    }
    return &it->second;
  }  // retrieve()

  Entry const* const
  ParameterSet::retrieveUnknown(char const* name) const {
    return retrieveUnknown(std::string(name));
  }

  Entry const* const
  ParameterSet::retrieveUnknown(std::string const& name) const {
    table::const_iterator  it = tbl_.find(name);
    if (it == tbl_.end()) return 0;
    return &it->second;
  }

  // ----------------------------------------------------------------------

  void
  ParameterSet::insert(bool okay_to_replace, char const* name, Entry const& value) {
    insert(okay_to_replace, std::string(name), value);
  }

  void
  ParameterSet::insert(bool okay_to_replace, std::string const& name, Entry const& value) {
    // We should probably get rid of 'okay_to_replace', which will
    // simplify the logic in this function.
    table::iterator  it = tbl_.find(name);

    if(it == tbl_.end())  {
      if(! tbl_.insert(std::make_pair(name, value)).second)
        throw edm::Exception(errors::Configuration,"InsertFailure")
	  << "cannot insert " << name
	  << " into a ParameterSet\n";
    }
    else if(okay_to_replace)  {
      it->second = value;
    }
  }  // insert()


  void ParameterSet::insertParameterSet(bool okay_to_replace, std::string const& name, const ParameterSetEntry & entry) {
    // We should probably get rid of 'okay_to_replace', which will
    // simplify the logic in this function.
    psettable::iterator  it = psetTable_.find(name);

    if(it == psetTable_.end())  {
      if(! psetTable_.insert(std::make_pair(name, entry)).second)
        throw edm::Exception(errors::Configuration,"InsertFailure")
          << "cannot insert " << name
          << " into a ParameterSet\n";
    }
    else if(okay_to_replace)  {
      it->second = entry;
    }
  }  // insert()


  // ----------------------------------------------------------------------
  // copy without overwriting
  // ----------------------------------------------------------------------
  void
  ParameterSet::augment(ParameterSet const& from) {
    // This preemptive invalidation may be more agressive than necessary.
    checkIfFrozen();

    if(& from == this)
      return;

    for(table::const_iterator b = from.tbl_.begin(), e = from.tbl_.end(); b != e; ++b) {
      this->insert(false, b->first, b->second);
    }
    for(psettable::const_iterator b2 = from.psetTable_.begin(), e2 = from.psetTable_.end(); b2 != e2; ++b2) {
      this->insertParameterSet(false, b2->first, b2->second);
    }
  }  // augment()

  // ----------------------------------------------------------------------
  // coding
  // ----------------------------------------------------------------------

  std::string
  ParameterSet::toString() const {
  //edm::pset::Registry* reg = edm::pset::Registry::instance();
  //edm::pset::loadAllNestedParameterSets(reg, *this);
  /*
    std::string rep;
    std::stringstream stst;
    boost::archive::text_oarchive ar(stst);
    ar << *this;
    rep = stst.str();
    return rep;
  */
    // make sure the PSets get filled
    if(!frozen_)
    {
      fillID();
    }
    std::string rep;
    if (tbl_.empty() && psetTable_.empty()) {
      std::string emptyPSet = "<>";
      return emptyPSet;
    }
    size_t size = 1;
    for(table::const_iterator b = tbl_.begin(), e = tbl_.end(); b != e; ++b) {
      size += 2;
      size += b->first.size();
      size += b->second.sizeOfString();
    }
    for(psettable::const_iterator b = psetTable_.begin(), e = psetTable_.end(); b != e; ++b) {
      size += 2;
      size += b->first.size();
      size += b->second.sizeOfString();
    }

    rep.reserve(size);
    rep += '<';
    for(table::const_iterator b = tbl_.begin(), e = tbl_.end(); b != e; ++b) {
      if(b != tbl_.begin()) rep += ';';
      rep += b->first;
      rep += '=';
      rep += b->second.toString();
    }
    for(psettable::const_iterator b = psetTable_.begin(), e = psetTable_.end(); b != e; ++b) {
      if(b != psetTable_.begin() || !tbl_.empty()) rep += ';';
      rep += b->first;
      rep += '=';
      rep += b->second.toString();
    }

    rep += '>';
    return rep;
  }  // to_string()

  // ----------------------------------------------------------------------

  std::string
  ParameterSet::toStringOfTracked() const {
    return trackedPart().toString();
    std::string rep;
    size_t size = 0;
    bool need_sep = false;
    for(table::const_iterator b = tbl_.begin(), e = tbl_.end(); b != e; ++b) {
      if(b->second.isTracked())  {
	if(need_sep) ++size;
	++size;
	size += b->first.size();
	size += b->second.sizeOfStringOfTracked();
	need_sep = true;
      }
    }
    if (size == 0) {
      std::string emptyPSet = "<>";
      return emptyPSet;
    }
    size += 2;
    rep.reserve(size);
    rep += '<';
    need_sep = false;
    for(table::const_iterator b = tbl_.begin(), e = tbl_.end(); b != e; ++b) {
      if(b->second.isTracked())  {
	if(need_sep) rep += ';';
	rep += b->first;
	rep += '=';
	rep += b->second.toStringOfTracked();
	need_sep = true;
      }
    }
    rep += '>';    
    return rep;
  } 

  // ----------------------------------------------------------------------

  bool
  ParameterSet::fromString(std::string const& from) {
    // This preemptive invalidation may be more agressive than necessary.
    checkIfFrozen();
    /*
    std::stringstream stst(from);
    boost::archive::text_iarchive ar(stst);
    ParameterSet p;
    ar >> p;
    *this = p;
    return true;
    */

    std::vector<std::string> temp;
    if(! split(std::back_inserter(temp), from, '<', ';', '>'))
      return false;

    tbl_.clear();  // precaution
    for(std::vector<std::string>::const_iterator b = temp.begin(), e = temp.end(); b != e; ++b) {
      // locate required name/value separator
      std::string::const_iterator  q
        = find_in_all(*b, '=');
      if(q == b->end())
        return false;

      // form name unique to this ParameterSet
      std::string  name = std::string(b->begin(), q);
      if(tbl_.find(name) != tbl_.end())
        return false;

      std::string rep(q+1, b->end());
      // entries are generically of the form tracked-type-rep
      if(rep[1] == 'Q')
      {
         ParameterSetEntry psetEntry( rep );
         if(! psetTable_.insert(std::make_pair(name, psetEntry)).second)
           return false;
      }
      else if(rep[1] == 'P')
      {
        //old representation of ParameterSet, included for backwards-compatibility
        Entry value(name, rep);
        ParameterSetEntry psetEntry( value.getPSet(), value.isTracked() );
        if(! psetTable_.insert(std::make_pair(name, psetEntry)).second)
          return false;
      }

      else
      {
        // form value and insert name/value pair
        Entry  value(name, rep);
        if(! tbl_.insert(std::make_pair(name, value)).second)
          return false;
      }
    }

    return true;
  }  // from_string()

  std::vector<edm::FileInPath>::size_type
  ParameterSet::getAllFileInPaths(std::vector<edm::FileInPath>& output) const {
    std::vector<edm::FileInPath>::size_type count = 0;
    table::const_iterator it = tbl_.begin();
    table::const_iterator end = tbl_.end();
    while (it != end) {
	Entry const& e = it->second;
	if (e.typeCode() == 'F') {
	    ++count;
	    output.push_back(e.getFileInPath());
	}
	++it;
    }
    return count;
  }

  std::vector<std::string>
  ParameterSet::getParameterNames() const {
    std::vector<std::string> returnValue;
    std::transform(tbl_.begin(), tbl_.end(), back_inserter(returnValue),
		   boost::bind(&std::pair<std::string const, Entry>::first,_1));
    std::transform(psetTable_.begin(), psetTable_.end(), back_inserter(returnValue),
                   boost::bind(&std::pair<std::string const, ParameterSetEntry>::first,_1));
    return returnValue;
  }


  bool ParameterSet::exists(const std::string & parameterName) const
  {
    return( tbl_.find(parameterName) != tbl_.end() || psetTable_.find(parameterName) != psetTable_.end());
  }


  ParameterSet
  ParameterSet::trackedPart() const
  {
    ParameterSet result;
    for(table::const_iterator tblItr = tbl_.begin(); tblItr != tbl_.end(); ++tblItr)
    {
      if(tblItr->second.isTracked()) 
      {
        result.tbl_.insert(*tblItr);
      }
    }
    for(psettable::const_iterator psetItr = psetTable_.begin(); 
        psetItr != psetTable_.end(); ++psetItr)
    {
      if(psetItr->second.isTracked())
      {
        result.addParameter(psetItr->first, psetItr->second.pset().trackedPart());
        result.psetTable_[psetItr->first].updateID();
      }
    }

    return result;
  }

   size_t
   ParameterSet::getParameterSetNames(std::vector<std::string>& output)
   {
     std::transform(psetTable_.begin(), psetTable_.end(), back_inserter(output),
                   boost::bind(&std::pair<std::string const, ParameterSetEntry>::first,_1));
     return output.size();
   }

   size_t
   ParameterSet::getParameterSetNames(std::vector<std::string>& output,
                                      bool trackiness) const
   {
     for(psettable::const_iterator psetItr = psetTable_.begin();
         psetItr != psetTable_.end(); ++psetItr)
     {
       if(psetItr->second.isTracked() == trackiness)
       {
         output.push_back(psetItr->first);
       }
     }
     return output.size();
   }

   size_t
   ParameterSet::getParameterSetVectorNames(std::vector<std::string>& output,
					    bool trackiness) const
   {
     return getNamesByCode_('p', trackiness, output);
   }

  size_t
  ParameterSet::getNamesByCode_(char code,
				bool trackiness,
				std::vector<std::string>& output) const
  {
    size_t count = 0;
    table::const_iterator it = tbl_.begin();
    table::const_iterator end = tbl_.end();
    while ( it != end )
    {
      Entry const& e = it->second;
      if (e.typeCode() == code &&
	  e.isTracked() == trackiness) // if it is a vector of ParameterSet
	{
	  ++count;
	  output.push_back(it->first); // save the name
	}
      ++it;
    }
    return count;

  }


  bool operator==(ParameterSet const& a, ParameterSet const& b) {
    // Maybe can replace this with comparison of id_ values.
    return a.toStringOfTracked() == b.toStringOfTracked();
  }


  std::string ParameterSet::dump() const
  {
    std::ostringstream os;
    table::const_iterator i(tbl_.begin()), e(tbl_.end());
    os << "{" << std::endl;
    for( ; i != e; ++i)
    {
      // indent a bit
      os << "  " << i->first << ": " << i->second << std::endl;
    }
    os << "}";
    psettable::const_iterator j(psetTable_.begin()), e2(psetTable_.end());
    os << "{" << std::endl;
    for( ; j != e2; ++j)
    {
      // indent a bit
      std::string n = j->first;
      const ParameterSetEntry & pe = j->second;
      os << "  " << n << ": " << pe <<  std::endl;
    }
    os << "}";
    return os.str();
  }


  std::ostream & operator<<(std::ostream & os, const ParameterSet & pset)
  {
    os << pset.dump();
    return os;
  }

  
    
  namespace pset
  {
    void explode(edm::ParameterSet const& top,
	       std::vector<edm::ParameterSet>& results)
    {
      using namespace edm;
      results.push_back(top);

      // Get names of all ParameterSets; iterate through them,
      // recursively calling explode...
        
      std::vector<std::string> names;
      const bool tracked = true;
      const bool untracked = false;
      top.getParameterSetNames(names);
      std::vector<std::string>::const_iterator it = names.begin();
      std::vector<std::string>::const_iterator end = names.end();
      for( ; it != end; ++it )
      {
	ParameterSet next_top =
	  top.getParameter<ParameterSet>(*it);
	explode(next_top, results);
      }

 
      // Get names of all ParameterSets vectors; iterate through them,
      // recursively calling explode...
      names.clear();
      top.getParameterSetVectorNames(names, tracked);
      it = names.begin();
      end = names.end();
      for( ; it != end; ++it )
      {
	std::vector<ParameterSet> next_bunch =
	  top.getParameter<std::vector<ParameterSet> >(*it);

	std::vector<ParameterSet>::const_iterator first =
	  next_bunch.begin();
	std::vector<ParameterSet>::const_iterator last
	  = next_bunch.end();

	for( ; first != last; ++first )
	{
	    explode(*first, results);
        }	
      }

      names.clear();
      top.getParameterSetVectorNames(names, untracked);
      it = names.begin();
      end = names.end();
      for( ; it != end; ++it )
      {
	std::vector<ParameterSet> next_bunch =
	  top.getUntrackedParameter<std::vector<ParameterSet> >(*it);

	std::vector<ParameterSet>::const_iterator first =
	  next_bunch.begin();
	std::vector<ParameterSet>::const_iterator last
	  = next_bunch.end();

	for( ; first != last; ++first )
	{
	    explode(*first, results);
	}	
      }
    }
  }

  // Free function to return a parameterSet given its ID.
  ParameterSet
  getParameterSet(ParameterSetID const& id) {
    ParameterSet result;
    if(!pset::Registry::instance()->getMapped(id, result)) {
        throw edm::Exception(errors::Configuration,"MissingParameterSet:")
          << "Parameter Set ID '" << id
          << "' not found.";
    }
    return result;
  }
  void ParameterSet::depricatedInputTagWarning(std::string const& name, 
					       std::string const& label) const
  {
    edm::LogWarning("Configuration") << "Warning:\n\tstring " << name 
				     << " = \"" << label 
				     << "\"\nis deprecated, "
				     << "please update your config file to use\n\tInputTag " 
				     << name << " = " << label;
  }
} // namespace edm
