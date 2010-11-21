/*
   This file is part of KDevelop
   Copyright 2009 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "parsertest.h"

#include <QTest>

#include "../parsesession.h"
#include "cssdebugvisitor.h"
#include "../editorintegrator.h"
#include "../htmlparser.h"

QTEST_MAIN(Css::TestParser)

namespace Css
{

void TestParser::parser_data()
{
    QTest::addColumn<QString>("contents");

    QTest::newRow("one declaration without semicolon")     << "body{color:red}";
    QTest::newRow("one declaration with semicolon") << "body{color:red;}";
    QTest::newRow("selector element") << "body{}";
    QTest::newRow("selector class") << ".foo{}";
    QTest::newRow("selector element.class") << "body.foo{}";
    QTest::newRow("selector star") << "*{}";
    QTest::newRow("selector star.class") << "*.foo{}";
    QTest::newRow("selector element attribute1") << "body[foo]{}";
    QTest::newRow("selector element attribute2") << "body[foo~=bar]{}";
    QTest::newRow("selector element attribute3") << "body[foo|=bar]{}";
    QTest::newRow("selector element pseudoclass") << "body:hover{}";
    QTest::newRow("selector element pseudoclass function") << "body:lang(no){}";
    QTest::newRow("selector element pseudoclass nth-child1") << "body:nth-child(even){}";
    QTest::newRow("selector element pseudoclass nth-child2") << "body:nth-child(4){}";
    QTest::newRow("selector element pseudoclass nth-child3") << "body:nth-child(5n+2){}";
    QTest::newRow("selector element pseudoclass selection") << "body::selection{}";
    QTest::newRow("declaration with px term") << "body{width:100px;}";
    QTest::newRow("declaration with em term") << "body{width:5em;}";
    QTest::newRow("declaration with none term") << "body{border:none;}";
    QTest::newRow("declaration with negative px term") << "body{width:-100px;}";
    QTest::newRow("declaration with positive px term") << "body{width:+100px;}";
    QTest::newRow("declaration with url term1") << "body{background-image:url(http://example.com/foo.png);}";
    QTest::newRow("declaration with url term2") << "body{background-image:url(/foo.png);}";
    QTest::newRow("declaration hex color term1") << "body{color:#ff0000;}";
    QTest::newRow("declaration hex color term2") << "body{color:#f00;}";
    QTest::newRow("declaration with multipe px terms") << "body{border-width:1px 2px 3px 4px;}";
    QTest::newRow("charset") << "@charset \"utf-8\"; body{}";
    QTest::newRow("import url") << "@import url(\"foo.css\"); body{}";
    QTest::newRow("import string") << "@import \"foo.css\"; body{}";
    QTest::newRow("two rules") << "h1{}h2{}";
    QTest::newRow("two rules whitespace") << "h1{} h2{}";
    QTest::newRow("missing value") << "body{width:;}"; //todo check for reported error
    QTest::newRow("missing value brace") << "body{width:1px;"; //todo check for reported error
    QTest::newRow("two selectors with spaces") << " a , b {}";
    QTest::newRow("selector with comma") << " a,{}"; //todo check for reported error
    QTest::newRow("selector without declarations") << "a"; //todo check for reported error
    QTest::newRow("selector without declarations2") << " a , "; //todo check for reported error
    QTest::newRow("selector without declarations3") << " a b "; //todo check for reported error
    QTest::newRow("space in declaration") << "body{ color:red;}";
    QTest::newRow("space in declaration") << "body{ color:red}";
    QTest::newRow("id selector") << "#foo{}";
    QTest::newRow("id element selector") << "body#foo{}";
    QTest::newRow("id element selector2") << "#foo a{}";
    QTest::newRow("selector with space") << "a a{}";
    QTest::newRow("empty declaration with space") << "a{ }";
    QTest::newRow("first property no value") << "a{color:;width:0;}";
    QTest::newRow("first property no value2") << "a{color: ; width:0;}";
    QTest::newRow("first property no value no semicolon") << "a{color:width :0;}";
    QTest::newRow("first property no value no semicolon2") << "a{color:width:0;}";
    QTest::newRow("first property no value no semicolon3") << "a{color: width:0;}";
    QTest::newRow("first property no value no semicolon4") << "a { color : width : 0 ; }";
    QTest::newRow("rgb value") << "a { color: rgb(1,2,3); }";
    QTest::newRow("rgb value2") << "a { color: rgb(1%,2%,3%); }";
    QTest::newRow("rgb value3") << "a { color: rgb(255,0,0); }";
    QTest::newRow("rgba value") << "a { color: rgba(255,0,0,127); }";
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

void TestParser::multiline()
{
    ParseSession session;
    session.setContents("body{color:red}\nbody{color:blue}");
    //                   012345678901234  01234567890123456
    //                   0         1      0         1
    StartAst* ast = 0;
    QVERIFY(session.parse(&ast));
    DebugVisitor debugVisitor(session.tokenStream(), session.contents());
    RuleAst* el = ast->rules->rulesSequence->at(1)->element;
    debugVisitor.visitNode(el);

    EditorIntegrator editor;
    editor.setParseSession(&session);
    kDebug() << editor.findPosition(el->startToken, EditorIntegrator::FrontEdge);
    kDebug() << editor.findPosition(el->endToken, EditorIntegrator::BackEdge);
    QCOMPARE(editor.findPosition(el->endToken, EditorIntegrator::BackEdge),
                KDevelop::CursorInRevision(1, 16));
}

void TestParser::htmlStyleElement()
{
    HtmlParser p;
                 //0         1          2          3         4
                 //012345678901234567 890123456 78901234567890123456789
    p.setContents("<html><style type=\"text/css\">a{color:red}</style></html>");
    QList<HtmlParser::Part> res = p.parse();
    QCOMPARE(res.count(), 1);
    QCOMPARE(res[0].contents, QString("a{color:red}"));
    QCOMPARE(res[0].range.start.line, 0);
    QCOMPARE(res[0].range.start.column, 29);
    QCOMPARE(res[0].range.end.line, 0);
    QCOMPARE(res[0].range.end.column, 41);
}

void TestParser::htmlInlineStyle()
{
    HtmlParser p;
                 //0         1          2          3         4
                 //012345678901234 5678901234 5678901234567890123456789
    p.setContents("<html><p style=\"color:red\"></p></html>");
    QList<HtmlParser::Part> res = p.parse();
    QCOMPARE(res.count(), 1);
    QCOMPARE(res[0].contents, QString("color:red"));
    QCOMPARE(res[0].range.start.line, 0);
    QCOMPARE(res[0].range.start.column, 16);
    QCOMPARE(res[0].range.end.line, 0);
    QCOMPARE(res[0].range.end.column, 25);
    QCOMPARE(res[0].tag, QString("p"));
}

void TestParser::htmlInlineStyle2()
{
    HtmlParser p;
                 //0         1           2          3          4
                 //0123456789012345 6789 01234567 8901234567 890123456789
    p.setContents("<html>  <div id=\"foo\" style=\"color:red\"></html>");
    QList<HtmlParser::Part> res = p.parse();
    QCOMPARE(res.count(), 1);
    QCOMPARE(res[0].contents, QString("color:red"));
    QCOMPARE(res[0].range.start.line, 0);
    QCOMPARE(res[0].range.start.column, 29);
    QCOMPARE(res[0].range.end.line, 0);
    QCOMPARE(res[0].range.end.column, 38);
}

void TestParser::htmlInlineStyle3()
{
    HtmlParser p;
                 //0                         1                   2      3
                 //0         1           2   0          1        0      0
                 //0123456789012345 6789 01 2012345 67890123456 7012 3 40123456
    p.setContents("<html>  <div id=\"foo\" \nstyle=\"color:red;\nabc\"\n></html>");
    QList<HtmlParser::Part> res = p.parse();
    QCOMPARE(res.count(), 1);
    QCOMPARE(res[0].contents, QString("color:red;\nabc"));
    QCOMPARE(res[0].range.start.line, 1);
    QCOMPARE(res[0].range.start.column, 7);
    QCOMPARE(res[0].range.end.line, 2);
    QCOMPARE(res[0].range.end.column, 3);
}


}

#include "parsertest.moc"
