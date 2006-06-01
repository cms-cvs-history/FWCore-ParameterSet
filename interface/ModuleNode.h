#ifndef ParameterSet_ModuleNode_h
#define ParameterSet_ModuleNode_h

#include "FWCore/ParameterSet/interface/CompositeNode.h"

namespace edm {
  namespace pset {


    /*
      -----------------------------------------
      Modules hold: source (named/unnamed), modules
    */

    struct ModuleNode : public CompositeNode
    {
      ModuleNode(const std::string& type, const std::string& instname,
                 const std::string& classname,
                 NodePtrListPtr nl,int line=-1);
      virtual Node * clone() const { return new ModuleNode(*this);}
      virtual std::string type() const;
      virtual void print(std::ostream& ost) const;

      virtual void accept(Visitor& v) const;
      virtual void replaceWith(const ReplaceNode * replaceNode);

      /// create an Entry for a ParameterSet representing this module
      virtual edm::Entry makeEntry() const;

      /// inserts (a secsource) into an existing parameterSet
      virtual void insertInto(ParameterSet & pset) const;

      /// Insert into the top level of the tree
      virtual void insertInto(edm::ProcessDesc & procDesc) const;


      std::string type_;
      std::string class_;
    };

  }
}

#endif
