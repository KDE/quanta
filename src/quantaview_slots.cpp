/**************************************************************************
                          quantaview_slots.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky <pdima@mail.univ.kiev.ua>
                           (C) 2002, 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// standard header
#include <time.h>

// include files for QT
#include <qdir.h>
#include <qregexp.h>
#include <qcstring.h>
#include <qlineedit.h>
#include <qclipboard.h>
#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qtextedit.h>
#include <qtextcodec.h>
#include <qspinbox.h>
#include <qradiobutton.h>

// include files for KDE
#include <kaction.h>
#include <kconfig.h>
#include <kdebug.h>
#include <klocale.h>
#include <kprocess.h>
#include <kcolordialog.h>
#include <kmessagebox.h>
#include <kdeversion.h>
#include <kstatusbar.h>

#include <ktexteditor/configinterface.h>
#include <ktexteditor/clipboardinterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/markinterface.h>
#include <ktexteditor/dynwordwrapinterface.h>

// application specific includes
#include "quantacommon.h"
#include "document.h"
#include "quanta.h"
#include "quantadoc.h"
#include "quantaview.h"
#include "resource.h"
#include "qextfileinfo.h"

#include "tag.h"
#include "project.h"
#include "messageoutput.h"

// dialogs headers
#include "tagdialog.h"

#include "tagquicklistdlg.h"
#include "tagmaildlg.h"
#include "tagmisc.h"
#include "tableeditor.h"

#include "csseditor.h"
#include "cssselector.h"

#include "spellchecker.h"
#include "framewizard.h"

#include "phpdebuggerinterface.h"

#include "qdom.h"

void QuantaView::slotEditCurrentTag()
{
  if (!writeExists())
    return;
  Document *w = write();
  if (parser->parsingNeeded())
    baseNode = parser->rebuild(w);
  uint line,col;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  Node *node = parser->nodeAt(line, col, false);
  bool isUnknown = true;
  QString tagName;
  if (node && node->tag)
  {
    Tag *tag = node->tag;
    tagName = tag->name;
    if ( QuantaCommon::isKnownTag(tag->dtd->name,tagName) )
    {
      isUnknown = false;
      TagDialog *dlg = new TagDialog( QuantaCommon::tagFromDTD(tag->dtd,tagName), tag, baseURL() );
      if (dlg->exec())
      {
        w->changeTag(tag, dlg->getAttributes() );
      }

      delete dlg;
    }
  }
  if (isUnknown)
  {
    QString message = i18n("Unknown tag: %1").arg(tagName);
    quantaApp->slotStatusMsg( message );
  }
}

void QuantaView::slotSelectTagArea(Node *node)
{
  int bLine, bCol, eLine, eCol;
  Tag *tag = node->tag;
  tag->beginPos(bLine, bCol);
  if (tag->single || !node->next)
  {
    tag->endPos(eLine, eCol);
  } else
  if (tag->closingMissing && node->child)
  {
    Node *childNode = node->child;
    while (childNode->child || childNode->next)
    {
      if (childNode->next)
      {
        childNode = childNode->next;
      } else
      {
        childNode = childNode->child;
      }
    }
    childNode->tag->endPos(eLine, eCol);
  } else
  {
    node->next->tag->endPos(eLine, eCol);
  }
  quantaApp->selectArea(bLine, bCol, eLine, eCol + 1);
}

void QuantaView::slotSelectTagArea()
{
  if (!writeExists())
      return;
  uint line,col;
  write()->viewCursorIf->cursorPositionReal(&line, &col); 
  Node *node = parser->nodeAt(line, col);
  slotSelectTagArea(node);
}

void QuantaView::slotFrameWizard()
{
  if (!writeExists())
      return;
  Document *w = write();
  QStringList list = w->tagAreas("frameset", true, true);
  bool framesetExists = !list.isEmpty();
  int bl, bc, el, ec;
  bl = bc = el = ec = 0;
  QStringList l;
  QStringList l2;
  QuantaCommon::normalizeStructure(list[0],l2);
  if (framesetExists)
  {
    l = QStringList::split('\n',list[0],true);
    QStringList coordList = QStringList::split(',', l[0], true);
    bl = coordList[0].toInt();
    bc = coordList[1].toInt();
    el = coordList[2].toInt();
    ec = coordList[3].toInt();
    l.remove(l.begin());
  }

  FrameWizard dlg(this);

  if (!w->isUntitled())
      {
       dlg.setSaved(true);
      }
  dlg.setFramesetFileCurrentPath(Project::ref()->projectBaseURL().path());
  dlg.loadExistingFramesetStructure(l2);

  if ( dlg.exec() )
  {
    QString tag =
QString("\n")+dlg.generateFramesetStructure()+QString("\n");
    if (framesetExists)
    {
      w->activateParser(false);
      w->editIf->removeText(bl, bc, el, ec + 1);
      w->viewCursorIf->setCursorPositionReal((uint)bl, (uint)bc);
      w->activateParser(true);
    }
    w->insertTag(tag);
  }
}


/** edit/insert CSS */
void QuantaView::slotInsertCSS()
{
  if (!writeExists()) return;
  Document *w = write();

  uint line, col;
  int bLine, bCol, eLine, eCol;
  bLine = bCol = eLine = eCol = 0;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  if (line == 0 && col == 0)
    col++;
  parser->rebuild(w);  
  Node *node = parser->nodeAt(line, col, false);
  unsigned int lastLine = w->editIf->numLines() - 1;
  unsigned int lastCol = w->editIf->lineLength(lastLine);
  Node *styleNode = node;

  if (styleNode->tag->type == Tag::XmlTagEnd && styleNode->prev)
    styleNode = styleNode->prev;

  while (styleNode && styleNode->parent && styleNode->tag->name.lower() != "style" && styleNode->tag->dtd->name == "text/css") 
    styleNode = styleNode->parent;

  Node *parentNode = node;
  if (parentNode->tag->type == Tag::XmlTagEnd && parentNode->prev)
    parentNode = parentNode->prev;
  else
    while (parentNode && parentNode->parent &&
           parentNode->tag->type != Tag::XmlTag)
      parentNode = parentNode->parent;
  QString fullDocument = w->editIf->text().stripWhiteSpace();
  
  if (styleNode->tag->name.lower() == "comment block" && styleNode->parent) {
    if (styleNode->parent->tag->name.lower() == "style") {
      styleNode = styleNode->parent;
    }
  }
  
  if (styleNode && styleNode->tag->name.lower() == "style" && styleNode->next)  //inside <style> invoke the selector editor
  {
    styleNode->tag->endPos(bLine, bCol);
    QString header(w->text(0, 0, bLine, bCol));// beginning part of the file
    styleNode->next->tag->endPos(eLine, eCol);
    QString footer("</style>" + w->text(eLine, eCol+1, lastLine, lastCol)); // ending part of the file

    styleNode->next->tag->beginPos(eLine, eCol);
    QString styleTagContent(w->text(bLine, bCol+1, eLine, eCol-1).remove("<!--").remove("-->"));// <style></style> block content
	kdDebug(24000) << "Style tag contains: " << endl << styleTagContent << endl;
    CSSSelector *dlg = new CSSSelector;

    dlg->setForInitialPreview(fullDocument);
    //dlg->setSourceFileName(w->url().path());

    dlg->setHeader(header);
    dlg->setFooter(footer);

    dlg->loadExistingStyleSection(styleTagContent);
    if( dlg->exec() ){
      w->activateParser(false);
      styleNode->next->tag->beginPos(eLine, eCol);
      w->editIf->removeText(bLine, bCol+1, eLine, eCol);
      w->viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol+1);
      w->activateParser(true);
      w->insertTag( "\n<!--" + dlg->generateStyleSection() + "-->\n");
    }
    delete dlg;

  } else if (!node || fullDocument.isEmpty() ||
      w->currentDTD(true)->name == "text/css") //empty document or pure CSS file, invoke the selector editor
  {
	kdDebug(24000) << "[CSS editor] This is a pure CSS document";

    CSSSelector *dlg = new CSSSelector;
    dlg->setForInitialPreview(QString::null);
    if (!fullDocument.isEmpty())
      dlg->loadExistingStyleSection(fullDocument);
    if (dlg->exec())
    {
      w->activateParser(false);
      w->editIf->clear();
      w->activateParser(true);
      w->insertTag(dlg->generateStyleSection());
    }
    delete dlg;
  } else
  if (parentNode && parentNode->tag->type == Tag::XmlTag)
  {
	kdDebug(24000) << "[CSS editor] We will add a style attribute to: " << parentNode->tag->name << endl;
    CSSEditor *dlg = new CSSEditor(this);
    dlg->setForInitialPreview(fullDocument);

    parentNode->tag->beginPos(bLine, bCol);
    parentNode->tag->endPos(eLine, eCol);
    dlg->setFooter(">" + w->text(eLine, eCol + 1, lastLine, lastCol));

    QString temp(QString::null);
    QString tempStyleContent(QString::null);
    if (parentNode->tag->hasAttribute("style"))
    {
      tempStyleContent = parentNode->tag->attributeValue("style");
      dlg->setInlineStyleContent(tempStyleContent);
      //parentNode->tag->deleteAttribute("style");
      temp = parentNode->tag->toString();
    } else {
      dlg->setInlineStyleContent(QString::null);
      temp = parentNode->tag->toString();
    }
    //using QString::mid sometimes generates strange results; maybe this is due to a (random) blank in temp
    temp = temp.left(temp.length()-1);//remove >
    temp = temp.right(temp.length()-1);//remove <
    dlg->setHeader(w->text(0, 0, bLine, bCol) + temp);

    dlg->initialize();
    if( dlg->exec() )
    {
      w->changeTagAttribute(parentNode->tag, "style", dlg->generateProperties());
    } else {
      //parentNode->tag->changeTagAttribute("style", tempStyleContent);
    }
    delete dlg;
   } else
   KMessageBox::sorry(this, i18n("The CSS Editor cannot be invoked here.\nTry to invoke it on a tag or on a style section."));
}

