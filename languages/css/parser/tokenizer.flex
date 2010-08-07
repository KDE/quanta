%{
/*
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright 2003 Lars Knoll (knoll\@kde.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#define DONT_INCLUDE_FLEXLEXER
#include "parser/tokenizer.h"

#include "cssparser.h"

%}

%option noyywrap
%option case-insensitive
%option noyywrap
%option 8bit
%option stack
%option c++
%option yyclass="Css::Tokenizer"
%s mediaquery at_rule block

h               [0-9a-fA-F]
nonascii        [\200-\377]
unicode         \\{h}{1,6}[ \t\r\n\f]?
escape          {unicode}|\\[ -~\200-\377]
nmstart         [_a-zA-Z]|{nonascii}|{escape}
nmchar          [_a-zA-Z0-9-]|{nonascii}|{escape}
string1         \"([\t !#$%&(-~]|\\{nl}|\'|{nonascii}|{escape})*\"
string2         \'([\t !#$%&(-~]|\\{nl}|\"|{nonascii}|{escape})*\'
hexcolor        {h}{3}|{h}{6}

ident           -?{nmstart}{nmchar}*
name            {nmchar}+
num             [0-9]+|[0-9]*"."[0-9]+
intnum          [0-9]+
string          {string1}|{string2}
url             ([!#$%&*-~]|{nonascii}|{escape})*
w               [ \t\r\n\f]*
nl              \n|\r\n|\r|\f
range           \?{1,6}|{h}(\?{0,5}|{h}(\?{0,4}|{h}(\?{0,3}|{h}(\?{0,2}|{h}(\??|{h})))))
nth             (-?[0-9]*n[\+-][0-9]+)|(-?[0-9]*n)

%%

\/\*[^*]*\*+([^/*][^*]*\*+)*\/  /* ignore comments */

[ \t\r\n\f]+            {return Parser::Token_WHITESPACE;}

"<!--"                  {return Parser::Token_SGML_CD;}
"-->"                   {return Parser::Token_SGML_CD;}
"~="                    {return Parser::Token_INCLUDES;}
"|="                    {return Parser::Token_DASHMATCH;}
"^="                    {return Parser::Token_BEGINSWITH;}
"$="                    {return Parser::Token_ENDSWITH;}
"*="                    {return Parser::Token_CONTAINS;}
<mediaquery>"not"       {return Parser::Token_MEDIA_NOT;}
<mediaquery>"only"      {return Parser::Token_MEDIA_ONLY;}
<mediaquery>"and"       {return Parser::Token_MEDIA_AND;}

{string}                {return Parser::Token_STRING;}
{ident}                 {return Parser::Token_IDENT;}
{nth}                   {return Parser::Token_NTH;}


<block>"#"{hexcolor}           {return Parser::Token_HEXCOLOR;}
"#"{ident}              {return Parser::Token_IDSEL;}
 /* @rule tokens surrounding css declaration blocks with { } braces must start a BEGIN(at_rule) context */
"@import"               {BEGIN(mediaquery); return Parser::Token_IMPORT_SYM;}
"@page"                 {BEGIN(at_rule); return Parser::Token_PAGE_SYM;}
"@media"                {BEGIN(mediaquery); return Parser::Token_MEDIA_SYM;}
"@font-face"            {BEGIN(at_rule); return Parser::Token_FONT_FACE_SYM;}
"@charset"              {BEGIN(at_rule); return Parser::Token_CHARSET_SYM;}
"@namespace"        {BEGIN(at_rule); return Parser::Token_NAMESPACE_SYM; }

"!"{w}"important"         {return Parser::Token_IMPORTANT_SYM;}

{num}em                 {return Parser::Token_EMS;}
{num}__qem              {return Parser::Token_QEMS;} /* quirky ems */
{num}ex                 {return Parser::Token_EXS;}
{num}px                 {return Parser::Token_PXS;}
{num}cm                 {return Parser::Token_CMS;}
{num}mm                 {return Parser::Token_MMS;}
{num}in                 {return Parser::Token_INS;}
{num}pt                 {return Parser::Token_PTS;}
{num}pc                 {return Parser::Token_PCS;}
{num}deg                {return Parser::Token_DEGS;}
{num}rad                {return Parser::Token_RADS;}
{num}grad               {return Parser::Token_GRADS;}
{num}ms                 {return Parser::Token_MSECS;}
{num}s                  {return Parser::Token_SECS;}
{num}Hz                 {return Parser::Token_HERZ;}
<mediaquery>{num}dpi    {return Parser::Token_DPI;}
<mediaquery>{num}dpcm   {return Parser::Token_DPCM;}
{num}kHz                {return Parser::Token_KHERZ;}
{num}{ident}            {return Parser::Token_DIMEN;}
{num}%                  {return Parser::Token_PERCENTAGE;}
{intnum}                {return Parser::Token_INTEGER;}
{num}                   {return Parser::Token_FLOAT;}


"not("                  {return Parser::Token_NOTFUNCTION;}
"url("{w}{string}{w}")" {return Parser::Token_URI;}
"url("{w}{url}{w}")"    {return Parser::Token_URI;}
{ident}"("              {return Parser::Token_FUNCTION;}

U\+{range}              {return Parser::Token_UNICODERANGE;}
U\+{h}{1,6}-{h}{1,6}    {return Parser::Token_UNICODERANGE;}

<INITIAL>"{"            {BEGIN(block); return Parser::Token_LBRACE;}
<at_rule>"{"            {BEGIN(block); return Parser::Token_LBRACE;}
<at_rule>";"            {BEGIN(block); return Parser::Token_SEMICOLON;}
<block>"}"              {BEGIN(block); return Parser::Token_RBRACE;}
<mediaquery>"{"         {BEGIN(block); return Parser::Token_LBRACE;}
<mediaquery>";"         {BEGIN(block); return Parser::Token_SEMICOLON;}

,                       {BEGIN(block); return Parser::Token_COMMA;}
\(                      {BEGIN(block); return Parser::Token_LPAREN;}
\)                      {BEGIN(block); return Parser::Token_RPAREN;}
\{                      {BEGIN(block); return Parser::Token_LBRACE;}
\}                      {BEGIN(block); return Parser::Token_RBRACE;}
\[                      {BEGIN(block); return Parser::Token_LBRACKET;}
\]                      {BEGIN(block); return Parser::Token_RBRACKET;}
:                       {BEGIN(block); return Parser::Token_COLON;}
\*                      {BEGIN(block); return Parser::Token_STAR;}
;                       {BEGIN(block); return Parser::Token_SEMICOLON;}
\.                      {BEGIN(block); return Parser::Token_DOT;}
=                       {BEGIN(block); return Parser::Token_EQUALS;}
\+                      {BEGIN(block); return Parser::Token_PLUS;}
\-                      {BEGIN(block); return Parser::Token_MINUS;}
\/                       {BEGIN(block); return Parser::Token_DIVIDE;}

 /* add all tokens that match here above */
.                       {BEGIN(block); return Parser::Token_INVALID;}
%%


namespace Css
{

Tokenizer::Tokenizer( KDevPG::TokenStream *tokenStream, const QByteArray &contents )
{
    restart( tokenStream, contents );
}

Tokenizer::Tokenizer( KDevPG::TokenStream *tokenStream, const char* contents )
{
    restart( tokenStream, contents );
}

Tokenizer::Tokenizer( KDevPG::TokenStream *tokenStream, const QString &contents )
{
    restart( tokenStream, contents.toLatin1() );
}

void Tokenizer::restart( KDevPG::TokenStream *tokenStream, const QByteArray &contents )
{
    m_locationTable = tokenStream->locationTable();
    m_contents = contents;
    m_tokenBegin = 0;
    m_tokenEnd = -1;
    m_currentOffset = 0;

    yyrestart(NULL);
    BEGIN(INITIAL); // is not set automatically by yyrestart()
}

// reads a character, and returns 1 as the number of characters read
// (or 0 when the end of the string is reached)
int Tokenizer::LexerInput( char *buf, int /*max_size*/ )
{
    if (m_currentOffset >= m_contents.length()) return 0;

    char c = m_contents.at(m_currentOffset++);

    switch(c)
    {
    case '\r':
        c = '\n'; // only have one single line break character: '\n'
        if ( m_contents.at(m_currentOffset + 1) == '\n' )
        {
            m_currentOffset++;
            m_tokenEnd++;
        }

        // fall through
    case '\n':
        m_locationTable->newline( m_currentOffset - 1 );
        break;

    default:
        break;
    }

    buf[0] = c;
    return 1;
}

} // end of namespace Css

