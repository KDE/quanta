/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden   *
 *   jens@kdewebdev.org   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TEMPLATESTREE_WIDGET_H
#define TEMPLATESTREE_WIDGET_H

#include <qwidget.h>
#include <QString>

namespace Koncrete { class Project; }
class TemplatesTreePart;

class TemplatesTreeWidget: public QWidget
{
    Q_OBJECT
public:  
    explicit TemplatesTreeWidget(TemplatesTreePart *part);
    ~TemplatesTreeWidget();

private:
    TemplatesTreePart *m_part;
};


#endif
