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

//qt includes
#include <qdatetime.h>
#include <qdict.h>
#include <qmap.h>
#include <qwidget.h>

#include <kurl.h>
#include <ktexteditor/markinterfaceextension.h>

//own includes
#include "qtag.h"

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon & Andras Mantia
  */

class QDomDocument;
class QEvent;
class QFocusEvent;
class QTextCodec;
class QStringList;
class KConfig;
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
  class EncodingInterface;
  class MarkInterface;
  class SelectionInterface;
  class SelectionInterfaceExt;
  class View;
  class ViewCursorInterface;
  class Mark;
}

class Document : public QWidget{
   Q_OBJECT

public:
  Document(KTextEditor::Document *doc,
           QWidget *parent = 0, const char *name = 0, WFlags f=0);
  ~Document();

  KURL url();

  bool isUntitled();
  void setUntitledUrl(const QString &url);
  /** Returns tag name at specified position */
  QString getTagNameAt(int line, int col );

  void selectText(int x1, int y1, int x2, int y2 );
  void replaceSelected(const QString &s);

  /** insert tag in document  */
  void insertTag(const QString &s1, const QString &s2 = QString::null);
  /** Change the current tag's attributes with those from dict */
  void changeTag(Tag *tag, QDict<QString> *dict );
  /**Change the attr value of the called attrName to attrValue*/
  void changeTagAttribute(Tag *tag, const QString& attrName, const QString&attrValue);
  /**Change the namespace in a tag. Add if it's not present, or remove if the
  namespace argument is empty*/
  void changeTagNamespace(Tag *tag, const QString& nameSpace);
  /** Insert the content of the url into the document. */
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

  /** Creates a temporary file where the editor content is saved.
  */
  void createTempFile();
  /** Closes and removes the temporary file. */
  void closeTempFile();
  /** Returns the name of the temporary file, QString::null if no temporary file exists. */
  QString tempFileName();

  /** Returns the DTD identifier for the document */
  QString getDTDIdentifier();
  /** Sets the DTD identifier */
  void setDTDIdentifier(const QString &id);
  /** Get a pointer to the current active DTD. If fallback is true, this always gives back a valid and known DTD pointer: the active, the document specified and in last case the application default document type. */
  const DTDStruct* currentDTD(bool fallback = true);
  /** Get a pointer to the default DTD (document, or app). */
  const DTDStruct* defaultDTD() const;
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
  /** Returns the dirty status. */
  bool dirty() const {return m_dirty;};
  void setDirtyStatus(bool status) {m_dirty = status;};
  /** Ask for user confirmation if the file was changed outside. */
  void checkDirtyStatus();
  /** Save the document and reset the dirty status. */
  void save();
  /** Save the document under a new name and calculate the new md5sum. */
  bool saveAs(const KURL& url);
  /** Enable or disable the visibility of groups for a DTEP.*/
  void enableGroupsForDTEP(const QString& dtepName, bool enable = true);
  /** Clears the selected DTEP list */
  void resetGroupsForDTEPList();
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
  /** Opens the url. The url must be valid and the file pointed to it must exists. */
  void open(const KURL &url, const QString &encoding);
  /**
   * Opens a file in the editor part.
   * @param url
   */
  bool openURL(const KURL& url);
  /** Reads the DTD info from the file, tries to find the correct DTD and builds the tag/attribute list from the DTD file. */
  void processDTD(const QString& documentType = QString::null);

  /** Resets the list of DTEPs found in the document */
  void resetDTEPs();
  /** Adds a DTEP to the list of DTEPs present in the document */
  void addDTEP(const QString &dtepName);
  /** Returns the list of DTEPs that should appear in the structure tree. By default
      this is the list of DTEPs present in the document, but the user can turn on/
      off them with the help of RMB->Show Groups For in the structure tree */
  QStringList groupsForDTEPs();

  bool busy;

  KTextEditor::ViewCursorInterface *viewCursorIf;
  KTextEditor::SelectionInterface *selectionIf;
  KTextEditor::SelectionInterfaceExt *selectionIfExt;
  KTextEditor::EditInterface *editIf;
  KTextEditor::EncodingInterface *encodingIf;
  KTextEditor::EditInterfaceExt *editIfExt;
  KTextEditor::CodeCompletionInterface *codeCompletionIf;
  KTextEditor::ConfigInterface* configIf;
  KTextEditor::MarkInterface* markIf;

