--  This file is part of KDevelop
--
--  Copyright 2010 Ruan Strydom <ruan@jcell.co.za>
--
--
--  This library is free software; you can redistribute it and/or
--  modify it under the terms of the GNU Lesser General Public
--  License as published by the Free Software Foundation; either
--  version 2 of the License, or (at your option) any later version.
--
--  This library is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
--  Lesser General Public License for more details.
--
--  You should have received a copy of the GNU Lesser General Public
--  License along with this library; if not, write to the Free Software
--  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
--

[:

#include <QtCore/QString>
#include <QtCore/QList>
#include <kdebug.h>
#include <language/interfaces/iproblem.h>
#include "sgmllexer.h"
#include "idtdhelper.h"

namespace KDevelop
{
    class DUContext;
}

:]


------------------------------------------------------------
-- Export macro to use the parser in a shared lib
------------------------------------------------------------
%export_macro "KDEVXMLPARSER_EXPORT"
%export_macro_header "parserexport.h"


------------------------------------------------------------
-- Enumeration types for additional AST members,
-- in the global "Xml" namespace
------------------------------------------------------------
%namespace
[:
    class Tokenizer;
:]

%token_stream TokenStream ;;

------------------------------------------------------------
-- Parser class members
------------------------------------------------------------

%parserclass (public declaration)
[:
  /**
   * Transform the raw input into tokens.
   * When this method returns, the parser's token stream has been filled
   * and any parse*() method can be called.
   */
  void tokenize(const QString& contents);

  enum ProblemType {
      Error,
      Warning,
      Info
  };
  void reportProblem( Parser::ProblemType type, const QString& message );
  QList<KDevelop::ProblemPointer> problems() {
      return m_problems;
  }
  QString tokenText(qint64 begin, qint64 end) const;
  void setDebug(bool debug);
  void setCurrentDocument(QString url);

:]

%parserclass (private declaration)
[:
    QString m_contents;
    bool m_debug;
    QString m_currentDocument;
    QList<KDevelop::ProblemPointer> m_problems;
    const IDtdHelper *m_dtdHelper;
    QString tagName(const ElementTagAst *ast) const;
    QString tagName(const ElementCloseTagAst *ast) const;
    QStack<ElementTagAst *> m_stack;
:]



-----------------------------------------------------------
-- List of defined tokens
-----------------------------------------------------------

---------------
-- HTML/XML TOKENS
---------------

