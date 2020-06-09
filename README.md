# Stack-Application
Designing a C++ program that use a stack for matching tags and quotes in XML eXtensive Markup Language.

## Output Format
Your program will get an XML code in an input file and it should figure out if tags and quotes are properly matched or
not using stack. In case the tags are not properly matched then your program should report following;
i) the first error 
ii) print the mismatched tag
iii) inform the line number where the starting tag occurred


## What is XML?
XML is a markup language somewhat like HTML. It was designed to store and transport data. XML is just
information wrapped in user defined tags which is both human- and machine-readable. The XML language has no
predefined tags like html. The tags are "invented" by the author of the XML document. For details see
https://www.w3schools.com/xml/xml_whatis.asp

## Features
Your program should handle the following features of XML:
1. xml prolog (xml header)
2. xml tags (xml elements). The xml tags are case sensitive.
3. xml attribute
4. xml comments, start with <!-- and ends with -->