/** for <a href=mailto> tag  */
void QuantaView::slotTagMail()
{
  if (!writeExists()) return;

  TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Email Link (mailto)"));

  if ( mailDlg->exec() ) {
    QString tag = QString(QuantaCommon::tagCase("<a"));

    if ( !QString(mailDlg->lineEmail->text()).isEmpty())
    {
      tag += QuantaCommon::attrCase(" href=")+qConfig.attrValueQuotation+"mailto:"+mailDlg->lineEmail->text();
        if ( !QString(mailDlg->lineSubject->text()).isEmpty())
         tag += "?subject="+KURL::encode_string(mailDlg->lineSubject->text());
         tag += qConfig.attrValueQuotation;
    }

    if ( !QString(mailDlg->titleEdit->text()).isEmpty())
         tag += QuantaCommon::attrCase(" title=")+qConfig.attrValueQuotation+mailDlg->titleEdit->text()+qConfig.attrValueQuotation;
    tag += QString(">");
    write()->insertTag(tag,QuantaCommon::tagCase("</a>"));
  }
  delete mailDlg;
}

/** Add the starting and closing text for a
user specified tag. */
void QuantaView::slotTagMisc()
{
 if (!writeExists()) return;

 static QString element = "";
 static bool addClosingTag = true;

 KDialogBase miscDlg(this, 0L, true, i18n("Misc. Tag"), KDialogBase::Ok | KDialogBase::Cancel);
 TagMisc *miscWidget = new TagMisc(&miscDlg);
 miscDlg.setMainWidget(miscWidget);
 miscWidget->addClosingTag->setChecked(addClosingTag);
 miscWidget->elementName->setText(element);

  if ( miscDlg.exec() )
  {
    QString tag;
    element = miscWidget->elementName->text();
    element.remove('<');
    element.remove('>');
    if ( !element.isEmpty())
    {
      tag += "<" + QuantaCommon::attrCase(element)+">";
        if ( (addClosingTag = miscWidget->addClosingTag->isChecked()) == true)
        {
          write()->insertTag(tag,QuantaCommon::tagCase( "</"+QuantaCommon::attrCase(element)+">"));
        } else
        {
          write()->insertTag(tag,QuantaCommon::tagCase(""));
        }
    }
  }
}

