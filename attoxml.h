/*
 *  Declarations for simple XML document generator and printer
 *  RW Penney, May 2012
 */

#ifndef _ATTOXML_H
#define _ATTOXML_H

/*
 *   Copyright 2012-2013 RW Penney <rwpenney@users.sourceforge.net>
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <iostream>
#include <list>
#include <sstream>
#include <string>


namespace attoxml {

class Document;
class StringBuilder;


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
        void AddAttribute(const std::string &attr, const VAL_T &value);
        void AddAttribute(const std::string &attr, const char *value);

        Node *PrependChild(const std::string &child);
        Node *AppendChild(const std::string &child);

        void AppendText(const std::string &txt);
        template <typename VAL_T>
        Node *AppendNamedText(const std::string &child, const VAL_T &text);
        Node *AppendNamedText(const std::string &child,
                              const StringBuilder &strm);

        void Output(std::ostream &strm, const std::string &indent,
                    const std::string &indentStep) const;

    protected:
        //! The top-level document to which this Node belongs
        Document *theDoc;

        typedef std::list<Attribute> AttrList;

        AttrList attributes;
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

        //! Specify the default XML namespace for the document
        void SetDefaultNS(const std::string &xmlns);

        void Print(std::ostream &strm, const std::string &indent="  ");

    protected:
        //! Stream used for formatting attribute values
        //! within Node::AddAttribute()
        std::stringstream attrStrm;

        std::stringstream &freshAttrStream();

        //! Nominal width of page allowed for each line of attributes
        size_t attrWidth;

        //! Location within the list of attributes
        //! of any current default namespace declaration
        AttrList::iterator *nsDecl;

    friend class Node;
};


/*!
 *  Stream-like class for building textual nodes
 *  via syntax like:
 *  \code
 *  node->AppendText(StringBuilder("Pi is poorly approximated by "
 *                                  << 22 / 7.0));
 *  \endcode
 */
class StringBuilder
{
  public:
    StringBuilder(const std::string &root="") {
      strm << root; }
    StringBuilder(const StringBuilder &other)
      : strm(other.strm.str()) {}
    StringBuilder &operator=(const StringBuilder &other) {
      strm.str(other.strm.str());
      return *this; }
    ~StringBuilder() {}

    template <typename T>
    StringBuilder &operator<<(const T &chunk) {
      strm << chunk;
      return *this; }

    operator std::string() const {
      return strm.str(); }

  protected:
    std::stringstream strm;
};


/*!
 *  Add an attribute to the current element,
 *  automatically converting the supplied value to a string.
 */
template <typename VAL_T>
void Node::AddAttribute(const std::string &attr, const VAL_T &value)
{   std::stringstream &strm = theDoc->freshAttrStream();

    strm << value;
    attributes.push_back(Attribute(attr, strm.str()));
}


/*!
 *  Add a child element, and populate that child with a pure-text child.
 */
template <typename VAL_T>
Node *Node::AppendNamedText(const std::string &child, const VAL_T &text)
{   Node *childElt = AppendChild(child);
    std::stringstream &strm = theDoc->freshAttrStream();

    strm << text;
    childElt->AppendText(strm.str());

    return childElt;
}

}   // namespace attoxml

#endif  /* !_ATTOXML_H */
