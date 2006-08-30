/***************************************************************************
    begin                : Aug 24 2006
    copyright            : (C) 2006 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef QUANTAHANDLER_H
#define QUANTAHANDLER_H

#include <QVector>

#include <ktexteditor/range.h>



/**
 * \short The interface for Quanta specific reporting.
 * 
 * This is an additional handler for Quanta which allows us to get 
 * the positions of the elements in the source code.
 * 
 * \author Jens Herden   \<jens@kdewebdev.org\>
 */

class QuantaHandler
{
  public:
    typedef QVector<KTextEditor::Range> Ranges;
    
    QuantaHandler() {};
    virtual ~QuantaHandler() {};
    
    /**
     * Report the position of the element in the source.
     * 
     * \param elementRange The Range of the whole element including '<' and '>'
     * \param attrRanges The Ranges of the attributes from the first character of
     *                   the attribute name to the last character of the attribute value.
     *                   This can be empty if there are not attributes.
     * 
     * \return \e false if the reader should stops parsing and report an error. 
     */
    virtual bool elementRanges(const KTextEditor::Range & elementRange, const Ranges & attrRanges) = 0;
    
    /**
     * The reader uses the function errorString() to get the error message.
     * 
     * \return the error string
     */
    virtual QString errorString () const = 0;
};

#endif

//kate: space-indent on; indent-width 2; replace-tabs on; mixedindent off; encoding utf-8
