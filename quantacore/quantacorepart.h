/***************************************************************************
    begin                : Thu Jun 16 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef QUANTACOREPART_H
#define QUANTACOREPART_H

#include "quantacoreif.h"
#include "quantadoc.h"
#include "helper.h"

#include <qhash.h>

#include <kdevplugin.h>

class Parser;
class ParseResult;

class KUrl;
class ConfigWidgetProxy;
class KDialogBase;
class FileContext;
class QMenu;
class Context;
class KDevDocument;

/**
@author Andras Mantia
*/
class QuantaCorePart : public QuantaCoreIf
{
Q_OBJECT
public:
  QuantaCorePart(QObject *parent, const char *name, const QStringList &);

  ~QuantaCorePart();
  /**
   * Inserts a new tag into the current document.
   * @param tagPair pair of full opening and closing strings
   * @param inLine if true, the tag and the closing tag will be in the same line. Otherwise they will be in different lines.
   * @param showDialog if true, the tag dialog for the tagName is shown
   */
  virtual void insertTag(const TagPair& tagPair, bool inLine, bool showDialog);

  /**
   * Find the nick name of a DTEP associated with a name.
   * @param name the name (id) of the DTEP
   * @return the nickname of the DTEP or an empty string if no such DTEP was loaded
   */
  virtual QString getDTEPNickName(const QString &name) const;

  inline EditorSource * activeEditorSource() const {return m_activeQuantaDoc;};

  /**
   * proxy for QuantaDoc to emit this signal 
   * @param qp qp.x() = line and qp.y() = column
   */
  void emitNewCursorPosition(const QPoint & qp) {emit newCursorPosition(qp);};
  
public slots:
  
  /** insert <img> tag for images or <a> for other
   *  
   * @param url the url to insert, will be converted to relative
   * @param dirInfo optional information for pre and post text
   */
  void slotInsertTag(const KUrl& url, Helper::DirInfo * dirInfo = 0);

private slots:

  void init();
  
  void contextMenu(QMenu *popup, const Context *context);
  
  /**
    * Called when a file was loaded into the application.
    * @param url The url pointing to the file
    */
  void slotFileLoaded(KDevDocument* document);

  void slotStartParsing(const EditorSource *source);
  
  void slotFinishedParsing(const EditorSource *source, const ParseResult *parseResult);

  void slotGroupsParsed(const EditorSource *source, const ParseResult *parseResult);

  void slotShowCompletion() const {if (m_activeQuantaDoc) m_activeQuantaDoc->codeCompletionRequested();};

  void slotShowCompletionHint() const {if (m_activeQuantaDoc)    m_activeQuantaDoc->codeCompletionHintRequested();};
  
  void slotMakeDonation();
  void slotHelpHomepage();
  void slotHelpUserList();
  void slotOpenNew();
  
  void slotInsertConfigWidget(const KDialogBase *dlg, QWidget *page, unsigned int pageNo);
  
  /**
   * emits @see finishedParsing when the new part is known
   * @param newPart 
   */
  void slotActivePartChanged(KParts::Part * newPart);
  
  /**
   * removes the pointer to this @see QuantaDoc from @see m_documents
   * @param url 
   */
  void slotClosedFile(KDevDocument* document);

  /**
   * adjusts the url in @see m_documents if a part changed it
   * @param part 
   */
  void slotPartURLChanged(KDevDocument* document, const KUrl &oldUrl, const KUrl &newUrl);
  
  /**
   * insert a tag from a file context
   */
  void slotInsertTag();

  void slotChangeDTEP();

private:
  
  void initActions();

  QHash<QString, QuantaDoc*> m_documents; ///< holds the QuantaDoc objects assigned to each opened document;
  ConfigWidgetProxy *m_configProxy;
    
  /** the active QuantaDoc object or null if the active part is not a QuantaDoc object.
   */
  QuantaDoc * m_activeQuantaDoc;
  
  KAction * m_insertTagAction;
  KUrl::List m_fileContextList;
};

#endif