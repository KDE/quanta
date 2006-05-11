/***************************************************************************
                          tagdialog.h  -  description
                             -------------------
    begin                : Sat Apr 1 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002-2003 by Andras Mantia <amantia@kde.org>
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
#include <q3tabdialog.h>
#include <QHash>

#include <kurl.h>

/**class for insert/edit tags
  *@author Yacovlev Alexander & Dmitry Poplavsky & Andras Mantia
  */

class TagWidget;
class Tagxml;
class QTag;
class Tag;
class QShowEvent;
class TagPair;

class TagDialog : public Q3TabDialog  {
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
  TagDialog(QTag* dtdTag, Tag* tag, const KUrl& a_baseURL = KUrl());
  TagDialog(QTag* dtdTag, Tag* tag, const QString& selection, const KUrl& a_baseURL = KUrl());
  TagDialog(QTag* dtdTag, const QString& attrs = QString::null, const KUrl& a_baseURL = KUrl());
  TagDialog(QTag* dtdTag, const QString& selection, const QString& attrs=QString::null, const KUrl& a_baseURL = KUrl());
  ~TagDialog();
  /** Insert an attribute to dict*/
  void insertAttribute(QString *attr, QString *value);
  /**  */
  void parseTag();
  /** returen doc path */
  KUrl baseURL();


  /** Return the attributes in QDict<QString> format*/
  QHash<QString, QString*> * getAttributes();
  /** Return all the attributes in one string*/
  QString getAttributeString();
  /**Return the value of the attribute specified by attr. */
  QString getAttribute(const QString& attr);
  /** Get the new tag*/
  TagPair getNewlyCreatedTag();
  /** build and return the new Node.*/
//FIXME   Node *buildNode(Document *w);

public:

  QWidget *mainDlg;

  QHash<QString, QString*> *dict;
  KUrl m_baseURL;

public slots: // Public slots
  void slotAccept();

protected slots:
  virtual void showEvent(QShowEvent *ev);

private:
  bool deleteTag;
  Tag *m_tag;
  QTag* dtdTag;
  QString m_selection;

  QList<Tagxml*> *extraPageList;

  void init(QTag* dtdTag, const KUrl& a_baseURL);
  void parseAttributes(const QString &attrs);
};

#endif