/** do quick list */
void QuantaView::slotTagQuickList()
{
  if (!writeExists()) return;

  TagQuickListDlg *listDlg = new TagQuickListDlg(this,i18n("Generate List"));
  if ( listDlg->exec() ) {
    int i;
    int n = listDlg->spinBoxRows->value();

    QString tag;
    if ( listDlg->radioOrdered->isChecked())
      tag = QString("<ol>\n")+space;
    else tag = QString("<ul>\n")+space;

    for ( i=0;i<n;i++)
      if ( qConfig.closeTags )
        tag += QString("  <li>  </li>\n")+space;
      else
        tag += QString("  <li> \n")+space;

    if ( listDlg->radioOrdered->isChecked())
      tag += QString("</ol>");
    else tag += QString("</ul>");

    write()->insertTag( QuantaCommon::tagCase(tag));
  }
  delete(listDlg);
}

void QuantaView::slotTagEditTable()
{
  if (!writeExists()) return;
  Document *w = write();
  baseNode = parser->rebuild(w);
  QStringList list = w->tagAreas("table", true, false);
  bool tableExists = false;
  uint line, col;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  int bl, bc, el, ec;
  int bLine, bCol, eLine, eCol;
  bLine = bCol = eLine = eCol = 0;
  QStringList l;
  QStringList l2;
  for (QStringList::Iterator it = list.begin(); it != list.end(); ++it)
  {
    QuantaCommon::normalizeStructure(*it, l2);
    l = QStringList::split('\n', *it, true);
    QStringList coordList = QStringList::split(',', l[0], true);
    bl = coordList[0].toInt();
    bc = coordList[1].toInt();
    el = coordList[2].toInt();
    ec = coordList[3].toInt();
    if (QuantaCommon::isBetween(line, col, bl, bc, el, ec) == 0)
    {
      tableExists = true;
      bLine = bl;
      bCol = bc;
      eLine = el;
      eCol = ec;
    }
    l.remove(l.begin());
  }

  TableEditor editor;
  bool tableRead = true;
  if (tableExists)
  {
    editor.setBaseURL(baseURL());
    tableRead = editor.setTableArea(bLine, bCol, eLine, eCol, parser);
    if (!tableRead)
    {
      KMessageBox::error(this, i18n("The table structure is invalid. Most probably you forgot to close some tags."), i18n("Cannot Read Table"));
    }
  } else
  {
    Node *node = parser->nodeAt(line, col);
    const DTDStruct *dtd = w->defaultDTD();
    if (node)
      dtd = node->tag->dtd;
    bLine = line;
    bCol = col;
    eLine = line;
    eCol = col;
    editor.createNewTable(w, dtd);
  }
  if (tableRead && editor.exec())
  {
    QString tableString = editor.readModifiedTable();
    w->activateParser(false);
//#ifdef BUILD_KAFKAPART
//          if(w->editIfExt)
//            w->editIfExt->editBegin();
//#endif
    if (eLine != bLine || (eLine == bLine && eCol != bCol))
      w->editIf->removeText(bLine, bCol, eLine, eCol + 1);
    w->viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol);
    w->insertText(tableString, false);
//#ifdef BUILD_KAFKAPART
//          if(w->editIfExt)
//            w->editIfExt->editEnd();
//#endif
    w->viewCursorIf->setCursorPositionReal(line, col);
    quantaApp->reparse(true);
  }
}

