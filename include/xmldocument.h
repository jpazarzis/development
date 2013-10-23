// xmldocument.h
//
// Author        : John Pazarzis
// Creation date : Tue 22 Oct 2013 10:13:36 PM EDT
//
// Summary
//      A simple and lightweight Xml parser expressed in as an object oriented
//      tree of nodes of type XmlNode.
//
//      What is interesting is that both attributes and child keys are treated
//      in the same way, so after the creation of the tree we cannot really tell
//      if a specific node was created by an attribute or a nested key.
//
//      This class can be useful in cases where we need to read small and simple xml 
//      documents, as for example any configuration file.
//
//      The guts of the implementation are based in an open source code called
//      that is contained in the basicxmlnode.h file.
//

/******* The following is a very simple example of the use of this class.******
 *
#include "xmldocument.h"
#include <iostream>
using namespace std;

int main()
{
    XmlDocument doc("bookstore.xml");
    cout << doc.to_string() << endl;
    cout << " ==" << endl;
    for(auto p : doc)
    {
        cout << p->name() << ": "<< p->value()<< endl;
        for (auto p1 : *p)
        {
            cout << p1->name() << ": "<< p1->value()<< endl;
        }
    }
    cout << " ==" << endl;
    return 0;
}
*********************************************************************************/
//
// Note
//
// To compile with g++ 4.7 you need the following flags that can be added in the
// make file:
//
// FLAGS=-g -D DEBUG -std=c++0x -pthread  
// INCLUDE=-I$(DEVELOPMENT_ROOT)/include
// VPATH = $(DEVELOPMENT_ROOT)/include



#ifndef XML_PARSER_INCLUDED
#define XML_PARSER_INCLUDED

#include "basicxmlnode.h"
#include<map>


class XmlNode;

using XML_NODE_PTR = XmlNode*;
using XML_NODE_REF = XmlNode&;
using XML_NODE_PTR_VECTOR = std::vector<XML_NODE_PTR>;
using XML_NODE_PTR_ITER = std::vector<XML_NODE_PTR>::iterator;
using XML_NODE_PTR_MAP = std::map<std::string, XML_NODE_PTR>;

class XmlNode
{
public:
    // use the [i] operator to access a child by its index
    virtual XML_NODE_PTR operator[](int index) = 0;

    // use the [name] operator to access a child knowing its name
    virtual XML_NODE_REF operator[](const std::string& name) = 0;

    virtual bool contains(const std::string& key) const = 0;

    // returns the name of the node
    virtual std::string name() const = 0; 

    // returns the value of the node
    virtual std::string value() const = 0; 

    virtual double value_to_double() const = 0; 

    virtual int value_to_int() const = 0; 

    // recursively constructs and returns a string representation of the node
    virtual std::string to_string() const = 0;

    // specifies the depth of the indentation to use for the string conversion 
    virtual void set_identation(int identation) = 0;

    // get the depth of the indentation to use for the string conversion
    virtual int get_identation() const = 0;

    // returns an iterator to the first child of the node 
    virtual XML_NODE_PTR_ITER begin() = 0;

    // returns an end iterator of the children of the node
    virtual XML_NODE_PTR_ITER end() = 0;

    // returns the total number of the children of the node
    virtual int count_children() const = 0;
};

class XmlChildNode: public XmlNode
{
        const std::string _name;
        const std::string _value;
        XML_NODE_PTR_VECTOR _children;
        XML_NODE_PTR_MAP _map;
        mutable int _identation;
    public:

        XmlChildNode(const std::string& name, const std::string& value):
            _name(name), _value(value)
        {
        }
        

        XmlChildNode(basicxmlnode* node):
            _name(node->tag), _value(trim(node->text))
        {
                for(register int i =0; node->attrs[i]; ++i)
                    _children.push_back(new XmlChildNode(node->attrs[i], node->values[i]));

                for(register int i = 0; node->children[i]; ++i)
                    _children.push_back(new XmlChildNode(node->children[i]));

                for(auto p_node : _children)
                    _map[p_node->name()] = p_node;
                    
        }

        virtual ~XmlChildNode()
        {
            for(auto p_node: _children)
                delete p_node;
            _children.clear();
            _map.clear();
        }

