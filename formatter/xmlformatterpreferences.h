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

#ifndef XMLFORMATTERPROPERTIES_H
#define XMLFORMATTERPROPERTIES_H

#include "ui_xmlformatterwidget.h"

#include <interfaces/isourceformatter.h>

class XmlFormatter;

class XmlFormatterPreferences : public KDevelop::SettingsWidget, public Ui::XmlFormatWidget {
    Q_OBJECT
public:
    XmlFormatterPreferences ( QWidget* parent = 0, Qt::WindowFlags f = 0 );
    virtual ~XmlFormatterPreferences();
    virtual QMap<QString, QVariant> getOptions();
    virtual QString save();
    virtual void load(const KDevelop::SourceFormatterStyle& );
protected slots:
    void updateText ( int );
protected:
    XmlFormatter *m_formatter;
};

#endif // XMLFORMATTERPROPERTIES_H