/** Open color Dialog and insert color in the text */
void QuantaView::slotTagColor()
{
  if (!writeExists()) return;
  QColor color;

  if (KColorDialog::getColor( color )) {
    char c[8];
    sprintf(c,"#%2X%2X%2X",color.red(),color.green(),color.blue());
    for (int i=0;i<7;i++) if (c[i] == ' ') c[i] = '0';
    QString scolor = (char *)c;
    write()->insertTag(scolor);
  }
}

/** insert date */
void QuantaView::slotTagDate()
{
  if (!writeExists()) return;
  time_t tektime;
  time( &tektime);
  QString stime = ctime( &tektime);

  write()->insertTag( stime);
}

/** for select form */
void QuantaView::slotTagSelect()
{
  if (!writeExists()) return;
  write()->insertTag(QuantaCommon::tagCase("<select")+ QuantaCommon::attrCase("name")+QuantaCommon::tagCase("=\"\"><option>"),QuantaCommon::tagCase("</select>"));
}

void QuantaView::slotViewInKFM()
{
  if (!writeExists()) return;
  Document *w = write();
  if (w->isModified())
  {
    if ( KMessageBox::questionYesNo(this,
                                    i18n("The file must be saved before external preview.\n"
                                         "Do you want to save and preview?"),
                                    i18n("Save Before Preview"),
                                    i18n("&Yes"),i18n("&No"), "AskForSaveBeforePreview")
         == KMessageBox::Yes)
    {
      if (w->isUntitled())
      {
       quantaApp->slotFileSaveAs();
      }
      else
      {
       w->save();
      }
    } else
    {
      return;
    }
  }
  if ( !w->isUntitled() )
  {
    KProcess *show = new KProcess();
    KURL url = Project::ref()->urlWithPrefix(w->url());
    *show << "kfmclient" << "openURL" << url.url();
    show->start( KProcess::DontCare );
  }
}

void QuantaView::slotViewInLynx()
{
  if (!writeExists()) return;
  Document *w = write();
  if (w->isModified())
  {
    if ( KMessageBox::questionYesNo(this,
                                    i18n("The file must be saved before external preview.\n"
                                         "Do you want to save and preview?"),
                                    i18n("Save Before Preview"),i18n("&Yes"),i18n("&No"), "AskForSaveBeforePreview")
         == KMessageBox::Yes)
    {
      if (w->isUntitled())
      {
       quantaApp->slotFileSaveAs();
      }
      else
      {
       w->save();
      }
    } else
    {
      return;
    }
  }
  if ( !w->isUntitled() )
  {
    KProcess *show = new KProcess();
    KURL url = Project::ref()->urlWithPrefix(w->url());
    *show << "konsole"
          << "--nohist"
          << "--notoolbar"
          << "--caption"
          << "Lynx Preview - Quanta"
          << "-e"
          << "lynx"
          << url.url();
    show->start( KProcess::DontCare );
  }
}

