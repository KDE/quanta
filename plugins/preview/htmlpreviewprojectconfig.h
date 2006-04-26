/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden   *
 *   jens@kdewebdev.org   *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef HTMLPREVIEW_PROJECT_CONFIG_H
#define HTMLPREVIEW_PROJECT_CONFIG_H

#include "htmlpreviewprojectconfigbase.h"

class HTMLPreviewPart;

class HTMLPreviewProjectConfig: public HTMLPreviewProjectConfigBase
{
    Q_OBJECT
public:
    HTMLPreviewProjectConfig(HTMLPreviewPart *part, QWidget *parent = 0, const char *name = 0);

public slots:
    void accept();

private:
    HTMLPreviewPart *m_part;
};

#endif
