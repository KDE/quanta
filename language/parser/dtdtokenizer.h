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

#ifndef DTDTOKENIZER_H
#define DTDTOKENIZER_H

#include "tokenizer.h"
#include <QtCore/QHash>

namespace Xml {


/** DTD language tokenizer.
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
     *  This is used to resolve DTD ENTITIES.
     */
    struct TokenizePart;
    QHash<QString, struct TokenizePart> m_entities;
    QStack<struct TokenizePart> m_parts;
    QString m_tokenEndString;
    const IDtdHelper *m_dtdHelper;
    const QChar *m_entityContentStart;
    const QChar *m_entityContentEnd;
    QString m_entityName;
    bool m_externEntity;
};

}

#endif //DTDTOKENIZER_H

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
