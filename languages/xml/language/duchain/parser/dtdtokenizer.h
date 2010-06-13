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

#ifndef DTDTOKENIZER_H
#define DTDTOKENIZER_H

#include "tokenizer.h"
#include <QtCore/QHash>

namespace Xml {


/** DTD language tokenizer.
 *  DTD defines ENTITIES which may include other documents or may contain text
 *  which has to be parsed. When a entity is declared it is global and the parser has to keep track of them
 *  for instance an entity <!ENTITY % optional.close "- O"> may be declared and reused in all included documents.
 *  The biggest problem is that there is no set rules as to what an entity may or may not contain. For instance 
 *  the example above may be declared as <!ENTITY % optional.close "O">; its beaning is determined by its usage.
 *  Another problem with entities is when the DUChain is built context's and declarations ranges vary greatly.
 */
class KDEVSGMLPARSER_EXPORT DTDTokenizer : public Tokenizer {
public:
    enum State {
        None, //Switch on first occurance of <
        DTD, //ended by >
        SGML,
        Quote,
        DOCTYPE,
        ENTITY,
        ELEMENT,
        ATTLIST,
        CONDITION,
        NOTATION
    };

    DTDTokenizer(TokenStream *tokenStream, const QString &contents, State initialState = None );
    DTDTokenizer(TokenStream *tokenStream, const QChar *content, qint64 contentLength, State initialState = None );

    virtual ~DTDTokenizer();

    virtual int nextTokenKind();

private:
    void init(State initialState = None );
    /** A structure pointing to sections in the content.
     *  This is used to try and resolve DTD ENTITIES.
     */
    struct TokenizePart;
    QHash<QString, struct TokenizePart> m_entities;
    QStack<struct TokenizePart> m_parts;
    QString m_tokenEndString;
    const QChar *m_entityContentStart;
    const QChar *m_entityContentEnd;
    QString m_entityName;
    bool m_externEntity;
};

}

#endif //DTDTOKENIZER_H

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
