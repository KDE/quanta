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

#include <kate/document.h>
#include <kate/view.h>


#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/codecompletioninterface.h>


/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
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

class KConfig;
class QStringList;
class KTempFile;

class Document : public QWidget{
   Q_OBJECT

public: 
	Document(const QString& basePath, KTextEditor::Document *doc, QWidget *parent = 0, const char *name = 0, WFlags f=0);
	~Document();

public:

  KURL url();

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

public:

  void readConfig (KConfig *);
  void writeConfig(KConfig *);
  
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
  void insertText(QString, bool=true);
  /** Get the view of the document */
  KTextEditor::View* view();
  /** Get the KTextEditor::Document of the document */
  KTextEditor::Document* doc();
  /** Sets the modifiedFlag value. */
  void setModified(bool flag);
  /** Returns true if the document was modified. */
  bool isModified();
  /** No descriptions */
  int checkOverwrite(KURL u);
  /** Creates a temporary file where the url is backed up. */
  int createTempFile();
  /** No descriptions */
  int closeTempFile();
  /** No descriptions */
  void clearTempFile();
  /** No descriptions */
  KURL tempURL();
  /** No descriptions */
  bool saveIt();

  /** Returns tag name at specified position */
  QString getTagNameAt( int line, int col );

  /** Brings up list of code completions */
  void showCodeCompletions( QValueList<KTextEditor::CompletionEntry> *completions );

  /** Get list of possibile tag name completions */
  QValueList<KTextEditor::CompletionEntry>* getTagCompletions();
  
  /** Get list of possibile tag attribute completions */
  QValueList<KTextEditor::CompletionEntry>* getAttributeCompletions( QString tag );
  
  /** Get list of possibile tag attribute value completions */
  QValueList<KTextEditor::CompletionEntry>* getAttributeValueCompletions( QString tag, QString attribute );
  
  /** Get list of possibile completions in normal text input (nt creating a tag) */
  QValueList<KTextEditor::CompletionEntry>* getCharacterCompletions();

  TagAttr tagAttr[50];
  int tagAttrNum;
  int tagBeginX, tagBeginY, tagEndX, tagEndY;

  bool oldstat;
  bool busy;
  QString basePath;

  KTextEditor::ViewCursorInterface *viewCursorIf;
  KTextEditor::SelectionInterface *selectionIf;
  KTextEditor::EditInterface *editIf;
  KTextEditor::CodeCompletionInterface *codeCompletionIf;

  Kate::Document *kate_doc;
  Kate::View *kate_view;
  
public slots:

  /* Called after a completion is inserted */
  void slotCompletionDone( KTextEditor::CompletionEntry completion );
  
  /* Called when a user selects a completion, we then can modify it */
  void slotFilterCompletion(KTextEditor::CompletionEntry*,QString *);
  
  /* Called whenever a user inputs text */
  void slotCharactersInserted(int ,int ,const QString&);

private:

  QString untitledUrl;

  QString         spellText;
	QValueList<int> *spellPos;
  KTextEditor::Document *_doc;
  KTextEditor::View *_view;

	KTempFile *tempFile;
  KURL tempUrl;

	int spellMoved;
};

#endif
