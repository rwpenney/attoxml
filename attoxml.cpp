/*
 *  Methods for simple XML document generator and printer
 *  RW Penney, May 2012
 */

/*
 *   Copyright 2012 RW Penney <rwpenney@users.sourceforge.net>
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

#include "attoxml.h"


namespace attoxml {

/*
 *  ==== BareNode ====
 */

BareNode::BareNode(const std::string &name_)
    : name(name_)
{
    // Nothing else
}


BareNode::BareNode(const BareNode &other)
    : name(other.name)
{
    // Nothing else
}


BareNode::~BareNode()
{
    // Nothing
}



/*
 *  ==== Attribute ====
 */

Attribute::Attribute(const std::string &key_, const std::string &value_)
    : key(key_), value(value_)
{
    length = key.size() + value.size();
}


Attribute::Attribute(const Attribute &other)
    : key(other.key), value(other.value)
{
    length = other.length;
}


Attribute &Attribute::operator=(const Attribute &other)
{
    if (&other != this) {
        key = other.key;
        value = other.value;
        length = other.length;
    }

    return *this;
}


Attribute::~Attribute()
{
}



/*
 *  ==== Node ====
 */

Node::Node(Document *doc, const std::string &name_)
    : BareNode(name_), theDoc(doc)
{
    // Nothing else
}


Node::~Node()
{
    while (!children.empty()) {
        delete children.front();
        children.pop_front();
    }
}


template <>
void Node::AddAttribute(const std::string &name, const std::string &value)
{
    attributes.push_back(Attribute(name, value));
}


void Node::AddAttribute(const std::string &name, const char *value)
{
    attributes.push_back(Attribute(name, value));
}


Node *Node::PrependChild(const std::string &name_)
{   Node *newnode = new Node(theDoc, name_);

    children.push_front(newnode);

    return newnode;
}


Node *Node::AppendChild(const std::string &name_)
{   Node *newnode = new Node(theDoc, name_);

    children.push_back(newnode);

    return newnode;
}


void Node::AppendText(const std::string &txt)
{
    children.push_back(new TextNode(txt));
}


/*!
 *  Recursively pretty-print this element and its children.
 */
void Node::Output(std::ostream &strm, const std::string &indent,
                    const std::string &indentStep) const
{   AttrList::const_iterator attr;
    std::list<BareNode*>::const_iterator child;
    const std::string attIndent = indent + "    ";
    size_t attLen = 0;

    strm << indent << "<" << name;

    for (attr=attributes.begin(); attr!=attributes.end(); ++attr) {
        if (attLen > theDoc->attrWidth) {
            strm << std::endl << attIndent;
            attLen = 0;
        } else {
            strm << " ";
        }
        strm << attr->key << "=\"" << attr->value << "\"";

        attLen += attr->length + 3;
    }

    if (!children.empty()) {
        const std::string childIndent = indent + indentStep;
        bool EOLdue = true;

        strm << ">";

        for (child=children.begin(); child!=children.end(); ++child) {
            const BareNode &nd = (**child);

            if (EOLdue && !nd.InLine()) strm << std::endl;

            nd.Output(strm, childIndent, indentStep);

            EOLdue = nd.InLine();
        }

        if (!EOLdue) strm << indent;
        strm << "</" << name << ">" << std::endl;
    } else {
        strm << "/>" << std::endl;
    }
}



/*
 *  ==== TextNode ====
 */

TextNode::TextNode(const std::string &content_)
    : BareNode("TEXT"), content(content_)
{
}


TextNode::~TextNode()
{
}


void TextNode::Output(std::ostream &strm, const std::string &name,
                        const std::string &indentStep) const
{
    strm << content;
}


/*
 *  ==== Document ====
 */

Document::Document(const std::string &root)
    : Node(this, root)
{
    attrWidth = 40;
    nsDecl = NULL;
}


Document::~Document()
{
    if (nsDecl != NULL) delete nsDecl;
}


void Document::SetDefaultNS(const std::string &xmlns)
{
    if (nsDecl != NULL) {
        attributes.erase(*nsDecl);
        delete nsDecl;
    }

    const AttrList::iterator attrLocation = attributes.insert(attributes.end(),
                                                    Attribute("xmlns", xmlns));
    nsDecl = new AttrList::iterator(attrLocation);
}


void Document::Print(std::ostream &strm, const std::string &indent)
{
    strm << "<?xml version=\"1.0\" encoding=\"UTF-8\""
        << " standalone=\"yes\"?>" << std::endl;

    Output(strm, "", indent);
}

}   // namespace attoxml


#ifdef ATTO_DEMO_MAIN

using namespace attoxml;

int main(int argc, char *argv[])
{   Document doc("TopNode");
    Node *nd0, *nd1, *nd2;

    doc.SetDefaultNS("http://pmcyg.sourceforge.net");
    doc.AddAttribute("timestamp", "01Jan1970");

    nd0 = doc.AppendChild("Alpha");
    nd0->AddAttribute("double", 1.72);
    nd0->AddAttribute("ident", "d44c8ba0d2d2");
    nd0->AppendChild("alpha_0");
    nd0->AppendChild("alpha_1");
    nd0->AppendChild("alpha_2");
    nd1 = doc.AppendChild("Beta");
    nd1->AddAttribute("ident", "d2cf30339783");
    nd2 = nd1->AppendChild("beta_0");
    nd2->AppendText("Some textual content");
    nd2->AddAttribute("float", 2 / 3.0f);
    nd2->AddAttribute("int", -34);
    nd2->AddAttribute("key", std::string("value"));
    nd2->AddAttribute("att1", 4.5);
    nd2->AddAttribute("att2", 4.7);
    nd2->AddAttribute("attribute3", 1<<30);
    nd2->AppendChild("beta_0_0");
    nd2 = doc.AppendChild("Gamma");
    nd2->AppendText("Rainfall in Spain favours small gradients");

    doc.Print(std::cout);

    return 0;
}

#endif  // ATTO_DEMO_MAIN
