# attoXML

attoXML is a small C++ library for generating well-formed XML documents.

attoXML provides a set of C++ classes for creating nested trees
of XML elements, including pure text nodes, and user-defined attributes,
which can be easily exported into human-readable text files.

attoXML is intended for applications where write-only support for XML is
required, and offers reliable mechanisms for ensuring that the output
document is syntactically correct XML and neatly indented.

The source-files 'attoxml.h', 'attoxml.cpp' can be directly embedded within
client applications (subject to the Apache-2.0 license conditions).
Alternatively, the [CMake](https://cmake.org) build-system
will compile a shared library for linking against client applications.
The file 'example.cpp' shows how a non-trivial XML document can be
constructed via the 'Document' and 'Node' classes.

Applications requiring fuller support for features such as XML ingest/parsing,
schema validation, non-trivial normalization of textual content,
should look towards one of the more heavyweight XML libraries,
such as (in no particular order):
[Expat](https://libexpat.github.io/),
[libxml++](http://libxmlplusplus.sourceforge.net/),
[TinyXML2](https://github.com/leethomason/tinyxml2),
[Xerces-C++](https://xerces.apache.org/xerces-c/), etc.


## Example

With a recent (C++11) compiler, one can use a recipe such as:

```cpp
    attoxml::Document doc("RootNode");
    doc.SetDefaultNS("http://my.namespace.com");

    auto child0 = doc.AppendChild("ZerothChild");
    child0->AddAttribute("parameter", 3.14159);

    auto grandchild = child0->AppendChild("Grandchild");
    grandchild->AppendText(attoxml::StringBuilder("Alpha")
                            << ", beta"
                            << ", gamma");

    auto child1 = doc.AppendChild("FirstChild");

    doc.Print(std::cout);
```

to construct an XML document of the form:

```xml
    <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
    <RootNode xmlns="http://my.namespace.com">
      <ZerothChild parameter="3.14159">
        <Grandchild>Alpha, beta, gamma</Grandchild>
      </ZerothChild>
      <FirstChild/>
    </RootNode>
```
