/*****************************************************************************
 * Copyright (c) 2010 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
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

#ifndef HTMLCOMPLETIONSUPPORT_H
#define HTMLCOMPLETIONSUPPORT_H

#include "completionsupport.h"

namespace Xml {

/** Provides completion support for HTML in addition to DTD
 *  Completes images paths, etc.
 */
class HtmlCompletionSupport : public CompletionSupport
{
public:
    virtual QList< CompletionItem::Ptr > findEnumerations(KTextEditor::Document* document,
                                                          const KTextEditor::Range& range,
                                                          const QString& element = QString(),
                                                          const QString& attribute = QString()) const;
};
}

#endif // HTMLCOMPLETIONSUPPORT_H
