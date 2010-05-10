/*****************************************************************************
 * Copyright (c) 2010 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#ifndef SGMLTOKENIZER_H
#define SGMLTOKENIZER_H

#include "tokenizer.h"

namespace Xml {


/** Markup language tokenizer.
 *  Tokenizes SGML/XML like languages. Because of SGML's dependency on DTD, SGML must
 *  specify a doctype, when a doctype is found, a facade IDtdHelper is used to help tokenize SGML.
 *  Until a - or if no doctype is found the source is treated as plain XML.<br/>
 *  Here is an example of the complexity of SGML:<br/>
 *  <pre>
 *  <![CDATA[
 *  <!doctype root>
 *  <root>
 *      <child1> I am a child where do I belong, where do I end? I also have mixed content.
 *          <child1.1>I am a <child> with cdata content.</child1.1>
 *      <child2>
 *  ]]>
 *  <pre><br/>
 *  It is clear that there is absolutely no way to determine where an element starts and end without DTD.
 *  It is also not possible to parse 'child1.1' without knowing that its content is cdata.<br/>
 *  This will correct CDATA and empty elements, the parser has to correct optional close elements.
 *  The parser may use the helper found by this lexer.
 *  @see IDtdHelper
 *  @todo Implement a DTD parser
 */
class KDEVSGMLPARSER_EXPORT SgmlTokenizer : public Tokenizer {
public:
    enum State {
        None, //Switch on first occurance of <
        Tag, //all tags opening and closing, etc
        Quote, //used by DTD not sgml
        DTD, //ended by >
        Processing, //ended by ?>
        PHP, //ended by ?>
        Source, //ended by %>
        Cdata //untill m_tokenEndString
    };

    SgmlTokenizer(TokenStream *tokenStream, const QString& contents, State initialState = None );

    virtual ~SgmlTokenizer();

    int nextTokenKind();
  
private:
    QString m_tokenEndString;
    Tokenizer *m_dtdTokenizer;
};

}

#endif //SGMLTOKENIZER_H

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