  /** Hold the list of user tags (real or not, like functions) that are in the document*/
  QTagList userTagList;
  /** The undo/redo stack */
  undoRedo *docUndoRedo;

  bool isBackedUp();
  /** Creates an automatic backup copy for the crash recovering mechanism */
  void createBackup(KConfig* config);
  /** No descriptions */
  QString backupPathEntryValue();
  /** No descriptions */
  void setBackupPathEntryValue(const QString& ev);
  /** Removes automatic backup copies */
  void removeBackup(KConfig *config);
  /** create a string using document path string */
  static QString hashFilePath(const QString& p);
  QString annotationText(uint line);
  void setAnnotationText(uint line, const QString& text);
  QMap<uint, QPair<QString, QString> > annotations() {return m_annotations;}
  void addAnnotation(uint line, const QPair<QString, QString>& annotation);
  void clearAnnotations();

public slots:

  /** Called after a completion is inserted */
  void slotCompletionDone( KTextEditor::CompletionEntry completion );
  /** Called when a user selects a completion, we then can modify it */
  void slotFilterCompletion(KTextEditor::CompletionEntry*,QString *);
  /** Called whenever a user inputs text */
  void slotCharactersInserted(int ,int ,const QString&);
  /** Called when the code completion is aborted.*/
  void slotCompletionAborted();
  /** Called whenever the text in the document is changed. */
  void slotTextChanged();
  /** Handle the text changed events. Usually called from slotTextChanged,
  but it's possible to force the handling by calling manually and setting
  forced to true. */
  void slotDelayedTextChanged(bool forced = false);
  void slotDelayedScriptAutoCompletion();
  void slotDelayedShowCodeCompletion();

signals:
 /** Emitted when the internal text editor got the focus */
  void editorGotFocus();
  void openingFailed(const KURL &url);
  void openingCompleted(const KURL &url);

  void breakpointMarked(Document*, int);
  void breakpointUnmarked(Document*, int);
  void showAnnotation(uint, const QString&, const QPair<QString, QString>&);

private slots:
  void slotReplaceChar();
  void slotOpeningCompleted();
  void slotOpeningFailed(const QString &errorMessage);
  /** Called when a file on the disk has changed. */
  void slotFileDirty(const QString& fileName);

  void slotMarkChanged(KTextEditor::Mark mark, KTextEditor::MarkInterfaceExtension::MarkChangeAction action);
private:
  /**
   * Finds the beginning of a tag in the document, starting from a position.
   * @param position start to look from this position backwards
   * @return the position of the starting character or an empty QPoint if not found
   */
  QPoint findTagBeginning(const QPoint& position);
  QPoint findTagEnd(const QPoint& position);



  QMap<uint, QPair<QString, QString> > m_annotations;
  QString untitledUrl;
  int m_replaceLine;
  int m_replaceCol;
  QString m_replaceStr;

  KTextEditor::Document *m_doc;
  KTextEditor::View *m_view;

  KTempFile *tempFile;
  QString m_tempFileName;
  QDateTime m_modifTime;
  /* path of the backup copy file of the document */
  QString m_backupPathValue;
  QString dtdName;
  QString m_encoding;
  QTextCodec *m_codec;
/*The DTD valid in the place where the completion was invoked.*/
  const DTDStruct *completionDTD;

  bool changed;
  bool completionInProgress;
  bool completionRequested; ///< true if the code completion was explicitely requested by the user
  bool argHintVisible;
  bool hintRequested;
  bool reparseEnabled;
  bool repaintEnabled;
  bool delayedTextChangedEnabled;
  /** True if the document is dirty (has been modified outside). */
  bool m_dirty;
  QString m_md5sum;
  Project *m_project;
  /** Parse the document according to this DTD. */
  QStringList m_groupsForDTEPs; ///< The list of the DTEPs for which the groups should appear in the structure tree
  QStringList m_DTEPList; ///< The list of all DTEPs found in the document
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
  bool scriptAutoCompletion(int line, int col, const QString &insertedString);
  /** Returns true if the number of " (excluding \") inside text is even. */
  bool evenQuotes(const QString &text);
  void handleCodeCompletion();
  bool isDerivatedFrom(const QString& className, const QString &baseClass);
};

#endif

