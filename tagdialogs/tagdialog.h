/***************************************************************************
                          tagdialog.h  -  description
                             -------------------
    begin                : Sat Apr 1 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002 by Andras Mantia
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
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
  */

class Document;
class TagWidget;
class CoreWidgetDlg;
class EventsWidgetDlg;

extern QDict<QString> *tagsList; // list of known tags
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
	TagDialog(QString tag = QString::null, QString attr = QString::null);
	~TagDialog();
  /** Insert an attribute to dict*/
  void insertAttribute(QString *attr, QString *value);
	/**  */
  void parseTag();
  /** returen doc path */
	QString getBasePath();
  /** set document path */
  void setBasePath(Document *w);

  /** Return the attributes in QDict<QString> format*/
  QDict<QString> * getAttributes();
  /** Return all the attributes in one string*/
  QString getAttributeString();
  /**Return the value of the attribute specified by attr. */
  QString getAttribute(QString attr);
  /** Insert the new tag into the Document*/
  void insertTag(Document *w, bool insertInLine = true);

public:

  CoreWidgetDlg 	*coreDlg;
  EventsWidgetDlg *eventsDlg;
  QWidget 				*mainDlg;

  QDict<QString> *dict;
  QString tag;
  QString basePath;

public slots: // Public slots
  void slotAccept();

private:
  void parseAttributes( QString attrs );
};

#endif
