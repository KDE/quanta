/***************************************************************************
                          document.h  -  description
                             -------------------
    begin                : Tue Jun 6 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
                           (C) 2001-2003 Andras Mantia <amantia@freemail.hu>
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
#include <qevent.h>

#include <kate/document.h>
#include <kate/view.h>


#include <ktexteditor/document.h>
#include <ktexteditor/view.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/codecompletioninterface.h>

#include "undoredo.h"
#include "parser/qtag.h"

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
  */

class KConfig;
class QStringList;
class KTempFile;
class Tag;
class Node;
class Project;
class QuantaPluginInterface;

class Document : public QWidget{
   Q_OBJECT

public:
  Document(const KURL& p_baseURL, KTextEditor::Document *doc, Project *project,
           QuantaPluginInterface * =0,
           QWidget *parent = 0, const char *name = 0, WFlags f=0);
  ~Document();

  void resizeEvent(QResizeEvent* e);

  KURL url();

  bool isUntitled();
  void setUntitledUrl(QString);
  /** Returns tag name at specified position */
  QString getTagNameAt(int line, int col );

  void selectText(int x1, int y1, int x2, int y2 );
  void replaceSelected(const QString &s);

  /** insert tag in document  */
  void insertTag( QString s1,QString s2 = "" );
  /** Change the current tag's attributes with those from dict */
  void changeTag(Tag *tag, QDict<QString> *dict );
  /**Change the attr value of the called attrName to attrValue*/
  void changeTagAttribute(Tag *tag, const QString& attrName, const QString&attrValue);
  /** No descriptions */
  void insertFile(const KURL& url);
  /** Inserts text at the current cursor position */
  void insertText(const QString &, bool=true);

  void readConfig (KConfig *);
  void writeConfig(KConfig *);


  QPoint getGlobalCursorPos();
  QString find(const QRegExp& rx, int sLine, int sCol, int& fbLine, int&fbCol, int &feLine, int&feCol);
  QString findRev(const QRegExp& rx, int sLine, int sCol, int& fbLine, int&fbCol, int &feLine, int&feCol);
  /** Get the view of the document */
  KTextEditor::View* view();
  /** Get the KTextEditor::Document of the document */
  KTextEditor::Document* doc();
  /** Sets the modifiedFlag value. */
  void setModified(bool flag);
  /** Returns true if the document was modified. */
  bool isModified();
  /** No descriptions */
  int checkOverwrite(const KURL& u);

  /** Creates a temporary file where the url is backed up. */
  int createTempFile();
  /** No descriptions */
  int closeTempFile();
  /** No descriptions */
  void clearTempFile();
  /** No descriptions */
  QString tempFileName();
  /** No descriptions */
  bool saveIt();

  /** Returns the DTD identifier for the document */
  QString getDTDIdentifier();
  /** Sets the DTD identifier */
  void setDTDIdentifier(QString id);
  /** Get a pointer to the current active DTD. If fallback is true, this always gives back a valid and known DTD pointer: the active, the document specified and in last case the application default document type. */
  DTDStruct* currentDTD(bool fallback = true);
  /** Get a pointer to the default DTD (document, or app). */
  DTDStruct* defaultDTD();
  /** Find the DTD name for a part of the document. */
  QString findDTDName(Tag **tag);
  /** Retriwes the text from the specified rectangle. The KTextEditor::EditInterface::text seems to not
work correctly. */
  QString text(int bLine, int bCol, int eLine, int eCol);
  /** Code completion was requested by the user. */
  void codeCompletionRequested();
  /** Bring up the code completion tooltip. */
  void codeCompletionHintRequested();
  /** No descriptions */
  bool dirty() const {return m_dirty;};
  void setDirtyStatus(bool status) {m_dirty = status;};
  /** Ask for user confirmation if the file was changed outside. */
  void checkDirtyStatus();
  /** Save the document and reset the dirty status. */
  void save();
  /** No descriptions */
  QString parsingDTD();
  /** No descriptions */
  void setParsingDTD(const QString& dtdName);
  /** Find the word until the first word boundary backwards */
  QString findWordRev(const QString& textToSearch, DTDStruct *dtd = 0L);
  /** Returns the changed status since the last query. Resets changed.*/
  bool hasChanged();
  /** Sets the changed status.*/
  void setChanged(bool newStatus);
  /** Paste the contents of clipboard into the document */
  void paste();