%token TEXT ("text"),
       COMMENT ("<!-- -->"),
       LT ("<"),
       GT (">"),
       CLOSE("</ />"),
       --LTFS ("</"),
       --FSGT ("/>"),
       QUOTE ("quote"),
       EQUAL ("="),
       COLON (":"),
       CDATA ("<![CDATA[ ]>"),       --Treated seperately because of <![ in DTD
       PCDATA ("<![PCDATA[ ]>"),     --Treated seperately because of <![ in DTD
       PROC_START ("<?xml"),
       PROC_END ("?>"),
       PHP_START ("<?php"),
       PHP_END ("?>"),
       SRC_START ("<%"),                --<% TEXT %> ie: JSP
       SRC_END ("%>"),
       WHITE ("white");;

---------------
-- DTD TOKENS
---------------

%token DECLARE_START("<!"),
       DOCTYPE("DOCTYPE"),
       ELEMENT("ELEMENT"),
       ATTLIST("ATTLIST"),
       ENTITY("ENTITY"),
       NOTATION("NOTATION"),
       PUBLIC("PUBLIC"),
       SYSTEM("SYSTEM"),
       ANY("ANY"),
       IMPLIED("IMPLIED"),
       REQUIRED("REQUIRED"),
       FIXED("FIXED"),
       EMPTY("EMPTY"),
       COND_START("<!["),               --TODO Not used - DTD Condition
       COND_END("]]>"),                 --TODO Not used - DTD Condition
       OBRACKET ("["),
       CBRACKET ("]"),
       OPAREN ("("),
       CPAREN (")"),
       VBAR("|"),
       PLUS("+"),
       OPT("O"),                        --Optional tag
       HYPHEN("-"),                     --Required tag
       ASTERISK("*"),
       COMMA,
       PERCENT("%"),
       NUMBER("#"),
       SEMICOLON(";"),
       QUESTION("?");;

-- data types
%token NDATA("NDATA"),
       CDATA("CDATA"),
       PCDATA("PCDATA"),
       SDATA("SDATA"),
       NMTOKEN("NMTOKEN"),
       ID("ID");;

-- lexer error
%token ERROR ("error") ;;

-- token that makes the parser fail in any case:
%token INVALID ("invalid token") ;;


---------------------------------------------------------------------------------
-- XML etc grammar starts here.
-- NOTE
-- The parser do not create a tree at the moment the logic was that DTD and the mime
-- defines how tag/elements are opened and closed ?
-- The builder can easily push pop the ast nodes based on the mime and/or included DTD ?
---------------------------------------------------------------------------------

    (#elements=element)*
->  start ;;


    --TODO how to get rid of element
    elementCloseTag
    | element=elementText
    | element=elementPCDATA
    | element=elementCDATA
    | element=elementPHP
    | element=elementTag
    | element=elementSource
    | element=elementProcessing
    | element=elementComment
    | element=elementDTD
    | error=error
->  element [
        member variable element: AstNode*;
    ];;


    WHITE+ | 0
->  maybeWhites ;;

    WHITE+
->  whites ;;

    TEXT+ | 0
->  maybeText ;;


    TEXT
->  text ;;


    TEXT
->  elementText ;;


    --NOTE safe but irritating FIRST/FIRST conflict
    (?[: LA(2).kind == Token_COLON :] (ns=text COLON name=text) | (name=text))
    (
        maybeWhites EQUAL maybeWhites
        ((QUOTE (value=text | 0) QUOTE) | value=text)
    ) | 0
->  attribute ;;

    --NOTE safe but irritating FIRST/FIRST conflict
    --TODO Supported tags marked with *:
    --  <sgml>                  * root not a problem
    --      <child1>            
    --          blah, blah
    --          <child1.1>
    --      <child2>
    --          <child2.1/>     *supported using dtdHelper not a problem
    --          <child2.2>      Problem as DTD will specify that child2.2 belongs actualy to child2
    --      </child2>           * will correctly pop to child2

    LT maybeWhites
    (?  [: LA(2).kind == Token_COLON :] (ns=text COLON name=text)
        | (name=text)
    ) maybeWhites
    #attributes=attribute @ whites
    (
        CLOSE [: while(m_dtdHelper && !m_stack.empty() && !m_dtdHelper->hasChild(tagName(m_stack.top()), tagName(*yynode))) m_stack.pop(); :]
        | GT [: m_stack.push(*yynode); :] (#children=element [: if(m_stack.empty() || m_stack.top() != *yynode) break; :])*
    )
->  elementTag ;;

    CLOSE maybeWhites
    (?  [: LA(2).kind == Token_COLON :] (ns=text COLON name=text)
        | (name=text)
    ) maybeWhites
    GT [: while(!m_stack.empty() && tagName(m_stack.top()) != tagName(*yynode)) m_stack.pop(); :]
->  elementCloseTag ;;


    CDATA
->  elementCDATA ;;


    PCDATA
->  elementPCDATA ;;


    PHP_START
    (text=text | 0)
    PHP_END
->  elementPHP ;;


    SRC_START
    (text=text | 0)
    SRC_END
->  elementSource ;;


    PROC_START
    (text=text | 0)
    PROC_END
->  elementProcessing ;;


    COMMENT
->  elementComment ;;


    --FIXME does not work here
    ERROR [: reportProblem( Error, "Sequence not allowed here" ); :]
->  error ;;

---------------------------------------------------------------------------------
-- DTD grammar starts here.
-- NOTE
-- The lexer skips over [ ] sections
---------------------------------------------------------------------------------

    DECLARE_START maybeWhites
    DOCTYPE maybeWhites
    name=text maybeWhites
    (PUBLIC maybeWhites QUOTE publicId=maybeText QUOTE maybeWhites (QUOTE systemId=maybeText QUOTE  | 0) | 0)
    (SYSTEM maybeWhites QUOTE systemId=maybeText QUOTE | 0)
    maybeWhites
    GT
->  elementDTD ;;

-----------------------------------------------------------------
-- Code segments copied to the implementation (.cpp) file.
-- If existent, kdevelop-pg's current syntax requires this block
-- to occur at the end of the file.
-----------------------------------------------------------------

[:

#include <QtCore/QDebug>
#include <KTextEditor/Range>
#include "sgmllexer.h"

namespace Xml
{

void Parser::tokenize(const QString& contents)
{
    m_contents = contents;
    SgmlLexer lexer(tokenStream, contents);
    int kind = Parser::Token_EOF;

    do
    {
        kind = lexer.nextTokenKind();
        if ( !kind ) // when the lexer returns 0, the end of file is reached
        {
            kind = Parser::Token_EOF;
        }
        Parser::Token &t = tokenStream->next();
        t.begin = lexer.tokenBegin();
        t.end = lexer.tokenEnd();
        t.kind = kind;
        if ( m_debug ) qDebug() << kind << tokenText(t.begin,t.end) << t.begin << t.end;
    }
    while ( kind != Parser::Token_EOF );

    m_dtdHelper = lexer.dtdHelper(); //If doctype found this will be non null.

    yylex(); // produce the look ahead token
}


QString Parser::tokenText(qint64 begin, qint64 end) const
{
    return m_contents.mid(begin,end-begin+1);
}


QString Parser::tagName(const ElementTagAst *ast) const
{
    if(!ast)
        return QString();
    if(ast->ns && ast->name)
        return tokenText(ast->ns->startToken, ast->ns->endToken) + ":" + tokenText(ast->name->startToken, ast->name->endToken);
    if(ast->name)
        return tokenText(ast->name->startToken, ast->name->endToken);
}

QString Parser::tagName(const ElementCloseTagAst *ast) const
{
    if(!ast)
        return QString();
    if(ast->ns && ast->name)
        return tokenText(ast->ns->startToken, ast->ns->endToken) + ":" + tokenText(ast->name->startToken, ast->name->endToken);
    if(ast->name)
        return tokenText(ast->name->startToken, ast->name->endToken);
}

void Parser::reportProblem( Parser::ProblemType type, const QString& message )
{
    if (type == Error)
        qDebug() << "** ERROR:" << message;
    else if (type == Warning)
        qDebug() << "** WARNING:" << message;
    else if (type == Info)
        qDebug() << "** Info:" << message;

    qint64 sLine;
    qint64 sCol;
    qint64 index = tokenStream->index()-1;
    tokenStream->startPosition(index, &sLine, &sCol);
    qint64 eLine;
    qint64 eCol;
    tokenStream->endPosition(index, &eLine, &eCol);
    KDevelop::Problem *p = new KDevelop::Problem();
    p->setSource(KDevelop::ProblemData::Parser);
    switch ( type ) {
        case Error:
            p->setSeverity(KDevelop::ProblemData::Error);
            break;
        case Warning:
            p->setSeverity(KDevelop::ProblemData::Warning);
            break;
        case Info:
            p->setSeverity(KDevelop::ProblemData::Hint);
            break;
    }
    p->setDescription(message);
    p->setFinalLocation(KDevelop::DocumentRange(m_currentDocument, KTextEditor::Range(sLine, sCol, eLine, eCol+1)));
    m_problems << KDevelop::ProblemPointer(p);
}


// custom error recovery
void Parser::expectedToken(int /*expected*/, qint64 /*where*/, const QString& name)
{
    reportProblem( Parser::Error, QString("Expected token \"%1\"").arg(name));
}

void Parser::expectedSymbol(int /*expectedSymbol*/, const QString& name)
{
    qint64 line;
    qint64 col;
    qint64 index = tokenStream->index()-1;
    Token &token = tokenStream->token(index);
    kDebug() << "token starts at:" << token.begin;
    kDebug() << "index is:" << index;
    tokenStream->startPosition(index, &line, &col);
    QString tokenValue = tokenText(token.begin, token.end);
    qint64 eLine;
    qint64 eCol;
    tokenStream->endPosition(index, &eLine, &eCol);
    reportProblem( Parser::Error,
                   QString("Expected symbol \"%1\" (current token: \"%2\" [%3] at %4:%5 - %6:%7)")
                  .arg(name)
                  .arg(token.kind != 0 ? tokenValue : "EOF")
                  .arg(token.kind)
                  .arg(line)
                  .arg(col)
                  .arg(eLine)
                  .arg(eCol));
}

void Parser::setDebug( bool debug )
{
    m_debug = debug;
}

void Parser::setCurrentDocument(QString url)
{
    m_currentDocument = url;
}

} // end of namespace Xml

:]

-- kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
