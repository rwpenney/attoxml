/*
 *  (C)Copyright RW Penney 2012
 */

#include "minixml.h"


namespace minixml {

BareNode::BareNode(const std::string &ident_)
    : ident(ident_)
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

Node::Node(Document *doc, const std::string &ident_)
    : BareNode(ident_), theDoc(doc)
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


void Node::AddAttribute(const std::string &name, const char *value)
{
    attributes.push_back(Attribute(name, value));
}


Node *Node::PrependChild(const std::string &ident_)
{   Node *newnode = new Node(theDoc, ident_);

    children.push_front(newnode);

    return newnode;
}


Node *Node::AppendChild(const std::string &ident_)
{   Node *newnode = new Node(theDoc, ident_);

    children.push_back(newnode);

    return newnode;
}


void Node::Output(std::ostream &strm, const std::string &indent,
                    const std::string &indentStep)
{   std::list<Attribute>::const_iterator attr;
    std::list<BareNode*>::const_iterator child;
    const std::string attIndent = indent + "    ";
    size_t attLen = 0;

    strm << indent << "<" << ident;

    for (attr=attributes.begin(); attr!=attributes.end(); ++attr) {
        if (attLen > 40) {
            strm << std::endl << attIndent;
            attLen = 0;
        } else {
            strm << " ";
        }
        strm << attr->key << "=\"" << attr->value << "\"";

        attLen += attr->length;
    }

    if (!children.empty()) {
        strm << ">" << std::endl;

        const std::string childIndent = indent + indentStep;

        for (child=children.begin(); child!=children.end(); ++child) {
            (*child)->Output(strm, childIndent, indentStep);
        }
        strm << indent << "</" << ident << ">" << std::endl;
    } else {
        strm << "/>" << std::endl;
    }
}



/*
 *  ==== Document ====
 */

Document::Document(const std::string &ident_)
    : Node(this, ident_)
{
}


Document::~Document()
{
    // Nothing
}


void Document::Print(std::ostream &strm, const std::string &indent)
{
    strm << "<?xml version=\"1.0\" encoding=\"UTF-8\""
        << " standalone=\"yes\"?>" << std::endl;

    Output(strm, "", indent);
}

}   // namespace minixml


#if 1

using namespace minixml;

int main(int argc, char *argv[])
{   Document doc("TopNode");
    Node *nd0, *nd1, *nd2;

    nd0 = doc.AppendChild("Alpha");
    nd0->AddAttribute("double", 1.72);
    nd0->AppendChild("alpha_0");
    nd0->AppendChild("alpha_1");
    nd0->AppendChild("alpha_2");
    nd1 = doc.AppendChild("Beta");
    nd2 = nd1->AppendChild("beta_0");
    nd2->AddAttribute("float", 2 / 3.0f);
    nd2->AddAttribute("int", -34);
    nd2->AddAttribute("string", std::string("bother"));
    nd2->AddAttribute("att1", 4.5);
    nd2->AddAttribute("att2", 4.7);
    nd2->AddAttribute("attribute3", 1<<30);
    nd2->AppendChild("beta_0_0");
    nd2 = doc.AppendChild("Gamma");

    doc.Print(std::cout);

    return 0;
}

#endif