  /** disable/enable the parser*/
  void activateParser(bool activation) {reparseEnabled = activation;}
  bool parserActivated() {return reparseEnabled;}

  /** returns all the areas that are between tag and it's closing pair */
  QStringList tagAreas(const QString &tag);

  bool busy;
  KURL baseURL;

  KTextEditor::ViewCursorInterface *viewCursorIf;
  KTextEditor::SelectionInterface *selectionIf;
  KTextEditor::EditInterface *editIf;
  KTextEditor::CodeCompletionInterface *codeCompletionIf;
  KTextEditor::ConfigInterface* configIf;

  Kate::Document *kate_doc;
  /** Hold the list of user tags (real or not, like functions) that are in the document*/
  QTagList userTagList;
  Kate::View *kate_view;
  /** The undo/redo stack */
  undoRedo docUndoRedo;

public slots:

  /* Called after a completion is inserted */
  void slotCompletionDone( KTextEditor::CompletionEntry completion );
  /* Called when a user selects a completion, we then can modify it */
  void slotFilterCompletion(KTextEditor::CompletionEntry*,QString *);
  /* Called whenever a user inputs text */
  void slotCharactersInserted(int ,int ,const QString&);
  /** No descriptions */
  void slotCompletionAborted();
  /** No descriptions */
  void slotTextChanged();
  /** No descriptions */
  void slotDelayedTextChanged();
  void slotDelayedScriptAutoCompletion();
  void slotDelayedShowCodeCompletion();

private:

  QuantaPluginInterface *m_pluginInterface;

  QString untitledUrl;

  QString         spellText;
  QValueList<int> *spellPos;
  KTextEditor::Document *m_doc;
  KTextEditor::View *m_view;

  KTempFile *tempFile;
  QString m_tempFileName;

  int spellMoved;
  QString dtdName;
  /*The DTD valid in the place where the completion was invoked.*/
  DTDStruct *completionDTD;

  bool changed;
  bool completionInProgress;
  bool reparseEnabled;
  /** True if the document is dirty (has been modified outside). */
  bool m_dirty;
  Project *m_project;
  /** Parse the document according to this DTD. */
  QString m_parsingDTD;
  //stores the data after an autocompletion. Used when bringing up the
  //autocompletion box delayed with the singleshot timer (workaround for
  //a bug: the box is not showing up if it is called from slotCompletionDone)
  int m_lastLine, m_lastCol;
  QValueList<KTextEditor::CompletionEntry>* m_lastCompletionList;

  /** Get list of possibile variable name completions */
  QValueList<KTextEditor::CompletionEntry>* getGroupCompletions(Node *node, const StructTreeGroup& groupName, int line, int col);
  /** Get list of possibile tag name completions */
  QValueList<KTextEditor::CompletionEntry>* getTagCompletions(int line, int col);
  /** Get list of possibile tag attribute completions */
  QValueList<KTextEditor::CompletionEntry>* getAttributeCompletions(const QString& tagName,const QString& startsWith=QString::null);
  /** Get list of possibile tag attribute value completions */
  QValueList<KTextEditor::CompletionEntry>* getAttributeValueCompletions(const QString& tagName, const QString& attribute, const QString& startsWith=QString::null);
  /** Get list of possibile completions in normal text input (nt creating a tag) */
  QValueList<KTextEditor::CompletionEntry>* getCharacterCompletions();
  /** Invoke code completion dialog for XML like tags according to the position (line, col), using DTD dtd. */
  bool xmlCodeCompletion(int line, int col);
  /** Returns list of values for attribute */
  QStringList* tagAttributeValues(const QString& dtdName, const QString& tag, const QString& attribute);
  /** Brings up list of code completions */
  void showCodeCompletions( QValueList<KTextEditor::CompletionEntry> *completions );
  /** Called whenever a user inputs text in an XML type document. */
  bool xmlAutoCompletion(int , int , const QString & );
  /** Called whenever a user inputs text in a script type document. */
  bool scriptAutoCompletion(int line, int col);
  /** Returns true if the number of " (excluding \") inside text is even. */
  bool evenQuotes(const QString &text);

};

#endif

