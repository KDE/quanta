/*****************************************************************************
 * Copyright (c) 2009 Ruan Strydom <rm3dom@gmail.com>                        *
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

#ifndef FORMATTERPREFERENCES_H
#define FORMATTERPREFERENCES_H

#include "ui_formatterwidget.h"

#include <interfaces/isourceformatter.h>

namespace Xml
{
    class Formatter;


class FormatterPreferences : public KDevelop::SettingsWidget, public Ui::FormatWidget {
    Q_OBJECT
public:
    FormatterPreferences ( QWidget* parent = 0, Qt::WindowFlags f = 0 );
    virtual ~FormatterPreferences();
    virtual QMap<QString, QVariant> getOptions();
    virtual QString save();
    virtual void load(const KDevelop::SourceFormatterStyle& );
protected slots:
    void indentChanged (int i);
    void caseChanged (int i);
    void formatSourceChanged (int i);
    void updateText ();
protected:
    Xml::Formatter *m_formatter;
};

}

#endif // FORMATTERPREFERENCES_H
