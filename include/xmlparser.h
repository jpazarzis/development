#ifndef XML_PARSER_INCLUDED
#define XML_PARSER_INCLUDED

#include "basicxmlnode.h"
#include<map>


class XmlNode;

using XML_NODE_PTR = XmlNode*;
using XML_NODE_PTR_VECTOR = std::vector<XML_NODE_PTR>;
using XML_NODE_PTR_ITER = std::vector<XML_NODE_PTR>::iterator;
using XML_NODE_PTR_MAP = std::map<std::string, XML_NODE_PTR>;

class XmlNode
{
public:
    virtual XML_NODE_PTR operator[](int index) = 0;
    virtual XML_NODE_PTR operator[](const std::string& name) = 0; 
    virtual std::string name() const = 0; 
    virtual std::string value() const = 0; 
    virtual std::string to_string() const = 0;
    virtual void set_identation(int identation) = 0;
    virtual int get_identation() const = 0;
    virtual XML_NODE_PTR_ITER begin() = 0;
    virtual XML_NODE_PTR_ITER end() = 0;
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

        virtual XML_NODE_PTR operator[](const std::string& key) 
        {
            return _map.find( key ) != _map.end() ? _map[key] : NULL;
        }

        virtual std::string name() const { return _name; }

        virtual std::string value() const { return _value; }

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

        virtual XML_NODE_PTR operator[](const std::string& key) 
        {
            return NULL != _p_root ? _p_root->operator[](key) : NULL;
        }

        virtual std::string name() const 
        {
            return NULL != _p_root ? _p_root->name() : "";
        }

        virtual std::string value() const 
        {
            return NULL != _p_root ? _p_root->value() : "";
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
