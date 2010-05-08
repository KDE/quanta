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

#include "sgmltokenizertest.h"
#include "../sgmltokenizer.h"
#include "sgmlparser.h"

#include <QTest>
#include <dtdtokenizer.h>

QTEST_MAIN(Xml::TestSgmlTokenizer)

namespace Xml
{

TestSgmlTokenizer::TestSgmlTokenizer()
{
}

void TestSgmlTokenizer::testDoctype()
{
    SgmlTokenizer tokenizer(NULL,"");
    QString token1 = "<!doctype html>";
    QString token2 = "<!DOCTYPE html PUBLIC \"publicId\">";
    QString token3 = "<!DOCTYPE html PUBLIC \"publicId\" \"systemId\">";
    QString token4 = "<!DOCTYPE html SYSTEM \"systemId\">";
    QString token5 = "<!DOCTYPE html SYSTEM>";
    QString name;
    QString publicId;
    QString systemId;
    tokenizer.doctype(token1, name, publicId, systemId);
    if(name != "html")
        QFAIL(QString("EXPECTED 'html' GOT '%1'").arg(name).toAscii().constData());

    tokenizer.doctype(token2, name, publicId, systemId);
    if(publicId != "publicId")
        QFAIL(QString("EXPECTED 'publicId' GOT '%1'").arg(publicId).toAscii().constData());

    tokenizer.doctype(token3, name, publicId, systemId);
    if(systemId != "systemId")
        QFAIL(QString("EXPECTED 'systemId' GOT '%1'").arg(systemId).toAscii().constData());

    tokenizer.doctype(token4, name, publicId, systemId);
    if(systemId != "systemId")
        QFAIL(QString("EXPECTED 'systemId' GOT '%1'").arg(systemId).toAscii().constData());

    tokenizer.doctype(token5, name, publicId, systemId);
    if(name != "html")
        QFAIL(QString("EXPECTED 'html' GOT '%1'").arg(name).toAscii().constData());
}

void TestSgmlTokenizer::testElementName()
{

    SgmlTokenizer tokenizer(NULL,"");
    QString tagName = tokenizer.elementName("<ns:tagName attrib='value'/>");
    if(tagName != "TAGNAME") {
         QFAIL(QString("EXPECTED 'TAGNAME' GOT '%1'").arg(tagName).toAscii().constData());
    }
    tagName = tokenizer.elementName("</\r\n\t ns:tagName >");
    if(tagName != "TAGNAME") {
         QFAIL(QString("EXPECTED 'TAGNAME' GOT '%1'").arg(tagName).toAscii().constData());
    }
    tagName = tokenizer.elementName("</tagName>");
    if(tagName != "TAGNAME") {
         QFAIL(QString("EXPECTED 'TAGNAME' GOT '%1'").arg(tagName).toAscii().constData());
    }
}

void TestSgmlTokenizer::testReadUntill()
{
    
    SgmlTokenizer tokenizer(NULL,"");
    QString test = "-            < </script>>>.";
    const QChar *start = test.data();
    const QChar *ret = tokenizer.readUntill(test.data(), test.data()+test.length(), "</script>", SgmlTokenizer::IgnoreWhites, &start);
    QStringRef ref(&test, start - test.data(), ret - start);
    if(ref.toString() != "</script>")
        QFAIL(QString("EXPECTED '</script>' GOT '%1'").arg(ref.toString()).toAscii().constData());

    test = " --";
    start = test.data();
    ret = tokenizer.readUntill(test.data(), test.data()+test.length(), "--", SgmlTokenizer::IgnoreWhites, &start);
    ref = QStringRef(&test, start - test.data(), ret - start);
    if(ref.toString() != "--")
        QFAIL(QString("EXPECTED '--' GOT '%1'").arg(ref.toString()).toAscii().constData());
}

void TestSgmlTokenizer::testReadUntillAny()
{

    SgmlTokenizer tokenizer(NULL,"");
    QString test = "<![CDATA ] ] >";
    const QChar * ret = tokenizer.readUntillAny(test.data(), test.data()+test.length(), "]]>", SgmlTokenizer::IgnoreWhites);
    QStringRef ref(&test, 0, ret - test.data());
    if(QString("<![CDATA ") != ref.toString())
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("<![CDATA ",ref.toString()).toAscii().constData());
    if(ret->unicode() != ']')
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("]",*ret).toAscii().constData());
    tokenizer.readUntillAny(test.data(), test.data()+test.length(), "xxx", SgmlTokenizer::IgnoreWhites);

}

