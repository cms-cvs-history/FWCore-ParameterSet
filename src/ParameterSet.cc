// ----------------------------------------------------------------------
// $Id: ParameterSet.cc,v 1.12 2005/11/11 19:57:58 paterno Exp $
//
// definition of ParameterSet's function members
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// prerequisite source files and headers
// ----------------------------------------------------------------------

#include "SealZip/MD5Digest.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ParameterSet/interface/split.h"
#include "FWCore/ParameterSet/interface/types.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "boost/bind.hpp"

#include <algorithm>
#include <utility>

// ----------------------------------------------------------------------
// class invariant checker
// ----------------------------------------------------------------------

namespace edm {

  void
  ParameterSet::validate() const 
  {
    std::string stringrep = this->toStringOfTracked();
    seal::MD5Digest md5alg;
    md5alg.update(stringrep.data(), stringrep.size());
    id_ = ParameterSetID(md5alg.format());
  }  // ParameterSet::validate()

  void
  ParameterSet::invalidate() const
  {
    id_ = ParameterSetID();
  }
  
  
  // ----------------------------------------------------------------------
  // constructors
  // ----------------------------------------------------------------------

  ParameterSet::ParameterSet() :
    tbl_(),
    id_()
  {
    validate();
  }


  // ----------------------------------------------------------------------
  // identification
  ParameterSetID
  ParameterSet::id() const
  {
    if (!id_.isValid()) validate();
    return id_;
  }
  
  // ----------------------------------------------------------------------
  // coded string
  
  ParameterSet::ParameterSet(std::string const& code) : 
    tbl_(),
    id_()
  {
    if(! fromString(code))
      throw edm::Exception(errors::Configuration,"InvalidInput")
	<< "The encoded configuration string "
	<< "passed to a ParameterSet during construction is invalid:\n"
	<< code;

    validate();
  }


  
  // ----------------------------------------------------------------------
  // Entry-handling
  // ----------------------------------------------------------------------

  Entry const*
  ParameterSet::getEntryPointerOrThrow_(std::string const& name) const {
    Entry const* result = retrieveUntracked(name);
    if (result == 0) 
      throw edm::Exception(errors::Configuration, "MissingParameter:")
	<< "The required parameter '" << name
	<< "' was not specified.\n";
    return result;
  }



  template <class T, class U> T first(std::pair<T,U> const& p) { return p.first; }
  template <class T, class U> U second(std::pair<T,U> const& p) { return p.second; }
  
  Entry const&
  ParameterSet::retrieve(std::string const& name) const {
    table::const_iterator  it = tbl_.find(name);
    if(it == tbl_.end()) {
      it = tbl_.find("label");
      if(it == tbl_.end())
        throw edm::Exception(errors::Configuration,"MissingParameter:")
	  << "Parameter '" << name
	  << "' not found.\n";
      else
        throw edm::Exception(errors::Configuration,"MissingParameter:")
	  << "Parameter '" << name
	  << "' specified in ParameterSet '"
	  << it->second.getString() << "' not found.\n";
    }
    return it->second;
  }  // retrieve()
  
  Entry const* const
  ParameterSet::retrieveUntracked(std::string const& name) const {
    table::const_iterator  it = tbl_.find(name);
    if(it == tbl_.end())  {
      return 0;
    }
    return &it->second;
  }  // retrieve()
  
  // ----------------------------------------------------------------------
  
  void
  ParameterSet::insert(bool okay_to_replace, std::string const& name, Entry const& value) 
  {
    // This preemptive invalidation may be more agressive than necessary.
    invalidate();

    // We should probably get rid of 'okay_to_replace', which will
    // simplify the logic in this function.
    table::iterator  it = tbl_.find(name);

    if(it == tbl_.end())  {
      if(! tbl_.insert(std::make_pair(name, value)).second)
        throw edm::Exception(errors::Configuration,"InsertFailure")
	  << "cannot insert " << name
	  << " into a ParmeterSet\n";
    }
    else if(okay_to_replace)  {
      it->second = value;
    }
  }  // insert()
  
  // ----------------------------------------------------------------------
  // copy without overwriting
  // ----------------------------------------------------------------------
  
  void
  ParameterSet::augment(ParameterSet const& from) {
    // This preemptive invalidation may be more agressive than necessary.
    invalidate();

    if(& from == this)
      return;
  
    for(table::const_iterator b = from.tbl_.begin(), e = from.tbl_.end(); b != e; ++b) {
      this->insert(false, b->first, b->second);
    }
  }  // augment()
  
  // ----------------------------------------------------------------------
  // coding
  // ----------------------------------------------------------------------
  
  std::string
  ParameterSet::toString() const {
    std::string rep;
    for(table::const_iterator b = tbl_.begin(), e = tbl_.end(); b != e; ++b) {
      if(b != tbl_.begin())
        rep += ';';
      rep += (b->first + '=' + b->second.toString());
    }
  
    return '<' + rep + '>';
  }  // to_string()
  
  // ----------------------------------------------------------------------
  
  std::string
  ParameterSet::toStringOfTracked() const {
    std::string  rep = "<";
    bool need_sep = false;
    for(table::const_iterator b = tbl_.begin(), e = tbl_.end(); b != e; ++b) {
      if(b->second.isTracked())  {
        if(need_sep)
          rep += ';';
        rep += (b->first + '=' + b->second.toString());
        need_sep = true;
      }
    }
  
    return rep + '>';
  }  // to_string()
  
  // ----------------------------------------------------------------------
  
  bool
  ParameterSet::fromString(std::string const& from) {
    // This preemptive invalidation may be more agressive than necessary.
    invalidate();

    std::vector<std::string> temp;
    if(! split(std::back_inserter(temp), from, '<', ';', '>'))
      return false;
  
    tbl_.clear();  // precaution
    for(std::vector<std::string>::const_iterator b = temp.begin(), e = temp.end(); b != e; ++b) {
      // locate required name/value separator
      std::string::const_iterator  q
        = std::find(b->begin(), b->end(), '=');
      if(q == b->end())
        return false;
  
      // form name unique to this ParameterSet
      std::string  name = std::string(b->begin(), q);
      if(tbl_.find(name) != tbl_.end())
        return false;
  
      // form value and insert name/value pair
      Entry  value(std::string(q+1, b->end()));
      if(! tbl_.insert(std::make_pair(name, value)).second)
        return false;
    }
  
    return true;
  }  // from_string()

  std::vector<edm::FileInPath>::size_type
  ParameterSet::getAllFileInPaths(std::vector<edm::FileInPath>& output) const
  {
    std::vector<edm::FileInPath>::size_type count = 0;
    table::const_iterator it = tbl_.begin();
    table::const_iterator end = tbl_.end();
    while ( it != end )
      {
	const Entry& e = it->second;
	if ( e.typeCode() == 'F' ) 
	  {
	    ++count;
	    output.push_back(e.getFileInPath());
	  }
	++it;
      }
    return count;
  }

   std::vector<std::string>
   ParameterSet::getParameterNames() const {
      std::vector<std::string> returnValue(tbl_.size());
      std::transform(tbl_.begin(), tbl_.end(),returnValue.begin(),
                     boost::bind(&std::pair<const std::string, Entry>::first,_1));
      return returnValue;
   }
  
} // namespace edm
// ----------------------------------------------------------------------