        XmlChildNode(const XmlChildNode&) = delete;

        XmlChildNode& operator=(const XmlChildNode&) = delete;

        virtual int count_children() const { return _children.size(); }

        virtual XML_NODE_PTR operator[](int index) 
        { 
            return index >= 0 && index < _children.size() ? _children[index] : NULL;
        }

        virtual bool contains(const std::string& key) const 
        {
            return _map.find( key ) != _map.end();
        }

        virtual XML_NODE_REF operator[](const std::string& key) 
        {
            if(contains(key))
                return *_map[key];
            throw "key does not exist";
        }

        virtual std::string name() const { return _name; }

        virtual std::string value() const { return _value; }

        virtual double value_to_double() const { return std::stod(_value); }

        virtual int value_to_int() const { return std::stoi(_value); }

        virtual std::string to_string() const 
        { 
            int identation = get_identation();
            auto s = std::string(identation++*4, ' ' ) + name(); 
            s +=  value().length()> 0 ? ":[" + value() + "]\n" : "\n";
            for(auto pNode : _children)
            {
                pNode->set_identation(identation);
                s += pNode->to_string();
            }
            return s;
        }

        virtual void set_identation(int identation) { _identation = identation; }

        virtual int get_identation() const { return _identation; }

        virtual XML_NODE_PTR_ITER begin() { return _children.begin(); }

        virtual XML_NODE_PTR_ITER end() { return _children.end(); }
};

class XmlDocument : public XmlNode
{
        XML_NODE_PTR _p_root;

        mutable int _identation;

        void delete_nodes()
        {
            if(NULL != _p_root)
            {
                delete _p_root;
                _p_root = NULL;
            }
        }
    
    public:

        XmlDocument(): _p_root(NULL), _identation(0)
        {
        }

        XmlDocument(const std::string& filename): _p_root(NULL), _identation(0)
        {
            open(filename);
        }

        virtual ~XmlDocument()
        {
            delete_nodes();
        }

        XmlDocument(const XmlDocument&) = delete;

        XmlDocument& operator=(const XmlDocument&) = delete;


        virtual int count_children() const 
        {
            return NULL != _p_root ? _p_root->count_children() : 0;
        }

        virtual XML_NODE_PTR operator[](int index) 
        {
            return NULL != _p_root ? _p_root->operator[](index) : NULL;
        }

        virtual XML_NODE_REF operator[](const std::string& key) 
        {
            if(NULL != _p_root)
                return _p_root->operator[](key);
            throw "document not ready";
        }

        virtual bool contains(const std::string& key) const 
        {
            if(NULL != _p_root)
                return _p_root->contains(key);
            throw "document not ready";
        }

        virtual std::string name() const 
        {
            return NULL != _p_root ? _p_root->name() : "";
        }

        virtual std::string value() const 
        {
            return NULL != _p_root ? _p_root->value() : "";
        }


        virtual double value_to_double() const 
        { 
            return NULL != _p_root ? _p_root->value_to_double() : 0.0;
        }

        virtual int value_to_int() const 
        { 
            return NULL != _p_root ? _p_root->value_to_int() : 0.0;
        }


        virtual std::string to_string() const 
        {
            if(NULL != _p_root)
            {
                _p_root->set_identation(0);   
                return _p_root->to_string();
            }
            else
            {
                return "";
            }
        }
        
       void open(const std::string& filename) 
       {
                FILE * fp = fopen(filename.c_str(), "rt");
                if (NULL == fp) throw("open failed");
                basicxmlnode* root = readbasicxmlnode(fp);
                assert(NULL != root);
                fclose(fp);
                fp = NULL;
                if (NULL == root) throw("read failed");
                _p_root = new XmlChildNode(root);
                assert(NULL != _p_root);
                deletebasicxmlnode(root);
       }

       virtual void set_identation(int identation) { _identation = identation; }

       virtual int get_identation() const { return _identation; }

       virtual XML_NODE_PTR_ITER begin() 
       {
            if(NULL != _p_root)
            {
                return _p_root->begin();
            }
            throw "document not ready";
            
       }

       virtual XML_NODE_PTR_ITER end() 
       {
            if(NULL != _p_root)
            {
                return _p_root->end();
            }
            throw "document not ready";
       }
};
#endif