void TestSgmlTokenizer::testReadWhileAny()
{

    SgmlTokenizer tokenizer(NULL,"");
    QString test = "<![ CDATA ] ] >";
    const QChar *start = test.data();
    const QChar * ret = tokenizer.readWhileAny(test.data(), test.data()+test.length(), "<![", SgmlTokenizer::IgnoreWhites, &start);
    QStringRef ref(&test, 0, ret - test.data());
    if(QString("<![ ") != ref.toString())
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("<![ ",ref.toString()).toAscii().constData());
    if(ret->unicode() != 'C')
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("C",*ret).toAscii().constData());
    if(start->unicode() != '<')
        QFAIL(QString("EXPECTED '%1' GOT '%2'").arg("<",*start).toAscii().constData());
    tokenizer.readWhileAny(test.data(), test.data()+test.length(), "<![ CDATA ] ] >", SgmlTokenizer::IgnoreWhites, &start);

}

void TestSgmlTokenizer::testDTDTokenizer()
{
    QHash<QString, QList<Parser::TokenType> > tests;

    QList<Parser::TokenType> tokens1;
    tokens1.append(Parser::Token_DOCTYPE);
    tokens1.append(Parser::Token_WHITE);
    tokens1.append(Parser::Token_TEXT);
    tokens1.append(Parser::Token_WHITE);
    tokens1.append(Parser::Token_PUBLIC);
    tokens1.append(Parser::Token_WHITE);
    tokens1.append(Parser::Token_QUOTE);
    tokens1.append(Parser::Token_TEXT);
    tokens1.append(Parser::Token_QUOTE);
    tokens1.append(Parser::Token_WHITE);
    tokens1.append(Parser::Token_QUOTE);
    tokens1.append(Parser::Token_TEXT);
    tokens1.append(Parser::Token_QUOTE);
    tokens1.append(Parser::Token_GT);
    tests.insert("<!doctype root PUBLIC \"uri\" \"uri\">", tokens1);

    QList<Parser::TokenType> tokens2;
    tokens2.append(Parser::Token_ELEMENT);
    tokens2.append(Parser::Token_WHITE);
    tokens2.append(Parser::Token_TEXT);
    tokens2.append(Parser::Token_WHITE);
    tokens2.append(Parser::Token_OPAREN);
    tokens2.append(Parser::Token_TEXT);
    tokens2.append(Parser::Token_COMMA);
    tokens2.append(Parser::Token_TEXT);
    tokens2.append(Parser::Token_QUESTION);
    tokens2.append(Parser::Token_CPAREN);
    tokens2.append(Parser::Token_GT);
    tests.insert("<!ELEMENT direction (left,top?)>", tokens2);

    QList<Parser::TokenType> tokens3;
    tokens3.append(Parser::Token_ELEMENT);
    tokens3.append(Parser::Token_WHITE);
    tokens3.append(Parser::Token_TEXT);
    tokens3.append(Parser::Token_WHITE);
    tokens3.append(Parser::Token_TEXT);
    tokens3.append(Parser::Token_GT);
    tests.insert("<!ELEMENT HR EMPTY>", tokens3);

    QList<Parser::TokenType> tokens4;
    tokens4.append(Parser::Token_ELEMENT);
    tokens4.append(Parser::Token_WHITE);
    tokens4.append(Parser::Token_TEXT);
    tokens4.append(Parser::Token_WHITE);
    tokens4.append(Parser::Token_OPAREN);
    tokens4.append(Parser::Token_NUMBER);
    tokens4.append(Parser::Token_TEXT);
    tokens4.append(Parser::Token_VBAR);
    tokens4.append(Parser::Token_TEXT);
    tokens4.append(Parser::Token_CPAREN);
    tokens4.append(Parser::Token_ASTERISK);
    tokens4.append(Parser::Token_GT);
    tests.insert("<!ELEMENT p (#PCDATA|I)*>", tokens4);

    QList<Parser::TokenType> tokens5;
    tokens5.append(Parser::Token_ELEMENT);
    tokens5.append(Parser::Token_WHITE);
    tokens5.append(Parser::Token_TEXT);
    tokens5.append(Parser::Token_WHITE);
    tokens5.append(Parser::Token_HYPHEN);
    tokens5.append(Parser::Token_WHITE);
    tokens5.append(Parser::Token_OPT);
    tokens5.append(Parser::Token_WHITE);
    tokens5.append(Parser::Token_TEXT);
    tokens5.append(Parser::Token_GT);
    tests.insert("<!ELEMENT P - O EMPTY>", tokens5);

    QList<Parser::TokenType> tokens6;
    tokens6.append(Parser::Token_ATTLIST); //<!ATTLIST
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_TEXT); //OPTIONS
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_TEXT); //ID
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_TEXT); //CDATA
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_NUMBER); //#
    tokens6.append(Parser::Token_TEXT); //IMPLIED
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_TEXT); //NUMBER
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_TEXT); //CDATA
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_NUMBER); //#
    tokens6.append(Parser::Token_TEXT); //IMPLIED
    tokens6.append(Parser::Token_GT);
    tests.insert("<!ATTLIST OPTIONS ID CDATA #IMPLIED NUMBER CDATA #IMPLIED>", tokens6);

    QList<Parser::TokenType> tokens7;
    tokens7.append(Parser::Token_ATTLIST);
    tokens7.append(Parser::Token_WHITE);
    tokens7.append(Parser::Token_TEXT);
    tokens7.append(Parser::Token_WHITE);
    tokens7.append(Parser::Token_TEXT);
    tokens7.append(Parser::Token_WHITE);
    tokens7.append(Parser::Token_OPAREN);
    tokens7.append(Parser::Token_TEXT);
    tokens7.append(Parser::Token_VBAR);
    tokens7.append(Parser::Token_TEXT);
    tokens7.append(Parser::Token_CPAREN);
    tokens7.append(Parser::Token_WHITE);
    tokens7.append(Parser::Token_QUOTE);
    tokens7.append(Parser::Token_TEXT);
    tokens7.append(Parser::Token_QUOTE);
    tokens7.append(Parser::Token_GT);
    tests.insert("<!ATTLIST WHERE CITY (JHB|PTA) 'JHB'--junk-->", tokens7);

    QList<Parser::TokenType> tokens8;
    tokens8.append(Parser::Token_ELEMENT);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_OPAREN);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_VBAR);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_CPAREN);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_HYPHEN);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_OPT);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_GT);
    tests.insert("<!ELEMENT (SUB|SUP) - O EMPTY>",  tokens8);

    QList<Parser::TokenType> tokens9;
    tokens9.append(Parser::Token_ATTLIST);
    tokens9.append(Parser::Token_WHITE);
    tokens9.append(Parser::Token_TEXT);
    tokens9.append(Parser::Token_WHITE);
    tokens9.append(Parser::Token_TEXT);
    tokens9.append(Parser::Token_WHITE);
    tokens9.append(Parser::Token_TEXT);
    tokens9.append(Parser::Token_WHITE);
    tokens9.append(Parser::Token_QUOTE);
    tokens9.append(Parser::Token_TEXT);
    tokens9.append(Parser::Token_QUOTE);
    tokens9.append(Parser::Token_GT);
    tests.insert("<!ATTLIST user role CDATA 'guest'>",  tokens9);

    QList<Parser::TokenType> tokens10;
    tokens10.append(Parser::Token_ELEMENT);
    tokens10.append(Parser::Token_WHITE);
    tokens10.append(Parser::Token_TEXT);
    tokens10.append(Parser::Token_WHITE);
    tokens10.append(Parser::Token_OPAREN);
    tokens10.append(Parser::Token_OPAREN);
    tokens10.append(Parser::Token_TEXT);
    tokens10.append(Parser::Token_VBAR);
    tokens10.append(Parser::Token_TEXT);
    tokens10.append(Parser::Token_CPAREN);
    tokens10.append(Parser::Token_ASTERISK);
    tokens10.append(Parser::Token_COMMA);
    tokens10.append(Parser::Token_OPAREN);
    tokens10.append(Parser::Token_TEXT);
    tokens10.append(Parser::Token_COMMA);
    tokens10.append(Parser::Token_TEXT);
    tokens10.append(Parser::Token_ASTERISK);
    tokens10.append(Parser::Token_CPAREN);
    tokens10.append(Parser::Token_CPAREN);
    tokens10.append(Parser::Token_GT);
    tests.insert("<!ELEMENT name1 ((name2|name3)*,(name4,name5*))>",  tokens10);

    foreach(QString key, tests.keys()) {
        DTDTokenizer tokenizer(NULL, key);
        int token;
        int count = 0;
        while((token = tokenizer.nextTokenKind()) != Parser::Token_EOF) {
            kDebug() << key.mid(tokenizer.tokenBegin(), tokenizer.tokenEnd() - tokenizer.tokenBegin() + 1);
            if(count >= tests[key].size())
                QFAIL(QString("In [%1] More tokens than expected").arg(key).toAscii().constData());
            if(token != tests[key].at(count)) {
                QStringRef tokeStr(&key, tokenizer.tokenBegin(), tokenizer.tokenEnd() - tokenizer.tokenBegin() + 1);
                QString fail("In [%1] EXPECTED '%2' GOT '%3' TOKEN '%4' AT '%5:%6'");
                fail = fail.arg(key).arg((int)tests[key].at(count)).arg((int)token);
                fail = fail.arg(tokeStr.toString()).arg(tokenizer.tokenBegin()).arg(tokenizer.tokenEnd());
                QFAIL(fail.toAscii().constData());
            }
            count++;
        }
        if(count != tests[key].size())
            QFAIL(QString("In [%1] Not all tokens read").arg(key).toAscii().constData());
    }
}

