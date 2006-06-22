#ifndef ParameterSet_Node_h
#define ParameterSet_Node_h

#include <ostream>
#include <list>
#include <map>
#include <vector>
#include <string>
#include "boost/shared_ptr.hpp"

namespace edm {

  class Entry;
  class ParameterSet;
  class ProcessDesc;

  namespace pset {

    struct Visitor;
    struct ReplaceNode;

    // Base type for all nodes.  All nodes have a type associated
    // with them - this is basically the keyword that caused the
    // generation of this node.  All nodes have a name - this is the
    // name assigned to the entity

    struct Node
    {
      Node(std::string const& n, int li);

      /// needed for deep copies
      virtual Node * clone() const = 0;

      typedef boost::shared_ptr<Node> Ptr;

      virtual std::string type() const = 0;

      virtual void  setParent(Node*  parent) { parent_ = parent;}
      virtual Node* getParent() { return parent_; }
      /// leaf nodes won't do anything
      virtual void setAsChildrensParent() {}
      /// Shows where this was included from
      /// default passes up to parent
      virtual void printTrace(std::ostream& ost) const;

      virtual void print(std::ostream& ost) const = 0;
      virtual ~Node();
      virtual void accept(Visitor& v) const = 0;

      virtual void setModified(bool value);
      /// throw an exception if this node is flagged as modified
      void assertNotModified() const;
      virtual bool isModified() const {return modified_;}
      /// throws an exception if they're not the same type
      virtual void replaceWith(const ReplaceNode * replaceNode);

      typedef std::map<std::string, Ptr> NodeMap;
      /// most subclasses won't do anything
      virtual void resolve(std::list<std::string> & openFiles, 
                           std::list<std::string> & sameLevelIncludes) {}
      virtual void resolveUsingNodes(const NodeMap & blocks) {}

      /// Nodes which can exist on the top level of the
      /// parse tree should implement this
      virtual void insertInto(ProcessDesc & procDesc) const;
      /// Nodes which can be inserted into ParameterSets
      /// which aren't top-level processes should overload this.
      virtual void insertInto(edm::ParameterSet & pset) const;
      /// makes a ParameterSet Entry for this Node
      virtual edm::Entry makeEntry() const;


      std::string name;
      int         line;
      // nodes can only be modified once, so the config files can be order-independent
      bool modified_;
      Node * parent_;
    };

    typedef boost::shared_ptr<Node>        NodePtr;
    typedef std::vector<std::string>       StringList;
    typedef boost::shared_ptr<StringList>  StringListPtr;
    typedef std::list<NodePtr>             NodePtrList;
    typedef boost::shared_ptr<NodePtrList> NodePtrListPtr;
    typedef NodePtrListPtr                 ParseResults;

    inline std::ostream& operator<<(std::ostream& ost, NodePtr p)
    {
      p->print(ost);
      return ost;
    }

    inline std::ostream& operator<<(std::ostream& ost, const Node& p)
    {
      p.print(ost);
      return ost;
    }

  }
}

#endif

