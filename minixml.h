/*
 *  Declarations for simple XML document generator and printer
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


/*!
 *  Abstract representation of a data or text node
 *  within an XML document tree.
 */
class BareNode
{
    public:
        BareNode(const std::string &name_);
        BareNode(const BareNode &other);
        virtual ~BareNode();

        //! Pretty-print this node (possibly recursively),
        //! using the given indentation level and increment.
        virtual void Output(std::ostream &strm, const std::string &indent,
                            const std::string &indentStep) const = 0;

        //! Indicator of whether this node can be pretty-printed
        //! on a single line (e.g. TextNode).
        virtual bool InLine() const {
            return false; }

    protected:
        const std::string name;

    private:
        BareNode &operator=(const BareNode &other);
};


/*!
 *  Representation of an attribute within an XML element,
 *  consisting of an identifier and value.
 */
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

        size_t length;          //!< Nominal width of textual representation

    friend class Node;
};


/*!
 *  An node element within an XML document.
 *  This can be used as a container of child elements,
 *  including textual elements.
 *  Attributes associated with this node can be added via AddAttribute().
 */
class Node : public BareNode
{
    protected:
        Node(Document *doc, const std::string &name_);
        virtual ~Node();

    public:
        template <typename VAL_T>
        void AddAttribute(const std::string &name, const VAL_T &value);
        void AddAttribute(const std::string &name, const char *value);

        Node *PrependChild(const std::string &name_);
        Node *AppendChild(const std::string &name_);

        void AppendText(const std::string &txt);

        void Output(std::ostream &strm, const std::string &indent,
                    const std::string &indentStep) const;

    protected:
        //! The top-level document to which this Node belongs
        Document *theDoc;

        std::list<Attribute> attributes;
        std::list<BareNode*> children;
};


/*!
 *  Representation of a textual element within an XML document,
 *  typically used as a child of an ordinary Node.
 */
class TextNode : public BareNode
{
    public:
        TextNode(const std::string &content_);
        ~TextNode();

        void Output(std::ostream &strm, const std::string &indent,
                    const std::string &indentStep) const;

        virtual bool InLine() const {
            return true; }

    protected:
        std::string content;
};


/*!
 *  Representation of an entire XML document,
 *  identified by its root Node.
 */
class Document : public Node
{
    public:
        Document(const std::string &root);
        ~Document();

        void Print(std::ostream &strm, const std::string &indent="  ");

    protected:
        //! Stream used for formatting attribute values
        //! within Node::AddAttribute()
        std::stringstream attrStrm;

        //! Nominal width of page allowed for each line of attributes
        size_t attrWidth;


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

}   // namespace minixml

#endif  /* !_MINIXML_H */