/**  */
void QuantaView::slotNewCurPos()
{
   emit newCurPos();
}

/** get output */
void QuantaView::slotGetScriptOutput(KProcess *, char *buffer, int buflen)
{
  if (writeExists())
  {
    Document *w = write();

    QString output(QString::fromLocal8Bit(buffer));
    output.truncate(buflen);

    if ( scriptOutputDest == "cursor" )
      w->insertTag(output);

    if ( scriptOutputDest == "message" )
    {
      MessageOutput *msgoutput = quantaApp->messageOutput();
        if ( beginOfScriptOutput )
        {
          //if ( !quantaApp->viewMenu->isItemChecked(ID_VIEW_MES) )
          //  quantaApp->slotViewMes();
          msgoutput->clear();
          msgoutput->insertItem(i18n("Script output:\n"));
        }

        msgoutput->showMessage(output);
    }

    if ( scriptOutputDest == "new" )
    {
      if ( beginOfScriptOutput )
          doc->openDocument( KURL() );
      w->insertTag(output);
    }

    if ( scriptOutputDest == "replace" )
    {
      if ( beginOfScriptOutput ) w->editIf->clear();
      w->insertTag(output);
    }

    beginOfScriptOutput = false;
  }
}

/** get output */
void QuantaView::slotGetScriptError(KProcess *, char *buffer, int buflen)
{
  if (writeExists())
  {
    Document *w = write();

    QString output(QString::fromLocal8Bit(buffer));
    output.truncate(buflen);

    if ( scriptErrorDest == "merge" ) {
      scriptErrorDest = scriptOutputDest;
      beginOfScriptError = beginOfScriptOutput;
    }

    if ( scriptErrorDest == "cursor" )
      w->insertTag(output);

    if ( scriptErrorDest == "message" )
    {
      MessageOutput *msgoutput = quantaApp->messageOutput();
      if ( beginOfScriptError ) {
          //if ( !quantaApp->viewMenu->isItemChecked(ID_VIEW_MES) )
          //  quantaApp->slotViewMes();
          msgoutput->clear();
          msgoutput->insertItem(i18n("Script output:\n"));
        }

        msgoutput->showMessage( output );
    }

    if ( scriptErrorDest == "new" )
    {
      if ( beginOfScriptError )
          doc->openDocument( KURL() );
      w->insertTag(output);
    }

    if ( scriptErrorDest == "replace" )
    {
      if ( beginOfScriptError ) write()->editIf->clear();
      w->insertTag(output);
    }

    beginOfScriptError = false;
  }
}

/** insert clipboard contents (but quote them for HTML first) */
void QuantaView::slotPasteHTMLQuoted()
{
  if (writeExists())
  {
    Document *w = write();
    QClipboard *cb = qApp->clipboard();
    QString text = cb->text();

    if ( ( !text.isNull() ) && (!text.isEmpty() ) )
    {
      text.replace( "&", "&amp;" );
      text.replace( "<", "&lt;" );
      text.replace( "\"", "&quot;" );
      text.replace( ">", "&gt;" );
      unsigned int line, col;
      w->viewCursorIf->cursorPositionReal(&line, &col);
      w->editIf->insertText(line, col, text );
    }
  }
}

/** insert clipboard contents (but quote them as a URL first) */
void QuantaView::slotPasteURLEncoded()
{
  if (writeExists())
  {
    Document *w = write();
    QClipboard *cb = qApp->clipboard();
    QString text = cb->text();

    if ( ( !text.isNull() ) && (!text.isEmpty() ) )
    {
      text = KURL::encode_string( text );
      unsigned int line, col;
      w->viewCursorIf->cursorPositionReal(&line, &col);
      w->editIf->insertText(line, col, text );
    }
  }
}


/** Kate related slots. */

void QuantaView::slotUndo ()
{
#ifdef BUILD_KAFKAPART
  if(hadLastFocus() == QuantaView::kafkaFocus)
  {
    KMessageBox::information(this, i18n("Sorry, VPL does not support this functionality yet."),
      QString::null, "show undo unavailable");
    return;
  }
#endif
  if (writeExists())
  {
    bool updateClosing = qConfig.updateClosingTags;
    qConfig.updateClosingTags = false;
//#ifdef BUILD_KAFKAPART
//    write()->docUndoRedo.undo(false);
//#else
    dynamic_cast<KTextEditor::UndoInterface*>(write()->doc())->undo();
//#endif
    qConfig.updateClosingTags = updateClosing;
  }
}