void TestSgmlTokenizer::testDTDEntity()
{
    QString text = "<!entity % --crap-- empty --crap-- 'EMPTY --crap--'><!entity % blah '<!element blah %empty; --crap-->'>%blah;";
    DTDTokenizer tokenizer(NULL, text);
    int token;
    int count = 0;
    while((token = tokenizer.nextTokenKind()) != Parser::Token_EOF) {
        QStringRef tokeStr(&text, tokenizer.tokenBegin(), tokenizer.tokenEnd() - tokenizer.tokenBegin() + 1);
        kDebug() << count << tokeStr.toString();
        count++;
    }
    if(count != 30)
        QFAIL(QString("In [%1] Incorrect number of tokens").arg(text).toAscii().constData());
}

void TestSgmlTokenizer::testSgmlTokenizer()
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
    tokens1.append(Parser::Token_TEXT);
    tokens1.append(Parser::Token_CLOSE);
    tests.insert("<ns:name parm=''/>", tokens1);

    QList<Parser::TokenType> tokens2;
    tokens2.append(Parser::Token_COMMENT);
    tests.insert("<!--  -->", tokens2);

    QList<Parser::TokenType> tokens3;
    tokens3.append(Parser::Token_CDATA);
    tests.insert("<![CDATA[ ]]>", tokens3);

    QList<Parser::TokenType> tokens4;
    tokens4.append(Parser::Token_PHP);
    tests.insert("<?php ?>", tokens4);

    QList<Parser::TokenType> tokens5;
    tokens5.append(Parser::Token_PROC);
    tokens5.append(Parser::Token_CDATA);
    tests.insert("<?xml?> <![CDATA[]]>", tokens5);

    QList<Parser::TokenType> tokens6;
    tokens6.append(Parser::Token_DOCTYPE);
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_TEXT);
    tokens6.append(Parser::Token_WHITE);
    tokens6.append(Parser::Token_GT);
    tests.insert("<!DOCTYPE root >", tokens6);

    QList<Parser::TokenType> tokens7;
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
    tests.insert("<!doctype root PUBLIC \"uri\" \"uri\">", tokens7);

    QList<Parser::TokenType> tokens8;
    tokens8.append(Parser::Token_LT);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_EQUAL);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_COLON);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_EQUAL);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_WHITE);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_COLON);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_EQUAL);
    tokens8.append(Parser::Token_TEXT);
    tokens8.append(Parser::Token_GT);
    tests.insert("<kcfg xmlns=\"http://www.kde.org/standards/kcfg/1.0\" \
 xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" \
 xsi:schemaLocation=\"http://www.kde.org/standards/kcfg/1.0 \
 http://www.kde.org/standards/kcfg/1.0/kcfg.xsd\">", tokens8);

    foreach(QString key, tests.keys()) {
        SgmlTokenizer tokenizer(NULL, key);
        int token;
        int count = 0;
        while((token = tokenizer.nextTokenKind()) != Parser::Token_EOF) {
        //    kDebug() << key.mid(tokenizer.tokenBegin(), tokenizer.tokenEnd() - tokenizer.tokenBegin());
            if(token != tests[key].at(count)) {
                QStringRef tokeStr(&key, tokenizer.tokenBegin(), tokenizer.tokenEnd() - tokenizer.tokenBegin() + 1);
                QString fail("In [%1] EXPECTED '%2' GOT '%3' TOKEN '%4' AT '%5:%6'");
                fail = fail.arg(key).arg((int)tests[key].at(count)).arg((int)token);
                fail = fail.arg(tokeStr.toString()).arg(tokenizer.tokenBegin()).arg(tokenizer.tokenEnd());
                QFAIL(fail.toAscii().constData());
            }
            count++;
        }
        if(count != tests[key].size())
            QFAIL(QString("In [%1] Not all tokens read").arg(key).toAscii().constData());
    }

}

void TestSgmlTokenizer::testTokenizerForFun()
{
    
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
        SgmlTokenizer tokenizer(NULL, text);
        int token;
        while ((token = tokenizer.nextTokenKind()) != Parser::Token_EOF) {
            kDebug() << text.mid(tokenizer.tokenBegin(), tokenizer.tokenEnd() - tokenizer.tokenBegin() + 1);
        }
    }
    
    {
        QString text = "<!DOCTYPE html>\n\
<html>\n\
    <head>\n\
        <script type='java'> \n\
        vfdc <  \n\
        </script><blah>\n\
        <table>\n\
            <tr>\n\
                <td/>\n\
            </tr>\n\
        </table>\n\
    </head>\n\
</html>\n\
";
        SgmlTokenizer tokenizer(NULL, text);
        int token;
        while ((token = tokenizer.nextTokenKind()) != Parser::Token_EOF) {
            kDebug() << text.mid(tokenizer.tokenBegin(), tokenizer.tokenEnd() - tokenizer.tokenBegin() + 1);
        }
    }
}

}

#include "sgmltokenizertest.moc"
