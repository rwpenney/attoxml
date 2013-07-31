/*
 *  Example usage of 'attoXML' document generator and printer
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

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "attoxml.h"

using namespace attoxml;


/*!
 *  Create a random XML document.
 */
void randomDoc(std::ostream &strm, const size_t n_nodes=16)
{   Document *theDoc = NULL;
    Node **allNodes, *parent, *child;
    size_t n, a, nAttr;

    allNodes = new Node*[n_nodes + 1];
    srand((unsigned)time(NULL) ^ 0xd44c8ba0u);

    theDoc = new Document("Random");
    theDoc->SetDefaultNS("http://www.rwpenney.org.uk/schemas/attoxml");
    allNodes[0] = theDoc;

    for (n=1; n<=n_nodes; ++n) {
        std::stringstream ident;

        ident << "Child" << std::setfill('0') << std::setw(3) << n;
        parent = allNodes[rand() % n];
        child = parent->AppendChild(ident.str());
        allNodes[n] = child;

        nAttr = rand() % 13;
        for (a=0; a<nAttr; ++a) {
            std::stringstream attr;
            attr << "attribute" << a;
            child->AddAttribute(attr.str(), (unsigned)(rand() & 0xfff));
        }
    }

    delete[] allNodes;

    theDoc->Print(strm);
    delete theDoc;
}


int main(int argc, char *argv[])
{
    // Create the document element:
    Document doc("TopNode");

    // Specify the default XML namespace to be used within the document:
    doc.SetDefaultNS("http://pmcyg.sourceforge.net");

    // Add an attribute to the top-level element
    // (This could also be used for schema references):
    doc.AddAttribute("timestamp", "01Jan1970");

    // Prepare a set of pointers for miscellaneous child elements:
    Node *nd0, *nd1, *nd2;

    // Create first child node, called 'Alpha':
    nd0 = doc.AppendChild("Alpha");
    nd0->AddAttribute("double", 1.72);
    nd0->AddAttribute("ident", "d44c8ba0d2d2");

    // Add some children of the 'Alpha' element:
    nd0->AppendChild("alpha_0");
    nd0->AppendChild("alpha_1");
    nd0->AppendChild("alpha_2");

    // Add a second child, called 'Beta':
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
    nd2->AppendText(StringBuilder("Rainfall in Spain")
                    << " favours gradients"
                    << " less than " << 13 << "%");

    nd0 = doc.AppendNamedText("Finale", "Thats all folks");
    nd0->AddAttribute("type", "Grand");

    // Now that the element tree has been fully populated,
    // pretty-print it to the standard output:
    doc.Print(std::cout);

    return 0;
}