void QuantaView::slotRedo ()
{
#ifdef BUILD_KAFKAPART
  if(hadLastFocus() == QuantaView::kafkaFocus)
  {
    KMessageBox::information(this, i18n("Sorry, VPL does not support this functionality yet."),
      QString::null, "show redo unavailable");
    return;
  }
#endif
  if (writeExists())
  {
    bool updateClosing = qConfig.updateClosingTags;
    qConfig.updateClosingTags = false;
//#ifdef BUILD_KAFKAPART
//    write()->docUndoRedo.redo(false);
//#else
    dynamic_cast<KTextEditor::UndoInterface*>(write()->doc())->redo();
//#endif
    qConfig.updateClosingTags = updateClosing;
  }
}

void QuantaView::slotCut ()
{
#ifdef BUILD_KAFKAPART
  if(hadLastFocus() == QuantaView::kafkaFocus)
  {
    KMessageBox::information(this, i18n("Sorry, VPL does not support this functionality yet."),
      QString::null, "show cut unavailable");
    return;
  }
#endif
  if (writeExists())
  {
    dynamic_cast<KTextEditor::ClipboardInterface*>(write()->view())->cut();
  }
}

void QuantaView::slotCopy ()
{
#ifdef BUILD_KAFKAPART
  if(hadLastFocus() == QuantaView::kafkaFocus)
  {
    KMessageBox::information(this, i18n("Sorry, VPL does not support this functionality yet."),
      QString::null, "show copy unavailable");
    return;
  }
#endif
  if (writeExists())
  {
    dynamic_cast<KTextEditor::ClipboardInterface*>(write()->view())->copy();
  }
 /* QClipboard *cb = QApplication::clipboard();
  QString text = cb->text(QClipboard::Selection);
  cb->setText(text, QClipboard::Clipboard);*/
}

void QuantaView::slotPaste ()
{
#ifdef BUILD_KAFKAPART
  if(hadLastFocus() == QuantaView::kafkaFocus)
  {
    KMessageBox::information(this, i18n("Sorry, VPL does not support this functionality yet."),
      QString::null, "show paste unavailable");
    return;
  }
#endif
  if (writeExists())
  {
    write()->paste();
  }
}

void QuantaView::slotDeselectAll ()
{
  if (writeExists())
  {
    write()->selectionIf->clearSelection ();
  }
}

void QuantaView::toggleVertical()
{
  if (writeExists())
  {
    dynamic_cast<KTextEditor::BlockSelectionInterface*>(write()->doc())->toggleBlockSelectionMode();
  }
}

void QuantaView::toggleInsert()
{
  if (writeExists())
  {
    Document *w = write();
    KToggleAction *a = dynamic_cast<KToggleAction*>(w->view()->actionCollection()->action("set_insert"));
    if (a)
    {
      a->activate();
      quantaApp->statusBar()->changeItem(a->isChecked() ? i18n(" OVR ") : i18n(" INS "),IDS_INS_OVR); }
    }
}

void QuantaView::slotIndent()
{
  if (writeExists())
  {
    KAction *a = write()->view()->actionCollection()->action("tools_indent");
    if (a)
      a->activate();
  }
}

void QuantaView::slotUnIndent()
{
  if (writeExists())
  {
    KAction *a = write()->view()->actionCollection()->action("tools_unindent");
    if (a)
      a->activate();
  }
}

void QuantaView::slotCleanIndent()
{
  if (writeExists())
  {
    KAction *a = write()->view()->actionCollection()->action("tools_cleanIndent");
    if (a)
      a->activate();
  }
}

void QuantaView::slotComment ()
{
  if (writeExists())
  {
    KAction *a = write()->view()->actionCollection()->action("tools_comment");
    if (a)
      a->activate();
  }
}

void QuantaView::slotUnComment ()
{
  if (writeExists())
  {
    KAction *a = write()->view()->actionCollection()->action("tools_uncomment");
    if (a)
      a->activate();
  }
}


void QuantaView::slotApplyWordWrap ()
{
  if (writeExists())
  {
    KAction *a = write()->view()->actionCollection()->action("tools_apply_wordwrap");
    if (a)
      a->activate();
  }
}

void QuantaView::slotGotoLine ()
{
  if (writeExists())
  {
    KAction *a = write()->view()->actionCollection()->action("go_goto_line");
    if (a)
      a->activate();
  }
}

void QuantaView::slotSpellcheck ()
{
  if (writeExists())
  {
    SpellChecker::ref(this)->spellCheck(write()->doc());
  }
}

