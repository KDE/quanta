/*****************************************************************************
 * Copyright 2005, 2006 Jakob Petsovits <jpetso@gmx.at>                      *
 * Copyright 2010 Niko Sams <niko.sams@gmail.com>                            *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU Library General Public               *
 * License as published by the Free Software Foundation; either              *
 * version 2 of the License, or (at your option) any later version.          *
 *                                                                           *
 * This grammar is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Lesser General Public License for more details.                           *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public License *
 * along with this library; see the file COPYING.LIB.  If not, write to      *
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,      *
 * Boston, MA 02110-1301, USA.                                               *
 *****************************************************************************/
#ifndef CSS_TOKENIZER_H
#define CSS_TOKENIZER_H

#include "parserexport.h"

#include <QByteArray>
#include <QString>

#ifndef DONT_INCLUDE_FLEXLEXER
#include "FlexLexer.h"
#endif

// The YY_USER_ACTION macro is called whenever a token is found by Flex
#define YY_USER_ACTION \
m_tokenBegin = m_tokenEnd + 1; \
m_tokenEnd += yyleng;

namespace KDevPG {
    class LocationTable;
    class TokenStream;
}
namespace Css
{

class KDEVCSSPARSER_EXPORT Tokenizer : public yyFlexLexer
{
public:
    explicit Tokenizer( KDevPG::TokenStream *tokenStream, const QByteArray &contents );
    explicit Tokenizer( KDevPG::TokenStream *tokenStream, const char* contents );
    explicit Tokenizer( KDevPG::TokenStream *tokenStream, const QString &contents );
    void restart( KDevPG::TokenStream *tokenStream, const QByteArray &contents );

    int yylex();
    int nextTokenKind() { return yylex(); }
    std::size_t tokenBegin() const { return m_tokenBegin; }
    std::size_t tokenEnd()   const { return m_tokenEnd;   }

protected:
    // custom input, replacing the Flex default input stdin
    virtual int LexerInput( char *buf, int max_size );

    // dismisses any lexer output (which should not happen anyways)
    virtual void LexerOutput( const char * /*buf*/, int /*max_size*/ ) { return; }
    virtual void LexerError( const char */*msg*/ ) { return; }

private:
    QByteArray m_contents;
    std::size_t m_tokenBegin, m_tokenEnd;
    std::size_t m_currentOffset;
    KDevPG::LocationTable *m_locationTable;
};

} // end of namespace Css

#endif

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on

