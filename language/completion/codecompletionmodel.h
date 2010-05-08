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

#ifndef CODECOMPLETIONMODEL_H
#define CODECOMPLETIONMODEL_H

#include "completionexport.h"

#include <KTextEditor/CodeCompletionModel>
#include <KTextEditor/CodeCompletionModelControllerInterface>
#include <language/codecompletion/codecompletionmodel.h>
#include <language/codecompletion/codecompletionworker.h>

namespace Xml {

class KDEVXMLCOMPLETION_EXPORT CodeCompletionModel  : public KTextEditor::CodeCompletionModel2, public KTextEditor::CodeCompletionModelControllerInterface
{
    Q_OBJECT
    Q_INTERFACES(KTextEditor::CodeCompletionModelControllerInterface)
public:
    CodeCompletionModel(QObject *parent);
    virtual ~CodeCompletionModel();
    
    virtual void completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, InvocationType invocationType);

    virtual QVariant data(const QModelIndex & index, int role) const;

    virtual KTextEditor::Range completionRange(KTextEditor::View* view, const KTextEditor::Cursor &position);
    
    virtual bool shouldAbortCompletion(KTextEditor::View* view, const KTextEditor::SmartRange& range, const QString &currentCompletion);

    virtual void executeCompletionItem2(KTextEditor::Document* document, const KTextEditor::Range& word, const QModelIndex& index) const;
};

}

#endif // CODECOMPLETIONMODEL_H
