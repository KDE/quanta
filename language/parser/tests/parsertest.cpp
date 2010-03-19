/*
Copyright (C) 2010  Ruan Strydom <ruan@jcell.co.za>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "parsertest.h"

#include <QTest>

#include "parsesession.h"
#include "mldebugvisitor.h"
#include "editorintegrator.h"

QTEST_MAIN(Ml::TestParser)

namespace Ml
{

TestParser::TestParser()
{
}

void TestParser::parser_data()
{

    QTest::addColumn<QString>("contents");
    
    QTest::newRow("XML ATT NS       ") << "<ns:name attname='value' attns:attname='value'> text </ns:name>";
    QTest::newRow("XML PROC         ") << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><root/>";
    QTest::newRow("XML DOCTYPE      ") << "<root><![CDATA[]]></root>";
    QTest::newRow("HTML JAVASCRIPT  ") << "<script type=\"text/javascript\">var t=1<2?1:2;</script>";
    QTest::newRow("DTD ELE SEQUENCE ") << "<!ELEMENT Name (Last_Name, First_Name, Tel+)>";
    QTest::newRow("DTD ELE CHOICE   ") << "<!ELEMENT ITEM (PRODUCT, NUMBER?, (PRICE | CHARGEACCT | SAMPLE)+)>";
    QTest::newRow("DTD ELE MIXED    ") << "<!ELEMENT PRODUCT (#PCDATA | PRODUCT_ID)*>";
    QTest::newRow("DTD ELE ENT PARM ") << "<!ELEMENT CUSTOMER %record;>";
    QTest::newRow("DTD ENTITY VALUE ") << "<!ENTITY name \"value\">";
    QTest::newRow("DTD ENTITY EXTERN") << "<!ENTITY name SYSTEM \"uri\">";
    QTest::newRow("DTD ENTITY PARM  ") << "<!ENTITY % record \"(NAME, DATE, ORDERS)\">";
    QTest::newRow("DTD ATTLIST      ") << "<!ATTLIST elementName name CDATA #REQUIRED>";
    QTest::newRow("DTD ATTLIST DEF  ") << "<!ATTLIST elementName name CDATA \"default\">";
    QTest::newRow("DTD ATTLIST ENUM ") << "<!ATTLIST elementName name (check | cash) \"default\">";
    QTest::newRow("DTD DEFINE       ") << "<!DOCTYPE root [<!ELEMENT name ANY> ]>";
    QTest::newRow("DTD HTML         ") << "<!doctype html>";
    QTest::newRow("DTD HTML PUBLICID") << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";

}

void TestParser::parser()
{
    QFETCH(QString, contents);
    ParseSession session;
    session.setContents(contents);
    StartAst* ast = 0;
    QVERIFY(session.parse(&ast));
    DebugVisitor debugVisitor(session.tokenStream(), session.contents());
    debugVisitor.visitStart(ast);
}

void TestParser::parserForFun()
{
    QList<QString> contents;
    contents << "<?xml?>\n\
    <!--Users DOCTYPE-->\n\
    <!DOCTYPE users [\n\
        <!--Simple User-->\n\
        <!ELEMENT user (userName | passWord) >\n\
        <!ATTLIST user\n\
            islocked (true | false) \"true\"\n\
            group CDATA #REQUIRED\n\
            issystem CDATA #FIXED \"false\">\n\
    ]>\n\
    <users>\n\
        <user isLocked=\"false\" group=\"operator\"/>\n\
    </users>";

    foreach(QString content, contents) {
        ParseSession session;
        session.setContents(content);
        StartAst* ast = 0;
        QVERIFY(session.parse(&ast));
        DebugVisitor debugVisitor(session.tokenStream(), session.contents());
        debugVisitor.visitStart(ast);
        foreach(KDevelop::ProblemPointer p, session.problems()) {
            kDebug() << p->description() << p->explanation() << p->sourceString();
        }
    }
}

}

#include "parsertest.moc"
