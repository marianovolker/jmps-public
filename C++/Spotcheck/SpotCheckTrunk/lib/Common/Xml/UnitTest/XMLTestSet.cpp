/*
 * XMLTestSet.cpp
 *
 *  Created on: Jun 22, 2011
 *      Author: pcantarini
 */

#include "XMLTestSet.h"

char acXMLTest1[]=""
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<!-- Comment1 -->\n"
"<root name=\"RootNode\">\n"
"\n"
"\t<flag1/>\n"
"The RootNode fly text.\n"
"\t<flag2 range=\"True-False\"/>\n"
"Splited in many lines.\n"
"<!-- Comment2 Line1\n Comment2 Line2 -->\n"
"\t<child1>\n"
"\t</child1>\n"
"\n"
"\t<child2 hasTwin=\"True\">\n"
"\t\t<subchild>TheChildValue</subchild>\n"
"\t</child2>\n"
"\n"
"\t<child2>\n"
"\t\t<subchild lastCall=\"False\">LastElementValue</subchild>\n"
"\t</child2>\n"
"Even the last line.\n"
"</root>\n";

char acXMLTest2[]=""
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<!DOCTYPE root [\n"
"\n"
"<!ELEMENT root (child1)>\n"
"<!ELEMENT child1 (#CDATA)>\n"
"\n"
"<!ATTLIST child1 myattr1 CDATA #REQUIRED>\n"
"<!ATTLIST child1 myattr2 CDATA #IMPLIED>\n"
"\n"
"]>\n"
"<!-- Comment1 -->\n"
"<?ProcessingInstruction1?>\n"
"<root xmlns=\"http://www.mynamespace.com/cracatoa/\" myattr0=\"myvalue0\">\n"
"\t<child1 myattr1=\"myvalue1\">\n"
"\t\tIndeed my dear friend.\n"
"\t\t<![CDATA[\n"
"This text is not to be parsed so we can use <> and whatever we want.\n"
"]]>\n"
"\t\t<subchild>TheChildValue</subchild>\n"
"\t</child1>\n"
"</root>\n";
