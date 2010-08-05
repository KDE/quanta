--  This file is part of KDevelop
--
--  Copyright 2009 Niko Sams <niko.sams@gmail.com>
--
--  This file is based on WebCore/css/CSSGrammar.y
--  Copyright (C) 2002-2003 Lars Knoll (knoll@kde.org)
--  Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
--  Copyright (C) 2006 Alexey Proskuryakov (ap@nypop.com)
--  Copyright (C) 2008 Eric Seidel <eric@webkit.org>
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
#include <kdebug.h>
#include <language/interfaces/iproblem.h>
#include "parser/editorintegrator.h"

:]


------------------------------------------------------------
-- Export macro to use the parser in a shared lib
------------------------------------------------------------
%export_macro "KDEVCSSPARSER_EXPORT"
%export_macro_header "parser/parserexport.h"


------------------------------------------------------------
-- Enumeration types for additional AST members,
-- in the global "Css" namespace
------------------------------------------------------------
%namespace
[:
    class Tokenizer;
:]

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
  QString tokenText(qint64 begin, qint64 end);
  void setDebug(bool debug);
  void setCurrentDocument(QString url);

:]

%parserclass (private declaration)
[:
    QString m_contents;
    bool m_debug;
    QString m_currentDocument;
    QList<KDevelop::ProblemPointer> m_problems;
:]



-----------------------------------------------------------
-- List of defined tokens
-----------------------------------------------------------

-- keywords:<
%token IMPORT_SYM ("@import"), PAGE_SYM ("@page"), MEDIA_SYM ("@media"),
    FONT_FACE_SYM ("@font-face"), CHARSET_SYM ("@charset"), NAMESPACE_SYM ("@namespace"),
    IMPORTANT_SYM ("!important") ;;

%token WHITESPACE ("whitespace"), IDENT ("identifier"), STRING ("string"), SGML_CD ("<!--/-->"),
    NOTFUNCTION ("not function"), URI ("uri"), FUNCTION ("function"), UNICODERANGE ("unicoderange"),
    HEXCOLOR ("hex color"), NTH ("nth"), IDSEL ("id selector"),
    MEDIA_NOT ("media not"), MEDIA_ONLY ("media only"), MEDIA_AND ("media and") ;;

-- seperators:
%token COMMA (","), LPAREN ("("), RPAREN (")"), LBRACE ("{"), RBRACE ("}"),
    LBRACKET ("["), RBRACKET ("]"),
    COLON (":"), STAR("*"), SEMICOLON (";"), DOT ("."), EQUALS("="),
    INCLUDES ("~="), DASHMATCH ("|="), BEGINSWITH ("^="), ENDSWITH ("$="),
    CONTAINS ("*="), MINUS ("-"), PLUS ("+"), DIVIDE ("/") ;;

%token EMS ("em"), QEMS ("__qem"), EXS ("ex"), PXS ("ps"), CMS ("cm"), MMS ("mm"),
    INS ("in"), PTS ("pt"), PCS ("pc"), DEGS ("deg"), RADS ("rad"), GRADS ("grad"),
    MSECS ("ms"), SECS ("s"), HERZ ("Hz"), DPI ("dpi"), DPCM ("dpcm"), KHERZ ("kHz"),
    DIMEN ("dimen"), PERCENTAGE ("%"), INTEGER ("integer"), FLOAT ("float") ;;

-- token that makes the parser fail in any case:
%token INVALID ("invalid token") ;;



-- The actual grammar starts here.

-- these rule isn't used by the parser, it's as ast node
-- to support multiple style elements in one html file
  0
-> html [
     member variable elements: QList<AstNode*>;
];;

  start=start
-> styleElement [
     member variable session: ParseSession*;
];;

  declarationList=declarationList
-> inlineStyle [
     member variable session: ParseSession*;
];;
-- end custom rules


  (charset=charset | 0)
  maybeSgml
  imports=importList
  --variablesList
  --namespaceList
  rules=ruleList
-> start ;;

  WHITESPACE+ | 0
-> maybeSpace ;;

maybeSpace (SGML_CD @ maybeSpace | 0)
-> maybeSgml ;;


-- closing_brace:
--     '}'
--   | %prec LOWEST_PREC TOKEN_EOF
--   ;

    CHARSET_SYM maybeSpace STRING maybeSpace SEMICOLON maybeSpace