void QuantaView::toggleBookmark ()
{
  if (writeExists())
  {
    KTextEditor::MarkInterface *markIf = dynamic_cast<KTextEditor::MarkInterface*>(write()->doc());
    uint line, col;
    write()->viewCursorIf->cursorPositionReal(&line, &col);
    int mark = markIf->mark(line);
    if (mark & KTextEditor::MarkInterface::markType01)
    {
      markIf->removeMark(line, KTextEditor::MarkInterface::markType01);
    } else
    {
      markIf->addMark(line, KTextEditor::MarkInterface::markType01);
    }
  }
}

void QuantaView::clearBookmarks ()
{
  if (writeExists())
  {
    KTextEditor::Mark* mark;
    KTextEditor::MarkInterface *markinterface = dynamic_cast<KTextEditor::MarkInterface*>(write()->doc());
    QPtrList<KTextEditor::Mark> marks= markinterface->marks();
    for ( mark = marks.first(); mark; mark = marks.next() )
    {
      markinterface->removeMark(mark->line,  KTextEditor::MarkInterface::markType01);
    }
  }
}

void QuantaView::gotoMark (KTextEditor::Mark *mark)
{
  if (writeExists())
  {
    if (mark->type & KTextEditor::MarkInterface::markType01)
    {
      Document *w = write();
      w->viewCursorIf->setCursorPositionReal(mark->line, 0);
    }
  }
}


void QuantaView::slotPreviousBookmark()
{
  if (writeExists())
  {
    Document *w = write();
    KTextEditor::MarkInterface *markinterface = dynamic_cast<KTextEditor::MarkInterface*>(w->doc());
    QPtrList<KTextEditor::Mark> marks = markinterface->marks();
    if (marks.isEmpty())
      return;

    uint line, col;
    w->viewCursorIf->cursorPositionReal(&line, &col);
    int found = -1;

    for (uint i = 0; i < marks.count(); i++)
    {
      if ( (marks.at(i)->type & KTextEditor::MarkInterface::markType01) &&
           (marks.at(i)->line < line) &&
           ((found == -1) || (uint(found) < marks.at(i)->line)))
        found = marks.at(i)->line;
    }

    if (found != -1)
      w->viewCursorIf->setCursorPositionReal(found, 0);
  }
}

void QuantaView::slotNextBookmark()
{
  if (writeExists())
  {
    Document *w = write();
    KTextEditor::MarkInterface *markinterface = dynamic_cast<KTextEditor::MarkInterface*>(w->doc());
    QPtrList<KTextEditor::Mark> marks = markinterface->marks();
    if (marks.isEmpty())
      return;

    uint line, col;
    w->viewCursorIf->cursorPositionReal(&line, &col);
    int found = -1;

    for (uint i = 0; i < marks.count(); i++)
    {
      if ( (marks.at(i)->type & KTextEditor::MarkInterface::markType01) &&
           (marks.at(i)->line > line) &&
           ((found == -1) || (uint(found) > marks.at(i)->line)))
        found = marks.at(i)->line;
    }

    if (found != -1)
      w->viewCursorIf->setCursorPositionReal(found, 0);
  }
}

void QuantaView::toggleIconBorder ()
{
  if (writeExists())
  {
    KToggleAction *a = dynamic_cast<KToggleAction*>(write()->view()->actionCollection()->action("view_border"));
    if (a)
    {
      a->activate();
      qConfig.iconBar = a->isChecked();
    }
  }
}

void QuantaView::toggleDynamicWordWrap()
{
  if (writeExists())
  {
    qConfig.dynamicWordWrap = !qConfig.dynamicWordWrap ;
    dynamic_cast<KTextEditor::DynWordWrapInterface *>(write()->view())->setDynWordWrap(qConfig.dynamicWordWrap);
  }
}

void QuantaView::toggleLineNumbers()
{
  if (writeExists())
  {
    KToggleAction *a = dynamic_cast<KToggleAction*>(write()->view()->actionCollection()->action("view_line_numbers"));
    if (a)
    {
      a->activate();
      qConfig.lineNumbers = a->isChecked();
    }
  }
}

void QuantaView::slotEditorOptions()
{
  if (writeExists())
  {
    KConfig *config = quantaApp->config();
    write()->configIf->configDialog();
    write()->writeConfig(config);
    config->sync();

#if KDE_IS_VERSION(3,1,90)
    config->setGroup("Kate View Defaults");
    qConfig.lineNumbers = config->readBoolEntry("Line Numbers", false);
    qConfig.iconBar = config->readBoolEntry("Icon Bar", false);
    qConfig.dynamicWordWrap = config->readBoolEntry("Dynamic Word Wrap",false);
#else
    config->setGroup("Kate View");
    qConfig.lineNumbers = config->readBoolEntry("LineNumbers", false);
    qConfig.iconBar = config->readBoolEntry("Iconbar", false);
    qConfig.dynamicWordWrap = config->readBoolEntry("DynamicWordWrap",false);
#endif
    quantaApp->viewBorder->setChecked(qConfig.iconBar);
    quantaApp->viewLineNumbers->setChecked(qConfig.lineNumbers);
    quantaApp->viewDynamicWordWrap->setChecked(qConfig.dynamicWordWrap);
  }
}

