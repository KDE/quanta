/***************************************************************************
                          corewidgetdlg.cpp  -  description
                             -------------------
    begin                : Thu Mar 30 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
    email                : pdima@mail.univ.kiev.ua
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "corewidgetdlg.h"
#include "corewidgetdlg.moc"
#include "tagwidget.h"

#include <qdict.h>
#include <qlineedit.h>

CoreWidgetDlg::CoreWidgetDlg(QWidget *parent, const char *name)
  :CoreWidgetDlgS(parent,name), TagWidget(parent,name)
{
}

CoreWidgetDlg::~CoreWidgetDlg(){
}

void CoreWidgetDlg::readAttributes( QDict<QString> *d )
{
  dict = d;

  updateDict("id", lineId );
  updateDict("class", lineClass );
  updateDict("style", lineStyle );
  updateDict("title", lineTitle );

  updateDict("lang", lineLang );
  updateDict("dir", lineDir );

}

void CoreWidgetDlg::writeAttributes( QDict<QString> *d )
{
  dict = d;
  QString *t; // value of attr.

  if (( t=d->find("id") ))    setValue(*t, lineId);
  if (( t=d->find("class") )) setValue(*t, lineClass);
  if (( t=d->find("style") )) setValue(*t, lineStyle);
  if (( t=d->find("title") )) setValue(*t, lineTitle);

  if (( t=d->find("lang") ))  setValue(*t, lineLang);
  if (( t=d->find("dir") ))   setValue(*t, lineDir);
}




/**  */
void CoreWidgetDlg::disableCoreAttribs()
{
  lineId->setEnabled(false);
  lineClass->setEnabled(false);
  lineStyle->setEnabled(false);
  lineTitle->setEnabled(false);
}
/**  */
void CoreWidgetDlg::disableI18nAttribs()
{
  lineLang->setEnabled(false);
  lineDir->setEnabled(false);
}
