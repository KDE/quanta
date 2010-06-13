/*
   Copyright (C) 2010 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef CSS_FIELDNAVIGATIONCONTEXT_H
#define CSS_FIELDNAVIGATIONCONTEXT_H

#include <language/duchain/navigation/abstractnavigationcontext.h>
#include "../completion/contentassistdata.h"

namespace Css {

class FieldNavigationContext : public KDevelop::AbstractNavigationContext
{
public:
    FieldNavigationContext(KDevelop::TopDUContextPointer topContext,
                                   const ContentAssistData::Field &field);

    virtual QString name() const;
    virtual QString html(bool shorten = false);
private:
    ContentAssistData::Field m_field;
};

}

#endif
