/***************************************************************************
                          document.h  -  description
                             -------------------
    begin                : Tue Jun 6 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2001-2004 Andras Mantia <amantia@kde.org>
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

#include "qtag.h"

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
  */

class QEvent;
class QFocusEvent;
class KConfig;
class QStringList;
class KTempFile;
class KURL;
class Tag;
class Node;
class Project;
class undoRedo;
struct AreaStruct;
struct DTDStruct;

namespace KTextEditor
{
  class CodeCompletionInterface;
  class CompletionEntry;
  class ConfigInterface;
  class Document;
  class EditInterface;
  class EditInterfaceExt;
  class MarkInterface;
  class SelectionInterface;
  class View;
  class ViewCursorInterface;
}

class Document : public QWidget{
   Q_OBJECT

public:
  Document(KTextEditor::Document *doc,
           QWidget *parent = 0, const char *name = 0, WFlags f=0);
  ~Document();

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
  /**Change the namespace in a tag. Add if it's not present, or remove if the
  namespace argument is empty*/
  void changeTagNamespace(Tag *tag, const QString& nameSpace);
  /** No descriptions */
  void insertFile(const KURL& url);
  /** Inserts text at the current cursor position */
  void insertText(const QString &text, bool adjustCursor = true, bool reparse = true);
  /** Recursively insert the mandatory childs of tag. Returns true if a child was
  inserted.*/
  bool insertChildTags(QTag *tag, QTag* lastTag = 0L);

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

  /** Creates a temporary file where the url is backed up.
  *  If dump is true the editor content is saved to the temporary file,
  *  otherwise a copy of the original file is created.
  */
  int createTempFile(bool dump = false);
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
  const DTDStruct* currentDTD(bool fallback = true);
  /** Get a pointer to the default DTD (document, or app). */
  const DTDStruct* defaultDTD();
  /** Find the DTD name for a part of the document. */
  QString findDTDName(Tag **tag);
  /** Retrives the text from the specified rectangle. The KTextEditor::EditInterface::text seems to not
work correctly. */
  QString text(int bLine, int bCol, int eLine, int eCol) const;
  /** Same as the above, but using AreaStruct as an argument */
  QString text(const AreaStruct &area) const;
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
  QString findWordRev(const QString& textToSearch, const DTDStruct *dtd = 0L);
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
  QStringList tagAreas(const QString &tag, bool includeCoordinates, bool skipFoundContent);

  /** disable/enable the repaint of the Kate view */
  void activateRepaintView(bool activation);
  bool RepaintViewActivated() {return repaintEnabled;}

  void setErrorMark(int line);
  void clearErrorMarks();
  void convertCase();

  /** returns the word under the cursor */
  QString currentWord();

  bool busy;

  KTextEditor::ViewCursorInterface *viewCursorIf;
  KTextEditor::SelectionInterface *selectionIf;
  KTextEditor::EditInterface *editIf;
#ifdef BUILD_KAFKAPART
  KTextEditor::EditInterfaceExt *editIfExt;
#endif
  KTextEditor::CodeCompletionInterface *codeCompletionIf;
  KTextEditor::ConfigInterface* configIf;
  KTextEditor::MarkInterface* markIf;

  /** Hold the list of user tags (real or not, like functions) that are in the document*/
  QTagList userTagList;
#ifdef BUILD_KAFKAPART
  /** The undo/redo stack */
  undoRedo *docUndoRedo;
#endif

  bool isBackedUp();
  /** Creates an automatic backup copy for the crash recovering mechanism */
  void createBackup(KConfig* config);
  /** No descriptions */
  void setBackupEntry(bool b);
  /** No descriptions */
  QString backupPathEntryValue();
  /** No descriptions */
  void setBackupPathEntryValue(const QString& ev);
  /** Removes automatic backup copies */
  void removeBackup(KConfig *config);
  /** create a string using document path string */
 static QString hashFilePath(const QString& p);

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
  void slotDelayedTextChanged(bool forced = false);
  void slotDelayedScriptAutoCompletion();
  void slotDelayedShowCodeCompletion();

signals:
 /** Emitted when the internal text editor got the focus */
  void editorGotFocus();

private slots:
  void slotReplaceChar();

private:

  QString untitledUrl;
  int m_replaceLine;
  int m_replaceCol;
  QString m_replaceStr;

  QString         spellText;
  QValueList<int> *spellPos;
  KTextEditor::Document *m_doc;
  KTextEditor::View *m_view;

  KTempFile *tempFile;
  QString m_tempFileName;
  bool m_backupEntry;
  /* path of the backup copy file of the document */
  QString m_backupPathValue;
  int spellMoved;
  QString dtdName;
/*The DTD valid in the place where the completion was invoked.*/
  const DTDStruct *completionDTD;

  bool changed;
  bool completionInProgress;
  bool reparseEnabled;
  bool repaintEnabled;
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
  QValueList<KTextEditor::CompletionEntry>* getCharacterCompletions(const QString& starstWith=QString::null);
  /** Invoke code completion dialog for XML like tags according to the position (line, col), using DTD dtd. */
  bool xmlCodeCompletion(int line, int col);
  /** Returns list of values for attribute. If deleteResult is true after the call,
  the caller must delete the returned list. */
  QStringList* tagAttributeValues(const QString& dtdName, const QString& tag, const QString& attribute, bool &deleteResult);
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
