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
   friend KWrite;
   friend KWriteDoc;
   friend KWriteView;
public: 
	Document( KWriteDoc *doc, QWidget *parent=0, const char *name=0, QString fname = "");
	~Document();

public:

  // configure dialog
  void editorOptions();

  bool hasFileName();
  QString fileName();

  /** return qstring with current tag for parse */
  QString currentTag();
  /**  */
  void selectText(int x1, int y1, int x2, int y2 );
  /**  */
  void replaceSelected(QString s);

  /** Insert tag in cursor position and set cursor between s1 and s2 */
//  void insert_Tag( char * s1,  char * s2 = 0L );
  /** insert tag in document  */
  void insertTag( QString s1,QString s2 = "" );

  /** add attrib to end of current tag */
  void insertAttrib(QString attr);

	int xy2pos( int x, int y );
	int pos2y(int pos);
	int pos2x(int pos);




public slots:

  // Spell checked related slots

  void slotSpellCheck();
  void slotSpellGo(KSpell *);
  void slotSpellResult(bool);
  void slotSpellCorrect( QString originalword, QString newword, unsigned pos);
  void slotSpellMis( QString originalword, QStringList *suggestions, unsigned pos);
  void slotSpellDone();
  void createSpellList();

public:

  void readConfig(KConfig *);
  void writeConfig(KConfig *);
  //bool hasFileName() { return (fileName() != 0 ); };

  QString findBeginOfTag( QString tag, int x, int y);
  QString findEndOfTag( QString tag, int x, int y);
  void parseTag();
  void parseTagAttr( QString t,int &x,int &y);
  //const char *parseTagAttrValue(const char *t,int &x,int &y);
  QString getLine(int y);
  QString getTagAttr(int i);
  QString getTagAttrValue(int i);
  void changeCurrentTag( QDict<QString> *dict );
  QPoint getGlobalCursorPos();

  /** convert tag to upper or lower case */
  QString tagCase( QString  tag);
  /** convert attribute of tag to upper or lower case */
  QString attrCase( QString  attr);

  TagAttr tagAttr[50];
  int tagAttrNum;
  int tagBeginX, tagBeginY, tagEndX, tagEndY;

  QString filename;

  KSpell *spell;
 	/** spell lists */
	QValueList<int> *spellPos;
//	QValueList<int> *spellLen;
//	QStringList     *spellList;
	QString  spellText;
	int spellMoved;

};

#endif
