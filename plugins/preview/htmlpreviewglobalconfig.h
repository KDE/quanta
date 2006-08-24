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

#ifndef HTMLPREVIEW_GLOBAL_CONFIG_H
#define HTMLPREVIEW_GLOBAL_CONFIG_H

#include "ui_htmlpreviewglobalconfigbase.h"

class HTMLPreviewPart;

class HTMLPreviewGlobalConfig: public QWidget, public Ui::HTMLPreviewGlobalConfigBase
{
    Q_OBJECT
public:
    HTMLPreviewGlobalConfig(HTMLPreviewPart *part, QWidget *parent = 0);

public slots:
    void accept();

private:
    HTMLPreviewPart *m_part;
};

#endif
