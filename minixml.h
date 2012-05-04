/*
 *  Simple XML document generator
 *  (C)Copyright RW Penney 2012
 */

#ifndef _MINIXML_H
#define _MINIXML_H

#include <iostream>
#include <list>
#include <sstream>
#include <string>


namespace minixml {

class Document;


class BareNode
{
    public:
        BareNode(const std::string &ident_);
        BareNode(const BareNode &other);
        BareNode &operator=(const BareNode &other);
        virtual ~BareNode();

        virtual void Output(std::ostream &strm, const std::string &indent,
                            const std::string &indentStep) = 0;

    protected:
        const std::string ident;
};


class Attribute
{
    public:
        Attribute(const std::string &key_, const std::string &value_);
        Attribute(const Attribute &other);
        Attribute &operator=(const Attribute &other);
        ~Attribute();

    protected:
        std::string key;
        std::string value;

        size_t length;

    friend class Node;
};


class Node : public BareNode
{
    protected:
        Node(Document *doc, const std::string &ident_);
        virtual ~Node();

    public:
        template <typename VAL_T>
        void AddAttribute(const std::string &name, const VAL_T &value);
        void AddAttribute(const std::string &name, const char *value);

        Node *PrependChild(const std::string &ident_);
        Node *AppendChild(const std::string &ident_);

        void AppendText(const std::string &txt);

        void Output(std::ostream &strm, const std::string &indent,
                    const std::string &indentStep);

    protected:
        Document *theDoc;

        std::list<Attribute> attributes;
        std::list<BareNode*> children;
};


class TextNode : public BareNode
{
    public:
        TextNode(const std::string &content_);
        ~TextNode();

        void Output(std::ostream &strm, const std::string &indent,
                    const std::string &indentStep);

    protected:
};


class Document : public Node
{
    public:
        Document(const std::string &ident_);
        ~Document();

        void Print(std::ostream &strm, const std::string &indent="  ");

    protected:
        std::stringstream attrStrm;

    friend class Node;
};


template <typename VAL_T>
void Node::AddAttribute(const std::string &name, const VAL_T &value)
{
    theDoc->attrStrm.clear();
    theDoc->attrStrm.str("");

    theDoc->attrStrm << value;
    attributes.push_back(Attribute(name, theDoc->attrStrm.str()));
}

template <>
void Node::AddAttribute(const std::string &name, const std::string &value)
{
    std::cerr << "SPECIAL";
    attributes.push_back(Attribute(name, value));
}

}   // namespace minixml

#endif  /* !_MINIXML_H */
