/***************************************************************************
 *   Copyright (C) 2003 by Gulmini Luciano                                          *
 *   gulmini.luciano@student.unife.it                                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/
#ifndef CSSSHPROPERTYPARSER_H
#define CSSSHPROPERTYPARSER_H

/**
@author Gulmini Luciano
*/
#include <qstringlist.h>

class CSSSHPropertyParser{

  public:
    CSSSHPropertyParser(const QString& s);
    ~CSSSHPropertyParser();
    QStringList parse();

  private:
    QString m_propertyToParse;

  private:
    QString extractURIList();
    QString extractFunctionList();
    QString extractQuotedStringList();
    QString removeBeginningWhiteSpaces(const QString& s);
    QString removeEndingWhiteSpaces(const QString& s);
};

#endif
