/***************************************************************************
                          tagdialog.h  -  description
                             -------------------
    begin                : Sat Apr 1 2000
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

#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <qwidget.h>
#include <qtabdialog.h>
#include <qdict.h>
#include <qstrlist.h>

/**class for insert/edit tags
  *@author Yacovlev Alexander & Dmitry Poplavsky
  */

class Document;
class TagWidget;
class CoreWidgetDlg;
class EventsWidgetDlg;

extern QStrList *tagsList; // list of known tags
extern QStrList *tagsCore; // list of tags, allowed core attributes ( id, class, id, title )
extern QStrList *tagsI18n; // list of tags, allowed i18 attribs.
extern QStrList *tagsScript; // list of tags, allowed script attribs ( onClicl, omMouseOver... )
extern QStrList *quotedAttribs; // list of attribs, that have quoted values ( alt, src ... )


class TagDialog : public QTabDialog  {
   Q_OBJECT
public:
  /**
    constructor: edit tag if "tag" == 0L or insert new tag if tag != 0L
    attr format: "attr1='value1' attr2='value2'..."
    insertInLine :
        true : <a></a>
        false: <a>
               </a>

  */
	TagDialog( Document *write, QString tag = QString::null, QString attr = QString::null, bool insertInLine = true );
	~TagDialog();
	/**  */
  void parseTag();
  /** forme new tag */
  void formeTag();
  /** returen doc path */
	QString basePath();
	
public:
  bool  fEdit;

  CoreWidgetDlg 	*coreDlg;
  EventsWidgetDlg *eventsDlg;
  QWidget 				*mainDlg;

  Document *write;
  QDict<QString> *dict;
  QString tag;

public slots: // Public slots
  void slotAccept();

private:
  bool insertInLine;
  /** convert tag to upper or lower case */
  QString tagCase( QString tag);
  /** convert attribute of tag to upper or lower case */
  QString attrCase( QString  attr);

  void parseAttributes( QString attrs );
};

#endif
