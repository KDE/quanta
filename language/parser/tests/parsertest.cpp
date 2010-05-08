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
#include "sgmldebugvisitor.h"
#include "editorintegrator.h"

QTEST_MAIN(Xml::TestParser)

namespace Xml
{

TestParser::TestParser()
{
}

void TestParser::parser_data()
{

    QTest::addColumn<QString>("contents");
    QTest::newRow("DTD DEFINE       ") << "<!DOCTYPE perm [<!ELEMENT perm (user*,group*,property*)?> <!ATTLIST user gid (guest|user|root) 'guest' pest CDATA #IMPLIED> ]>";
    QTest::newRow("DTD ATT          ") << "<!DOCTYPE perm [<!ATTLIST user active (true|false) #IMPLIED 'true' role CDATA 'guest' > ]>";
    QTest::newRow("DTD ENUM         ") << "<!DOCTYPE perm [<!ATTLIST (user|server ) active ( true | false ) true auth ( true | false ) #implied>]>";
    QTest::newRow("DTD ELEMENT INCL ") << "<!DOCTYPE perm [<!ELEMENT user (role)* +(gid)>]>";
    QTest::newRow("DTD ELEMENT SEQ  ") << "<!DOCTYPE perm [<!ELEMENT (name1|name2) ((name3|name4)|(name5, name6))>]>";
    QTest::newRow("XML ATT NS       ") << "<ns:name attname='' attns:attname='value'> text </ns:name>";
    QTest::newRow("XML PROC         ") << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><root/>";
    QTest::newRow("XML DOCTYPE      ") << "<root><![CDATA[]]></root>";
    QTest::newRow("DTD HTML         ") << "<!doctype html>";
    QTest::newRow("DTD HTML PUBLICID") << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
    QTest::newRow("HTML JAVASCRIPT  ") << "<!doctype html><script type=\"text/javascript\">var t=1<2?1:2;</script>";
    QTest::newRow("HTML TEXT        ") << "<!doctype html><head><body><p>Some text<p>More text</body></head><blah/>";
    
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
    /*
    QList<QString> contents;
    contents << "<?xml?><root1><child1/><child2><child3/></child2></root1><root2/>";

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
    */
}

}

#include "parsertest.moc"
