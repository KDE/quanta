/***************************************************************************
                          document.h  -  description
                             -------------------
    begin                : Tue Jun 6 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <qdict.h>
#include <qwidget.h>
#include "kwrite/kwview.h"

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

struct TagAttr {
  QString text;
  QString value;
  int textlen;
  int valuelen;
  int x;
  int endx;
  int y;
  bool quotes;
};

class KSpell;
class KConfig;
class QStringList;

class Document : public KWrite  {
   Q_OBJECT
   friend class KWrite;
   friend class KWriteDoc;
   friend class KWriteView;
   
public: 
	Document( KWriteDoc *doc, const QString& basePath, QWidget *parent=0, const char *name=0);
	~Document();

public:

  // configure dialog
  void editorOptions();

  KURL url();
  void setURL(KURL,bool);
  
  bool isUntitled();
  void setUntitledUrl(QString);
  
  /** return qstring with current tag for parse */
  QString currentTag();
  
  void selectText(int x1, int y1, int x2, int y2 );
  
  void replaceSelected(QString s);

  /** insert tag in document  */
  void insertTag( QString s1,QString s2 = "" );

  /** add attrib to end of current tag */
  void insertAttrib(QString attr);

	int pos2y (int pos);
	int pos2x (int pos);
	int xy2pos(int x, int y );

public slots:
  
  void slotSpellCheck  ();
  void slotSpellGo     (KSpell *);
  void slotSpellResult (bool);
  void slotSpellCorrect( const QString &originalword, const QString &newword, unsigned int pos);
  void slotSpellMis    ( const QString &originalword, const QStringList &suggestions, unsigned int pos);
  void slotSpellDone   ();
  void createSpellList ();

public:

  void readConfig (KConfig *);
  void writeConfig(KConfig *);
  
  bool  isVerticalSelect();
  void setVerticalSelect(bool);

  QString findBeginOfTag( QString tag, int x, int y);
  QString findEndOfTag  ( QString tag, int x, int y);
  
  void parseTag    ();
  void parseTagAttr( QString t,int &x,int &y);
  
  QString getLine        (int y);
  QString getTagAttr     (int i);
  QString getTagAttrValue(int i);
  
  void changeCurrentTag( QDict<QString> *dict );
  
  QPoint getGlobalCursorPos();

  /** convert tag to upper or lower case */
  QString tagCase( QString  tag);
  
  /** convert attribute of tag to upper or lower case */
  QString attrCase( QString  attr);
  /** No descriptions */
  void insertFile(QString fileName);

  TagAttr tagAttr[50];
  int tagAttrNum;
  int tagBeginX, tagBeginY, tagEndX, tagEndY;

  bool oldstat;
  bool busy;
  QString basePath;

private:

  QString untitledUrl;


  KSpell          *spell;
  QString         spellText;
	QValueList<int> *spellPos;
	
	int spellMoved;
};

#endif
