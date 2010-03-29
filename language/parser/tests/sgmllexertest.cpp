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

#include "sgmllexertest.h"
#include "../sgmllexer.h"
#include "sgmlparser.h"

#include <QTest>
#include <stringreflist.h>

QTEST_MAIN(Xml::TestSgmlLexer)

namespace Xml
{

TestSgmlLexer::TestSgmlLexer()
{
}

void TestSgmlLexer::testDoctype()
{
    SgmlLexer lexer(NULL,"");
    QString token1 = "<!doctype html>";
    QString token2 = "<!DOCTYPE html PUBLIC \"publicId\">";
    QString token3 = "<!DOCTYPE html PUBLIC \"publicId\" \"systemId\">";
    QString token4 = "<!DOCTYPE html SYSTEM \"systemId\">";
    QString token5 = "<!DOCTYPE html SYSTEM>";
    QString name;
    QString publicId;
    QString systemId;
    lexer.doctype(token1, name, publicId, systemId);
    if(name != "html")
        QFAIL(QString("EXPECTED 'html' GOT '%1'").arg(name).toAscii().constData());
    
    lexer.doctype(token2, name, publicId, systemId);
    if(publicId != "publicId")
        QFAIL(QString("EXPECTED 'publicId' GOT '%1'").arg(publicId).toAscii().constData());

    lexer.doctype(token3, name, publicId, systemId);
    if(systemId != "systemId")
        QFAIL(QString("EXPECTED 'systemId' GOT '%1'").arg(systemId).toAscii().constData());

    lexer.doctype(token4, name, publicId, systemId);
    if(systemId != "systemId")
        QFAIL(QString("EXPECTED 'systemId' GOT '%1'").arg(systemId).toAscii().constData());

    lexer.doctype(token5, name, publicId, systemId);
    if(name != "html")
        QFAIL(QString("EXPECTED 'html' GOT '%1'").arg(name).toAscii().constData());
}

void TestSgmlLexer::testElementName()
{
    
    SgmlLexer lexer(NULL,"");
    QString tagName = lexer.elementName("<ns:tagName attrib='value'/>");
    if(tagName != "TAGNAME") {
         QFAIL(QString("EXPECTED 'TAGNAME' GOT '%1'").arg(tagName).toAscii().constData());
    }
    tagName = lexer.elementName("</\r\n\t ns:tagName >");
    if(tagName != "TAGNAME") {
         QFAIL(QString("EXPECTED 'TAGNAME' GOT '%1'").arg(tagName).toAscii().constData());
    }
    tagName = lexer.elementName("</tagName>");
    if(tagName != "TAGNAME") {
         QFAIL(QString("EXPECTED 'TAGNAME' GOT '%1'").arg(tagName).toAscii().constData());
    }
    
}

void TestSgmlLexer::testStringRefList()
{
    QString test = "%blah %blah ha!";
    StringRefStream strl(&test);
    if(test.length() != strl.length()) {
        QFAIL("Lengths do not match");
    }
    if(strl.str() != test) {
        QFAIL("Expected test");
    }
    strl.replace("blah", QStringRef(&test, 5, 1));
    strl.replace("%", QStringRef(&test, 5, 1));
    kDebug() << strl.str();
}

void TestSgmlLexer::testReadUntill()
{
    SgmlLexer lexer(NULL,"");
    QString test = "<![CDATA ] ] >";
    QChar *start = test.data();
    QChar *ret = lexer.readUntill(test.data(), test.data()+test.length(), "]]>", SgmlLexer::IgnoreWhites, &start);
    QStringRef ref(&test, 0, ret - test.data());
    
    if(ref.toString() != test)
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg(test,ref.toString()).toAscii().constData());
    if(ret->unicode() != '\0') 
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("0",*ret).toAscii().constData());
    if(start->unicode() != ']') 
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("]",*start).toAscii().constData());
    lexer.readUntill(test.data(), test.data()+test.length(), "xxx", SgmlLexer::IgnoreWhites, &start);
    
}

