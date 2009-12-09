/*
   Copyright (C) 2009 Niko Sams <niko.sams@gmail.com>

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

#ifndef CSS_CODECOMPLETIONMODEL_H
#define CSS_CODECOMPLETIONMODEL_H

#include <KDE/KTextEditor/CodeCompletionModel>

namespace Css {

class ContentAssistData;

class CodeCompletionModel : public KTextEditor::CodeCompletionModel2
{
public:
    CodeCompletionModel(QObject *parent);
    virtual void completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, InvocationType invocationType);
    virtual QVariant data(const QModelIndex & index, int role) const;

private:
    QList< QString > m_currentFields;
    ContentAssistData *m_assistData;
};

}

#endif // CSS_CODECOMPLETIONMODEL_H
