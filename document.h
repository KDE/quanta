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

#include "parser/qtag.h"

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
  */



class KConfig;
class QStringList;
class KTempFile;
class Tag;
class Node;

class Document : public QWidget{
   Q_OBJECT

public: 
	Document(const QString& basePath, KTextEditor::Document *doc, QWidget *parent = 0, const char *name = 0, WFlags f=0);
	~Document();

public:

  KURL url();

  bool isUntitled();
  void setUntitledUrl(QString);
  /** Return a node Tag according to line,col (or current cursor pos if p_line==p_col==-1), and
      according to dtd. If forwardOnly is true, the text is parsed from (p_line,p_col) forward.*/
  Tag *tagAt( DTDStruct *dtd, int p_line = -1, int p_col = -1, bool forwardOnly=false);
  /** return a pointet to the Node according to p_line, p_col (or current cursor pos, if both are -1)  */
  Node *nodeAt(int p_line = -1, int p_col = -1);
  /** Returns tag name at specified position */
  QString getTagNameAt( int line, int col );

  void selectText(int x1, int y1, int x2, int y2 );
  void replaceSelected(QString s);

  /** insert tag in document  */
  void insertTag( QString s1,QString s2 = "" );
  /** Change the current tag's attributes with those from dict */
  void changeCurrentTag( QDict<QString> *dict );
  /** add attrib to end of current tag */
  void insertAttrib(QString attr);
  /** No descriptions */
  void insertFile(QString fileName);
  /** Inserts text at the current cursor position */
  void insertText(QString, bool=true);

  void readConfig (KConfig *);
  void writeConfig(KConfig *);


  QPoint getGlobalCursorPos();
  QString find(QRegExp& rx, int sLine, int sCol, int& fbLine, int&fbCol, int &feLine, int&feCol);
  QString findRev(QRegExp& rx, int sLine, int sCol, int& fbLine, int&fbCol, int &feLine, int&feCol);
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

  /** Returns the DTD identifier for the document */
  QString getDTDIdentifier();
  /** Sets the DTD identifier */
  void setDTDIdentifier(QString id);
  /** Get a pointer to the current active DTD. If fallback is true, this always gives back a valid and known DTD pointer: the active, the document specified and in last case the application default document type. */
  DTDStruct* currentDTD(bool fallback = true);
  /** Find the DTD name for a part of the document. Search all the document if startLine=endLine=-1.*/
  QString findDTDName(int startLine, int endLine, bool checkCursorPos = true);
  /** Retriwes the text from the specified rectangle. The KTextEditor::EditInterface::text seems to not
work correctly. */
  QString text(int bLine, int bCol, int eLine, int eCol);
  /** Code completion was requested by the user. */
  void codeCompletionRequested();
  /** Code completion is manually invoked for script type languages. */
  void scriptCodeCompletion(DTDStruct *dtd, int line, int col);
  /** Bring up the code completion tooltip. */
  void codeCompletionHintRequested();
  /** Splits the document content in a QStringList (m_text). Must be called before find/findRev function is used in order to have an updated content. */

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
  /** No descriptions */
  void slotCompletionAborted();

private:

  QString untitledUrl;

  QString         spellText;
	QValueList<int> *spellPos;
  KTextEditor::Document *_doc;
  KTextEditor::View *_view;

	KTempFile *tempFile;
  KURL tempUrl;

	int spellMoved;
  QString dtdName;

  Tag *findScriptText(int line, int col);
  Tag *findScriptStruct(int line, int col);

  Tag *findXMLTag(int line, int col, bool forwardOnly = false);
  Tag *findText(int line, int col, bool forwardOnly = false);

  /** Brings up list of code completions */
  void showCodeCompletions( QValueList<KTextEditor::CompletionEntry> *completions );
  /** Get list of possibile tag name completions */
  QValueList<KTextEditor::CompletionEntry>* getTagCompletions(DTDStruct *dtd, int line, int col);
  /** Get list of possibile tag attribute completions */
  QValueList<KTextEditor::CompletionEntry>* getAttributeCompletions( DTDStruct *dtd, QString tagName, QString startsWith=QString::null);
  /** Get list of possibile tag attribute value completions */
  QValueList<KTextEditor::CompletionEntry>* getAttributeValueCompletions( DTDStruct *dtd, QString tagName, QString attribute, QString startsWith=QString::null);
  /** Get list of possibile completions in normal text input (nt creating a tag) */
  QValueList<KTextEditor::CompletionEntry>* getCharacterCompletions();
  /** Called whenever a user inputs text in an XML type document. */
  void xmlAutoCompletion(DTDStruct*,int , int , const QString & );
  /** Called whenever a user inputs text in a script type document. */
  void scriptAutoCompletion(DTDStruct*,int , int , const QString & );
  /** Find the word until the first word boundary backwards */
  QString findWordRev(const QString& textToSearch);
  /** Invoke code completion dialog for XML like tags according to the position (line, col), using DTD dtd. */
  void xmlCodeCompletion(DTDStruct *dtd, int line, int col);
protected: // Protected attributes
  /**  */
  bool completionInProgress;
};

#endif