-> charset ;;

  (#imports=import maybeSgml)*
-> importList ;;

--     0
--   | variablesList variablesRule maybeSgml
-- -> variablesList ;;

--    0
--  | namespaceList namespace maybeSgml
---> namespaceList ;;

(#rules=rule maybeSgml)*
-> ruleList ;;

    ruleset=ruleset
--  | media
--  | page
--  | font_face
--  | keyframes
-> rule ;;
--
-- block_ruleList:
--     /* empty */ { $$ = 0; }
--   | block_ruleList block_rule maybeSgml
--   ;
-- 
-- block_valid_rule:
--     ruleset
--   | page
--   | font_face
--   | keyframes
--   ;
-- 
-- block_rule:
--     block_valid_rule
--   | invalid_rule
--   | invalid_at
--   | invalid_import
--   | invalid_media
--   ;

    --IMPORT_SYM maybeSpace stringOrUri maybeSpace maybeMediaList SEMICOLON
    IMPORT_SYM maybeSpace stringOrUri maybeSpace SEMICOLON maybeSpace
-> import ;;

-- variablesRule:
--     WEBKIT_VARIABLES_SYM maybeSpace maybeMediaList '{' maybeSpace variables_declaration_list '}'
--     |
--     WEBKIT_DEFINE_SYM maybeSpace variables_mediaList '{' maybeSpace variables_declaration_list '}'
--     ;
-- 
-- variables_mediaList:
--     /* empty */
--     |
--     VARIABLES_FOR WHITESPACE mediaList
--     ;
-- 
-- variables_declaration_list:
--     variables_declaration
--     | variables_decl_list variables_declaration
--     | variables_decl_list
--     | error invalid_block_list error
--     | error
--     | variables_decl_list error
--     ;
-- 
-- variables_decl_list:
--     variables_declaration ';' maybeSpace
--     | variables_declaration invalid_block_list ';' maybeSpace
--     | error ';' maybeSpace
--     | error invalid_block_list error ';' maybeSpace
--     | variables_decl_list variables_declaration ';' maybeSpace
--     | variables_decl_list error ';' maybeSpace
--     | variables_decl_list error invalid_block_list error ';' maybeSpace
--     ;
-- 
-- variables_declaration:
--     variable_name ':' maybeSpace expr
--     |
--     variable_name maybeSpace '{' maybeSpace declaration_list '}' maybeSpace
--     |
--     variable_name error
--     |
--     variable_name ':' maybeSpace error expr
--     |
--     variable_name ':' maybeSpace /* @variables { varname: } Just reduce away this variable with no value. */
--     |
--     variable_name ':' maybeSpace error /* if we come across rules with invalid values like this case: @variables { varname: *; }, just discard the property/value pair */
--     ;
-- 
-- variable_name:
--     IDENT maybeSpace
--     ;
-- 
-- namespace:
-- NAMESPACE_SYM maybeSpace maybe_ns_prefix stringOrUri maybeSpace ';'
-- | NAMESPACE_SYM error invalid_block
-- | NAMESPACE_SYM error ';'
-- ;
-- 
-- maybe_ns_prefix:
-- /* empty */
-- | IDENT WHITESPACE
-- ;

    string=STRING
  | uri=URI
-> stringOrUri ;;

-- media_feature:
--     IDENT maybeSpace
--     ;
-- 
-- maybe_media_value:
--     /*empty*/
--     | ':' maybeSpace expr maybeSpace
--     ;
-- 
-- mediaQuery_exp:
--     '(' maybeSpace media_feature maybeSpace maybe_media_value ')' maybeSpace
--     ;
-- 
-- mediaQueryExpList:
--     mediaQuery_exp
--     | mediaQueryExpList maybeSpace MEDIA_AND maybeSpace mediaQuery_exp
--     ;
-- 
-- maybe_and_mediaQueryExpList:
--     /*empty*/
--     | MEDIA_AND maybeSpace mediaQueryExpList
--     ;
-- 
-- maybe_media_restrictor:
--     /*empty*/
--     | MEDIA_ONLY
--     | MEDIA_NOT
--     ;
-- 
-- mediaQuery:
--     mediaQueryExpList
--     |
--     maybe_media_restrictor maybeSpace medium maybe_and_mediaQueryExpList
--     ;

--    0
--  | mediaList
---> maybeMediaList ;;

--    mediaQuery
--  | mediaList COMMA maybeSpace mediaQuery
--  | mediaList error
---> mediaList ;;

-- media:
--     MEDIA_SYM maybeSpace mediaList '{' maybeSpace block_ruleList save_block
--     | MEDIA_SYM maybeSpace '{' maybeSpace block_ruleList save_block
--     ;
-- 
-- medium:
--   IDENT maybeSpace
--   ;
-- 
-- keyframes:
--     WEBKIT_KEYFRAMES_SYM maybeSpace keyframe_name maybeSpace '{' maybeSpace keyframes_rule '}'
--     ;
-- 
-- keyframe_name:
--     IDENT
--     | STRING
--     ;
-- 
-- keyframes_rule:
--     /* empty */
--     | keyframes_rule keyframe_rule maybeSpace
--     ;
-- 
-- keyframe_rule:
--     key_list maybeSpace '{' maybeSpace declaration_list '}'
--     ;
-- 
-- key_list:
--     key
--     | key_list maybeSpace ',' maybeSpace key
--     ;
-- 
-- key:
--     PERCENTAGE
--     | IDENT
--     ;
-- 
-- /*
-- page:
--     PAGE_SYM maybeSpace IDENT? pseudo_page? maybeSpace
--     '{' maybeSpace declaration [ ';' maybeSpace declaration ]* '}' maybeSpace
--   ;
-- 
-- pseudo_page
--   : ':' IDENT
--   ;
-- */
-- 
-- page:
--     PAGE_SYM error invalid_block
--   | PAGE_SYM error ';'
--     ;
-- 
-- font_face:
--     FONT_FACE_SYM maybeSpace
--     '{' maybeSpace declaration_list '}'  maybeSpace
--     | FONT_FACE_SYM error invalid_block
--     | FONT_FACE_SYM error ';'
-- ;
-- 
-- combinator:
--     '+' maybeSpace
--   | '~' maybeSpace
--   | '>' maybeSpace
--   ;

    MINUS
  | PLUS
-> unaryOperator ;;

  selectors=selectorList (lbrace=LBRACE | 0 [: reportProblem( Error, "Expected '{'" ); :]) maybeSpace declarations=declarationList
  (  rbrace=RBRACE
   | 0 [: reportProblem( Error, "Expected '}'" ); :])
-> ruleset ;;

--selector %prec UNIMPORTANT_TOK
--   | selectorList ',' maybeSpace selector %prec UNIMPORTANT_TOK
--   | selectorList error
#selectors=selector
    @ (COMMA maybeSpace
        [: if(LA(1).kind == Token_LBRACE || LA(1).kind == Token_EOF) {
            reportProblem( Error, "Expected Selector" );
            break;}
        :])
-> selectorList ;;

--     | selector combinator simpleSelector
--     | selector error
   simpleSelector=simpleSelectorWithWhitespace+
-> selector ;;

    simpleSelector=simpleSelector maybeSpace
-> simpleSelectorWithWhitespace ;;

--     /* empty */ '|'
--     | STAR '|'
--     | IDENT '|'
-- -> namespaceSelector ;;

    element=elementName (specifier=specifierList | 0)
  | specifier=specifierList
--     | namespace_selector elementName
--     | namespace_selector elementName specifierList
--     | namespace_selector specifierList
-> simpleSelector ;;

  ident=IDENT | star=STAR
-> elementName ;;

    (#specifiers=specifier)+
-> specifierList ;;

    DOT className=IDENT
  | attrib=attrib
  | pseudo=pseudo
  | idSel=IDSEL
--   | HEX
-> specifier ;;

--     | LBRACKET maybeSpace namespace_selector IDENT maybeSpace RBRACKET
--     | LBRACKET maybeSpace namespace_selector IDENT maybeSpace match maybeSpace identOrString maybeSpace RBRACKET
    LBRACKET
        maybeSpace attrName=IDENT maybeSpace
        (match=match maybeSpace ident=identOrString maybeSpace | 0)
    RBRACKET
-> attrib ;;

    EQUALS
  | INCLUDES
  | DASHMATCH
  | BEGINSWITH
  | ENDSWITH
  | CONTAINS
-> match ;;

    ident=IDENT
  | string=STRING
-> identOrString ;;

    COLON (
        ident=IDENT
      | FUNCTION (
          NTH     --used by :nth-*(ax+b)
        | INTEGER --used by :nth-*
        | IDENT   --used by :nth-*(odd/even) and :lang
      ) RPAREN
      | NOTFUNCTION maybeSpace simpleSelector maybeSpace RPAREN --used by :not
      | COLON ident=IDENT --used by ::selection (I guess)
    )
-> pseudo ;;

    (#declarations=declaration)+ | 0
-> declarationList ;;

--     variable_reference maybeSpace
--     |
--     property error
--     |
--     property ':' maybeSpace error expr prio {
--         /* The default movable type template has letter-spacing: .none;  Handle this by looking for
--         error tokens at the start of an expr, recover the expr and then treat as an error, cleaning
--         up and deleting the shifted expr.  */
--         $$ = false;
--     }
--     |
--     property ':' maybeSpace expr prio error {
--         /* When we encounter something like p {color: red !important fail;} we should drop the declaration */
--         $$ = false;
--     }
--     |
--     IMPORTANT_SYM maybeSpace {
--         /* Handle this case: div { text-align: center; !important } Just reduce away the stray !important. */
--         $$ = false;
--     }
--     |
--     property ':' maybeSpace {
--         /* div { font-family: } Just reduce away this property with no value. */
--         $$ = false;
--     }
--     |
--     property ':' maybeSpace error {
--         /* if we come across rules with invalid values like this case: p { weight: *; }, just discard the rule */
--         $$ = false;
--     }
--     |
--     property invalid_block {
--         /* if we come across: div { color{;color:maroon} }, ignore everything within curly brackets */
--         $$ = false;
--     }
  property=property
    (colon=COLON | 0 [: reportProblem( Error, "Expected Colon" ); :])
    maybeSpace
    --don't parse next property as expr (happens if there is no expr and semicolon)
    [: if (LA(2).kind != Token_COLON && !(LA(2).kind == Token_WHITESPACE && LA(3).kind == Token_COLON)) { :]
    expr=expr prio
    (semicolon=SEMICOLON | 0 )
    maybeSpace
    [: } :]
-> declaration ;;


  maybeSpace ident=IDENT maybeSpace
-> property ;;

IMPORTANT_SYM maybeSpace | 0
-> prio ;;


    -- term+ as adding |0 to operator gives a conflict
    #terms=term+ @ operator
  | 0 [: reportProblem( Error, "Expected exprssion" ); :]
-> expr ;;

    DIVIDE maybeSpace
  | COMMA maybeSpace
-> operator ;;

--   | '#' maybeSpace /* Handle error case: "color: #;" */
--   | variable_reference maybeSpace
--   | '%' maybeSpace /* Handle width: %; */
    string=STRING maybeSpace
  | ident=IDENT maybeSpace
  | (op=unaryOperator | 0) (term=unaryTerm | DIMEN)
  | uri=URI maybeSpace
  | unicoderange=UNICODERANGE maybeSpace
  | hexcolor=hexcolor
  | function=function --   /* FIXME: according to the specs a function can have a unaryOperator in front. I know no case where this makes sense */
-> term ;;


    value=INTEGER maybeSpace
  | value=FLOAT maybeSpace
  | value=PERCENTAGE maybeSpace
  | value=PXS maybeSpace
  | value=CMS maybeSpace
  | value=MMS maybeSpace
  | value=INS maybeSpace
  | value=PTS maybeSpace
  | value=PCS maybeSpace
  | value=DEGS maybeSpace
  | value=RADS maybeSpace
  | value=GRADS maybeSpace
  | value=MSECS maybeSpace
  | value=SECS maybeSpace
  | value=HERZ maybeSpace
  | value=KHERZ maybeSpace
  | value=EMS maybeSpace
  | value=QEMS maybeSpace
  | value=EXS maybeSpace
--  | TURNS maybeSpace
--  | REMS maybeSpace
-> unaryTerm ;;


-- variable_reference:
--   VARCALL
--   ;

    FUNCTION maybeSpace expr RPAREN maybeSpace
    --|
    --FUNCTION maybeSpace error
-> function ;;


-- /*
--  * There is a constraint on the color that it must
--  * have either 3 or 6 hex-digits (i.e., [0-9a-fA-F])
--  * after the "#"; e.g., "#000" is OK, but "#abcd" is not.
--  */
   HEXCOLOR maybeSpace
--  | IDSEL maybeSpace
-> hexcolor ;;



-----------------------------------------------------------------
-- Code segments copied to the implementation (.cpp) file.
-- If existent, kdevelop-pg's current syntax requires this block
-- to occur at the end of the file.
-----------------------------------------------------------------

[:

#include <QtCore/QDebug>
#include <KTextEditor/Range>
#include "parser/tokenizer.h"

namespace Css
{

void Parser::tokenize(const QString& contents)
{
    m_contents = contents;

    Tokenizer tokenizer(tokenStream, contents.toLatin1().data());

    int kind = Parser::Token_EOF;
    do
    {
        kind = tokenizer.yylex();

        if ( !kind ) // when the lexer returns 0, the end of file is reached
        {
            kind = Parser::Token_EOF;
        }
        Parser::Token &t = tokenStream->next();
        t.begin = tokenizer.tokenBegin();
        t.end = tokenizer.tokenEnd();
        t.kind = kind;
        //if ( m_debug ) qDebug() << kind << tokenText(t.begin,t.end) << t.begin << t.end;
    }
    while ( kind != Parser::Token_EOF );

    yylex(); // produce the look ahead token
}


QString Parser::tokenText(qint64 begin, qint64 end)
{
    return m_contents.mid(begin,end-begin+1);
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

} // end of namespace Css

:]

-- kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
