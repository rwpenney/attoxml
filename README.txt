This directory contains C++ source-code for the 'attoXML' document generator,
provided under the Apache License, Version 2.0.

attoXML is a set of simple C++ classes for creating well-formed XML documents
which can be easily exported into human-readable text files.

attoXML is intended for applications where write-only support for XML is
required, and offers reliable mechanisms for ensuring that the output
document is syntactically correct XML and neatly indented.

The source-files 'attoxml.h', 'attoxml.cpp' can be directly embedded within
client applications (subject to the Apache-2.0 license conditions).
The file 'example.cpp' shows how a non-trivial XML document can be
constructed via the 'Document' and 'Node' classes.

Applications requiring fuller support for features such as XML ingest/parsing,
schema validation, non-trivial normalization of textual content,
should look towards one of the more heavy-weight XML libraries,
such as (in no particular order): eXpat, libxml2, TinyXML, Xerces-C++, etc.

Constructive comments and bug-fixes should be sent to the author:
    rwpenney@users.sourceforge.net
