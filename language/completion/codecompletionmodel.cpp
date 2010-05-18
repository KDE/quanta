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

#include "codecompletionmodel.h"

using namespace Xml;

CodeCompletionModel::CodeCompletionModel(QObject* parent): CodeCompletionModel2(parent)
{

}

CodeCompletionModel::~CodeCompletionModel()
{

}

void CodeCompletionModel::completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, KTextEditor::CodeCompletionModel::InvocationType invocationType)
{
    KTextEditor::CodeCompletionModel::completionInvoked(view, range, invocationType);
}

KTextEditor::Range CodeCompletionModel::completionRange(KTextEditor::View* view, const KTextEditor::Cursor& position)
{
    return KTextEditor::CodeCompletionModelControllerInterface::completionRange(view, position);
}

QVariant CodeCompletionModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void CodeCompletionModel::executeCompletionItem2(KTextEditor::Document* document, const KTextEditor::Range& word, const QModelIndex& index) const
{
    KTextEditor::CodeCompletionModel2::executeCompletionItem2(document, word, index);
}

bool CodeCompletionModel::shouldAbortCompletion(KTextEditor::View* view, const KTextEditor::SmartRange& range, const QString& currentCompletion)
{
    return KTextEditor::CodeCompletionModelControllerInterface::shouldAbortCompletion(view, range, currentCompletion);
}


#include "codecompletionmodel.moc"