void QuantaView::setEol(int which)
{
  if (writeExists())
  {
    KSelectAction *a = dynamic_cast<KSelectAction*>(write()->view()->actionCollection()->action("set_eol"));
    if (a)
    {
      a->setCurrentItem(which);
      a->activate();
      write()->setModified(true);
    }
  }
}

/** insert special character */
void QuantaView::slotInsertChar(const QString &selected)
{
  if (writeExists())
  {
    int begin = selected.find("(&")+1;
    int length = selected.find(";)") - begin + 1;
    QString part = selected.mid(begin, length);
    write()->insertTag(part);
  }
}

Node *QuantaView::showTagDialogAndReturnNode(const QString &tag, const QString &attr)
{
#ifdef BUILD_KAFKAPART
  Node *n = 0L;
  if(writeExists())
  {
    Document *w = write();

    TagDialog *dlg = new TagDialog(QuantaCommon::tagFromDTD(w->getDTDIdentifier(),tag), attr, baseURL());
    if (dlg->exec())
    {
      n= dlg->buildNode(w);
    }

    delete dlg;
  }
  return n;
#else
  Q_UNUSED(tag);
  Q_UNUSED(attr);
  return 0L;
#endif
}

/** Insert a new tag by bringing up the TagDialog. */
void QuantaView::insertNewTag(const QString &tag, const QString &attr, bool insertInLine)
{
  if (writeExists())
  {
    Document *w = write();

    TagDialog *dlg = new TagDialog(QuantaCommon::tagFromDTD(w->getDTDIdentifier(),tag), attr, baseURL());
    if (dlg->exec())
    {
      dlg->insertTag(w, insertInLine);
    }

    delete dlg;
  }
}
/** Returns the baseURL of the document. */
KURL QuantaView::baseURL()
{
  Document *w = write();
  KURL base;
  if ( !w->isUntitled() )
  {
    base = QuantaCommon::convertToPath(w->url());
  } else
  {
    base = Project::ref()->projectBaseURL();
  }

  return base;
}

void QuantaView::slotCloseOtherTabs()
{
  bool block = m_writeTab->signalsBlocked();
  m_writeTab->blockSignals(true);
  QWidget *w = m_writeTab->currentPage();
  int i = 0;
  while (m_writeTab->count() > 1)
  {
    if (w != m_writeTab->page(i))
    {
      quantaApp->slotClosePage(m_writeTab->page(i));
      i = 0;
    } else
      i++;
  }
  m_writeTab->blockSignals(block);
}


void QuantaView::debugToggleBreakpoint ()
{
  if (writeExists())
  {
    KTextEditor::MarkInterface *markIf = dynamic_cast<KTextEditor::MarkInterface*>(write()->doc());
    uint line, col;
    write()->viewCursorIf->cursorPositionReal(&line, &col);
    int mark = markIf->mark(line);
    if (mark & KTextEditor::MarkInterface::markType02)
    {
      if(quantaApp->debugger()->removeBreakpoint(write()->url(), line))
         markIf->removeMark(line, KTextEditor::MarkInterface::markType02);
    }
    else
    {
      if(quantaApp->debugger()->setBreakpoint(write()->url(), line))
         markIf->addMark(line, KTextEditor::MarkInterface::markType02);
    }
  }
}
void QuantaView::debugClearBreakpoints ()
{
  if (writeExists())
  {
    KTextEditor::Mark* mark;
    KTextEditor::MarkInterface *markinterface = dynamic_cast<KTextEditor::MarkInterface*>(write()->doc());
    QPtrList<KTextEditor::Mark> marks= dynamic_cast<KTextEditor::MarkInterface*>(write()->doc())->marks();
    for ( mark = marks.first(); mark; mark = marks.next() )
    {
      if(quantaApp->debugger()->removeBreakpoint(write()->url(), mark->line))
         markinterface->removeMark(mark->line,  KTextEditor::MarkInterface::markType02);
    }
  }
}
void QuantaView::debugGotoBreakpoint (KTextEditor::Mark *mark)
{
  if (writeExists())
  {
    if (mark->type == KTextEditor::MarkInterface::markType02)
    {
      Document *w = write();
      w->viewCursorIf->setCursorPositionReal(mark->line, 0);
    }
  }
}


