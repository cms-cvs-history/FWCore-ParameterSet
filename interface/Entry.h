#ifndef FWCore_ParameterSet_Entry_h
#define FWCore_ParameterSet_Entry_h

// ----------------------------------------------------------------------
// interface to edm::Entry and related types
//
//
// The functions here are expected to go away.  The exception
// processing is not ideal and is not a good model to follow.
//
// ----------------------------------------------------------------------


#include <string>
#include <vector>
#include <iosfwd>

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/ESInputTag.h"
//@@ not needed, but there might be trouble if we take it out
#include "DataFormats/Provenance/interface/EventRange.h"
#include "DataFormats/Provenance/interface/LuminosityBlockID.h"
#include "DataFormats/Provenance/interface/LuminosityBlockRange.h"
#include "DataFormats/Provenance/interface/MinimalEventID.h"
#include <boost/cstdint.hpp>

// ----------------------------------------------------------------------
// contents

namespace edm {
  // forward declarations:
  class ParameterSet;

  // ----------------------------------------------------------------------
  // Entry
  
  class Entry 
  {
  public:
    // Bool
    Entry(std::string const& name, bool val, bool is_tracked);
    bool  getBool() const;
  
    // Int32
    Entry(std::string const& name, int val, bool is_tracked);
    int  getInt32() const;
  
    // vInt32
    Entry(std::string const& name, std::vector<int> const& val, bool is_tracked);
    std::vector<int>  getVInt32() const;
  
    // Uint32
    Entry(std::string const& name, unsigned val, bool is_tracked);
    unsigned  getUInt32() const;
  
    // vUint32
    Entry(std::string const& name, std::vector<unsigned> const& val, bool is_tracked);
    std::vector<unsigned>  getVUInt32() const;
  
    // Int64
    Entry(std::string const& name, long long val, bool is_tracked);
    long long  getInt64() const;

    // vInt64
    Entry(std::string const& name, std::vector<long long> const& val, bool is_tracked);
    std::vector<long long>  getVInt64() const;

    // Uint64
    Entry(std::string const& name, unsigned long long val, bool is_tracked);
    unsigned long long  getUInt64() const;

    // vUint64
    Entry(std::string const& name, std::vector<unsigned long long> const& val, bool is_tracked);
    std::vector<unsigned long long>  getVUInt64() const;

    // Double
    Entry(std::string const& name, double val, bool is_tracked);
    double getDouble() const;
  
    // vDouble
    Entry(std::string const& name, std::vector<double> const& val, bool is_tracked);
    std::vector<double> getVDouble() const;
  
    // String
    Entry(std::string const& name, std::string const& val, bool is_tracked);
    std::string getString() const;
  
    // vString
    Entry(std::string const& name, std::vector<std::string> const& val, bool is_tracked);
    std::vector<std::string>  getVString() const;

    // FileInPath
    Entry(std::string const& name, edm::FileInPath const& val, bool is_tracked);
    edm::FileInPath getFileInPath() const;
  
    // InputTag
    Entry(std::string const& name, edm::InputTag const & tag, bool is_tracked);
    edm::InputTag getInputTag() const;

    // InputTag
    Entry(std::string const& name, std::vector<edm::InputTag> const & vtag, bool is_tracked);
    std::vector<edm::InputTag> getVInputTag() const;

    // ESInputTag
    Entry(std::string const& name, edm::ESInputTag const & tag, bool is_tracked);
    edm::ESInputTag getESInputTag() const;

    // VESInputTag
    Entry(std::string const& name, std::vector<edm::ESInputTag> const & vtag, bool is_tracked);
    std::vector<edm::ESInputTag> getVESInputTag() const;

    // EventID
    Entry(std::string const& name, edm::MinimalEventID const & tag, bool is_tracked);
    edm::MinimalEventID getEventID() const;

    // VEventID
    Entry(std::string const& name, std::vector<edm::MinimalEventID> const & vtag, bool is_tracked);
    std::vector<edm::MinimalEventID> getVEventID() const;

    // LuminosityBlockID
    Entry(std::string const& name, edm::LuminosityBlockID const & tag, bool is_tracked);
    edm::LuminosityBlockID getLuminosityBlockID() const;

    // VLuminosityBlockID
    Entry(std::string const& name, std::vector<edm::LuminosityBlockID> const & vtag, bool is_tracked);
    std::vector<edm::LuminosityBlockID> getVLuminosityBlockID() const;

    // LuminosityBlockRange
    Entry(std::string const& name, edm::LuminosityBlockRange const & tag, bool is_tracked);
    edm::LuminosityBlockRange getLuminosityBlockRange() const;

    // VLuminosityBlockRange
    Entry(std::string const& name, std::vector<edm::LuminosityBlockRange> const & vtag, bool is_tracked);
    std::vector<edm::LuminosityBlockRange> getVLuminosityBlockRange() const;

    // EventRange
    Entry(std::string const& name, edm::EventRange const & tag, bool is_tracked);
    edm::EventRange getEventRange() const;

    // VEventRange
    Entry(std::string const& name, std::vector<edm::EventRange> const & vtag, bool is_tracked);
    std::vector<edm::EventRange> getVEventRange() const;

    // ParameterSet
    Entry(std::string const& name, ParameterSet const& val, bool is_tracked);
    ParameterSet getPSet() const;
  
    // vPSet
    Entry(std::string const& name, std::vector<ParameterSet> const& val, bool is_tracked);
  
    std::vector<ParameterSet>  getVPSet() const;
  
    // coded string
    Entry(std::string const& name, std::string const&);
    Entry(std::string const& name, std::string const& type, 
          std::string const& value, bool is_tracked);
    Entry(std::string const& name, std::string const& type, 
          std::vector<std::string> const& value, bool is_tracked);
    
    ~Entry();
    // encode

    std::string toString() const;
    void toString(std::string& result) const;

    size_t sizeOfString() const {return rep.size() + 4;}
  
    // access
    bool isTracked() const { return tracked == '+'; }

    char typeCode() const { return type; }

    friend std::ostream& operator<<(std::ostream& ost, const Entry & entry);

  private:
    std::string name_;
    std::string  rep;
    char         type;
    char         tracked;
  
    // verify class invariant
    void validate() const;
  
    // decode
    bool fromString(std::string::const_iterator b, std::string::const_iterator e);

    // helpers to throw exceptions
    void throwValueError(const char* expectedType) const;
    void throwEntryError(const char* expectedType,std::string const& badRep) const;
    void throwEncodeError(const char* type) const;

  };  // Entry


  inline bool
  operator==(Entry const& a, Entry const& b) {
    return a.toString() == b.toString();
  }
  
  inline bool
  operator!=(Entry const& a, Entry const& b) {
    return !(a == b);
  }
} // namespace edm

  
#endif