void TestSgmlLexer::testReadUntillAny()
{
    
    SgmlLexer lexer(NULL,"");
    QString test = "<![CDATA ] ] >";
    QChar * ret = lexer.readUntillAny(test.data(), test.data()+test.length(), "]]>", SgmlLexer::IgnoreWhites);
    QStringRef ref(&test, 0, ret - test.data());
    if(QString("<![CDATA ") != ref.toString())
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("<![CDATA ",ref.toString()).toAscii().constData());
    if(ret->unicode() != ']') 
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("]",*ret).toAscii().constData());
    lexer.readUntillAny(test.data(), test.data()+test.length(), "xxx", SgmlLexer::IgnoreWhites);
    
}

void TestSgmlLexer::testReadWhileAny()
{
    
    SgmlLexer lexer(NULL,"");
    QString test = "<![ CDATA ] ] >";
    QChar *start = test.data();
    QChar * ret = lexer.readWhileAny(test.data(), test.data()+test.length(), "<![", SgmlLexer::IgnoreWhites, &start);
    QStringRef ref(&test, 0, ret - test.data());
    if(QString("<![ ") != ref.toString())
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("<![ ",ref.toString()).toAscii().constData());
    if(ret->unicode() != 'C') 
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("C",*ret).toAscii().constData());
    if(start->unicode() != '<') 
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("<",*start).toAscii().constData());
    lexer.readWhileAny(test.data(), test.data()+test.length(), "<![ CDATA ] ] >", SgmlLexer::IgnoreWhites, &start);
    
}


