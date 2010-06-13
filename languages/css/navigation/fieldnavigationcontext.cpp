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

#include "fieldnavigationcontext.h"

#include <KLocalizedString>

namespace Css {

FieldNavigationContext::FieldNavigationContext(KDevelop::TopDUContextPointer topContext, const ContentAssistData::Field& field)
    : AbstractNavigationContext(topContext), m_field(field)
{
}

QString FieldNavigationContext::name() const
{
    return m_field.name;
}

QString FieldNavigationContext::html(bool /*shorten*/)
{
    QString ret;
    ret += m_field.description;
    if (!m_field.example.isEmpty()) {
        ret += "<br>"+i18n("Example:");
        ret += "<code>"+m_field.example+"</code>";
    }
    if (!m_field.remarks.isEmpty()) {
        ret += "<br>"+m_field.remarks;
    }
    return ret;
}

}