void TestSgmlLexer::testLexer()
{
    QHash<QString, QList<Parser::TokenType> > tests;
    
    QList<Parser::TokenType> tokens1;
    tokens1.append(Parser::Token_LT);
    tokens1.append(Parser::Token_TEXT);
    tokens1.append(Parser::Token_COLON);
    tokens1.append(Parser::Token_TEXT);
    tokens1.append(Parser::Token_WHITE);
    tokens1.append(Parser::Token_TEXT);
    tokens1.append(Parser::Token_EQUAL);
    tokens1.append(Parser::Token_QUOTE);
    tokens1.append(Parser::Token_TEXT);
    tokens1.append(Parser::Token_QUOTE);
    tokens1.append(Parser::Token_CLOSE);
    tests.insert("<ns:name parm='He said: \"Hello!\"'/>", tokens1);

    QList<Parser::TokenType> tokens2;
    tokens2.append(Parser::Token_COMMENT);
    tests.insert("<!--  -->", tokens2);

    QList<Parser::TokenType> tokens3;
    tokens3.append(Parser::Token_CDATA);
    tests.insert("<![CDATA[ ]]>", tokens3);

    QList<Parser::TokenType> tokens4;
    tokens4.append(Parser::Token_PHP_START);
    tokens4.append(Parser::Token_TEXT);
    tokens4.append(Parser::Token_PHP_END);
    tests.insert("<?php ?>", tokens4);
    
    QList<Parser::TokenType> tokens5;
    tokens5.append(Parser::Token_PROC_START);
    tokens5.append(Parser::Token_PROC_END);
    tokens5.append(Parser::Token_TEXT);
    tokens5.append(Parser::Token_CDATA);
    tests.insert("<?xml?> <![CDATA[]]>", tokens5);

    QList<Parser::TokenType> tokens6;
    tokens6.append(Parser::Token_DECLARE_START);
    tokens6.append(Parser::Token_DOCTYPE);
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_TEXT);
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_GT);
    tests.insert("<!DOCTYPE root >", tokens6);
    
    QList<Parser::TokenType> tokens7;
    tokens7.append(Parser::Token_DECLARE_START);
    tokens7.append(Parser::Token_DOCTYPE);
    tokens7.append(Parser::Token_WHITE);
    tokens7.append(Parser::Token_TEXT);
    tokens7.append(Parser::Token_WHITE);
    tokens7.append(Parser::Token_PUBLIC);
    tokens7.append(Parser::Token_WHITE);
    tokens7.append(Parser::Token_QUOTE);
    tokens7.append(Parser::Token_TEXT);
    tokens7.append(Parser::Token_QUOTE);
    tokens7.append(Parser::Token_WHITE);
    tokens7.append(Parser::Token_QUOTE);
    tokens7.append(Parser::Token_TEXT);
    tokens7.append(Parser::Token_QUOTE);
    tokens7.append(Parser::Token_GT);
    tests.insert("<!doctype root PUBLIC \"uri\" \"uri\"[<!--  --><!ELEMENT child >]>", tokens7);
    
    QList<Parser::TokenType> tokens8;
    tokens8.append(Parser::Token_LT);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_EQUAL);
    tokens8.append(Parser::Token_QUOTE);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_QUOTE);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_COLON);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_EQUAL);
    tokens8.append(Parser::Token_QUOTE);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_QUOTE);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_COLON);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_EQUAL);
    tokens8.append(Parser::Token_QUOTE);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_QUOTE);
    tokens8.append(Parser::Token_GT);
    tests.insert("<kcfg xmlns=\"http://www.kde.org/standards/kcfg/1.0\"\n\
 xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n\
 xsi:schemaLocation=\"http://www.kde.org/standards/kcfg/1.0\n\
 http://www.kde.org/standards/kcfg/1.0/kcfg.xsd\">", tokens8);

    foreach(QString key, tests.keys()) {
        SgmlLexer lexer(NULL, key);
        int token;
        int count = 0;
        while((token = lexer.nextTokenKind()) != Parser::Token_EOF) {
        //    kDebug() << key.mid(lexer.tokenBegin(), lexer.tokenEnd() - lexer.tokenBegin());
            if(token != tests[key].at(count)) {
                QStringRef tokeStr(&key, lexer.tokenBegin(), lexer.tokenEnd() - lexer.tokenBegin() + 1);
                QString fail("In [%1] EXPECTED '%2' GOT '%3' TOKEN '%4' AT '%5:%6'");
                fail = fail.arg(key).arg((int)tests[key].at(count)).arg((int)token);
                fail = fail.arg(tokeStr.toString()).arg(lexer.tokenBegin()).arg(lexer.tokenEnd());
                QFAIL(fail.toAscii().constData());
            }
            count++;
        }
        if(count != tests[key].size())
            QFAIL(QString("In [%1] Not all tokens read").arg(key).toAscii().constData());
    }
    
}

void TestSgmlLexer::testLexerForFun()
{
    
    QString text = "<!DOCTYPE html><?php var test=\"test\" ?>\n\
    <head>\n\
            <script type=\"text/javascript\">\n\
                var test = 1 < 2 ? \"1\" : \"2\";\n\
            </script>\n\
            <style>\n\
              body {\
                  background-color: #fff;\
              }\
            </style>\n\
        </head>";
    SgmlLexer lexer1(NULL, text);
    int token;
    while((token = lexer1.nextTokenKind()) != Parser::Token_EOF);

    text = "<!--Users DOCTYPE-->\n\
    <!DOCTYPE users [\n\
    <!--Simple User-->\n\
    <!ELEMENT user (userName | passWord) >\n\
    <!ATTLIST user locked (true | false) \"true\">\n\
    <!--groups.xml-->\n\
    <!ATTLIST user group CDATA #REQUIRED>\n\
    ]>";
    SgmlLexer lexer2(NULL, text);
    while((token = lexer2.nextTokenKind()) != Parser::Token_EOF){
        kDebug() << text.mid(lexer2.tokenBegin(), lexer2.tokenEnd() - lexer2.tokenBegin() + 1);
    }
}

}

#include "sgmllexertest.moc"
