/***************************************************************************
                          quantaview_slots.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
                           (C) 2002 by Andras Mantia
    email                : pdima@mail.univ.kiev.ua
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

// include files for KDE
#include <kapp.h>
#include <kconfig.h>
#include <klocale.h>
#include <kprocess.h>
#include <kcolordlg.h>
#include <kmessagebox.h>

#include <ktexteditor/configinterface.h>
#include <ktexteditor/clipboardinterface.h>
#include <ktexteditor/selectioninterface.h>


// application specific includes
#include "quantacommon.h"
#include "document.h"
#include "quanta.h"
#include "quantadoc.h"
#include "quantaview.h"
#include "resource.h"
#include "qextfileinfo.h"

#include "parser/tag.h"
#include "project/project.h"
#include "messages/messageoutput.h"

// dialogs headers
#include "tagdialogs/tagdialog.h"

#include "tagdialogs/tagquickstart.h"
#include "tagdialogs/tagquicklistdlg.h"
#include "tagdialogs/tagquicktable.h"
#include "tagdialogs/csseditor.h"
#include "tagdialogs/cssselectoreditor.h"
#include "tagdialogs/tagmaildlg.h"
#include "tagdialogs/tagmiscdlg.h"

#include "plugins/spellchecker.h"

#include "qdom.h"

void QuantaView::slotEditCurrentTag()
{

  Document *w = write();
  uint line,col;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  DTDStruct *dtd = w->currentDTD();
  QString dtdName = dtd->name;
  Tag *tag = w->tagAt(dtd, line, col);
  bool isUnknown = true;
  QString tagName;
  if (tag)
  {
    tagName = tag->name;
    if ( QuantaCommon::isKnownTag(dtd->name,tagName) )
    {
      isUnknown = false;
      TagDialog *dlg = new TagDialog( QuantaCommon::tagFromDTD(dtd,tagName), tag, baseURL() );
      if (dlg->exec())
      {
       w->changeTag(tag, dlg->getAttributes() );
      }

      delete dlg;
    }
    delete tag;
  }

  if (isUnknown)
  {
    dtd = w->defaultDTD();
    tag = w->tagAt(dtd, line, col, false, true);
    if (tag)
    {
      tagName = tag->name;
      if ( QuantaCommon::isKnownTag(dtd->name,tagName) )
      {
        isUnknown = false;
        TagDialog *dlg = new TagDialog( QuantaCommon::tagFromDTD(dtd,tagName), tag );

        if (dlg->exec())
        {
         w->changeTag(tag, dlg->getAttributes() );
        }
        delete dlg;
      }
      delete tag;
    }
  }
  if (isUnknown)
  {
    QString message = i18n("Unknown tag: %1").arg(tagName);
    quantaApp->slotStatusMsg( message.data() );
  }
}

/** edit tag */
void QuantaView::slotInsertCSS()
{
 Document *w = write();
/*
 CSSEditor* dlg = new CSSEditor("", 0L, "CSS Editor");
 if (dlg->exec())
 {
   w->insertTag( dlg->code() );
  // Todo
 }

  delete dlg;*/
  QString code="";
  
  uint line, col;
  w->viewCursorIf->cursorPositionReal(&line, &col);

  bool insertNew = true;
  QRegExp rx("\\b[a-zA-z_]+\\b");
  Tag *tag = w->findScriptText(line, col, rx);
  if (tag)
  {
    int bLine, bCol, eLine, eCol;
    tag->beginPos(bLine,bCol);
    tag->endPos(eLine,eCol);
    if (w->text(bLine, bCol-1,bLine, bCol-1) == "{" &&
        w->text(eLine, eCol+1,eLine, eCol+1) == "}")
    {
      code = tag->tagStr();
    	CSSSelectorEditor* dlg = new CSSSelectorEditor (code, false, this,
    		i18n ("Edit selector"));
    	if (dlg->exec())
      {
        w->editIf->removeText(bLine, bCol-1, eLine, eCol+2);
        w->viewCursorIf->setCursorPositionReal((uint)bLine, (uint)bCol-1);
        w->insertText(dlg->code());
    	}

      delete tag;
      delete dlg;
      insertNew = false;
    }
  }
  if (insertNew)
  {
  	CSSSelectorEditor* dlg = new CSSSelectorEditor (code, false, this,
  		i18n ("Insert a new selector"));
  	if (dlg->exec()) {
  		 w->insertTag( dlg->code() );
  	}
    delete dlg;
  }

}

/** for <a href=mailto> tag  */
void QuantaView::slotTagMail()
{
	TagMailDlg *mailDlg = new TagMailDlg( this, i18n("Email Link (mailto)"));

  if ( mailDlg->exec() ) {
  	QString tag = QString(QuantaCommon::tagCase("<a"));

  	if ( !QString(mailDlg->lineEmail->text()).isEmpty())
  	{
  		tag += QuantaCommon::attrCase(" href=\"")+"mailto:"+mailDlg->lineEmail->text();
  	  	if ( !QString(mailDlg->lineSubject->text()).isEmpty())
	   		tag += "?subject="+mailDlg->lineSubject->text();
  	   	tag += "\"";
  	}

  	if ( !QString(mailDlg->titleEdit->text()).isEmpty())
	   		tag += QuantaCommon::attrCase(" title=\"")+mailDlg->titleEdit->text()+"\"";
    tag += QString(">");
    write()->insertTag(tag,QuantaCommon::tagCase("</a>"));
  }
  delete mailDlg;
}

/** Add the starting and closing text for a
user specified tag. */
//FIXME: In the future, the user should freely specify his tags in external files, and Quanta
// will automatically use them
void QuantaView::slotTagMisc()
{
 static QString element = "";
 static bool addClosingTag = true;

 TagMiscDlg *miscDlg = new TagMiscDlg(element, addClosingTag, this, i18n("Misc. Tag"));

  if ( miscDlg->exec() ) {
  	QString tag;
  	element = miscDlg->elementName->text();
  	if ( !element.isEmpty())
  	{
  		tag += "<" + QuantaCommon::attrCase(element)+">";
  	  	if ( (addClosingTag = miscDlg->addClosingTag->isChecked()) == true)
        {
          write()->insertTag(tag,QuantaCommon::tagCase( "</"+QuantaCommon::attrCase(element)+">"));
        } else
        {
		      write()->insertTag(tag,QuantaCommon::tagCase(""));
        }
  	}
  }
  delete miscDlg;
}


/** quick html text generate */
void QuantaView::slotTagQuickStart(){
	TagQuickStart *quickDlg = new TagQuickStart( quantaApp->projectBaseURL(), this, i18n("Generate HTML Text"));

  if ( quickDlg->exec() )
   {
    const QString chset = QTextCodec::codecForLocale()->mimeName();
  	QString tag = QString("<!DOCTYPE HTML PUBLIC \""+DEFAULT_DTD+"\">\n")+QuantaCommon::tagCase("<html>\n")
  	                  +space+QuantaCommon::tagCase("<head>\n")+space+QuantaCommon::tagCase("  <title>");
  	if ( !QString(quickDlg->lineTitle->text()).isEmpty())
	   		tag += quickDlg->lineTitle->text();
    tag += QuantaCommon::tagCase("</title>\n")+space+
           "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=" + chset + "\">\n"+
           space+"  <meta name=\"GENERATOR\" content=\"Quanta Plus\">\n"+space+
           QuantaCommon::tagCase("</head>\n")+space+QuantaCommon::tagCase("<body");
    if ( !QString(quickDlg->lineBGImage->text()).isEmpty())
	   		tag += QuantaCommon::attrCase(" background=\"")+quickDlg->lineBGImage->text()+"\"";
	  if ( !QString(quickDlg->comboBGColor->currentText()).isEmpty())
	   		tag += QuantaCommon::attrCase(" bgcolor=\"")+quickDlg->comboBGColor->currentText()+"\"";
	  if ( !QString(quickDlg->comboTextColor->currentText()).isEmpty())
	   		tag += QuantaCommon::attrCase(" text=\"")+quickDlg->comboTextColor->currentText()+"\"";
	  if ( !QString(quickDlg->comboLinkColor->currentText()).isEmpty())
	   		tag += QuantaCommon::attrCase(" link=\"")+quickDlg->comboLinkColor->currentText()+"\"";
	  if ( !QString(quickDlg->comboALinkColor->currentText()).isEmpty())
	   		tag += QuantaCommon::attrCase(" alink=\"")+quickDlg->comboALinkColor->currentText()+"\"";
	  if ( !QString(quickDlg->comboVLinkColor->currentText()).isEmpty())
	   		tag += QuantaCommon::attrCase(" vlink=\"")+quickDlg->comboVLinkColor->currentText()+"\"";
    tag += QString(">\n")+space+QString("  ");
    write()->insertTag(tag,QString("\n")+space+QuantaCommon::tagCase("</body>\n")+space+QuantaCommon::tagCase("</html>"));
  }
  delete quickDlg;
}

/** do quick list */
void QuantaView::slotTagQuickList(){
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

/** for quick create table */
void QuantaView::slotTagQuickTable()
{
	int y,x;

	TagQuickTable *quickDlg = new TagQuickTable(write(), this,i18n("Generate Table"));

  if ( quickDlg->exec() )
  {
  		y = quickDlg->SpinBoxRow->value();
  		x = quickDlg->SpinBoxCol->value();

  		QString tag = QString("<table>\n")+space;

      if (quickDlg->useCaption->isChecked())
      {
        tag +="  <caption>\n     ";
        tag += quickDlg->CaptionTextEdit->text();
        tag +="\n  </caption>\n";
      }

      if (quickDlg->useCols->isChecked())
      {
        QListViewItem *item = quickDlg->ColumnsListView->firstChild();
        QString value;
        for (int i = 1; i <= x; i++)
        {
          tag +="  <col";
          value = item->text(1);
          if (!value.isEmpty())
          {
            tag +=" width=" + value;
          }
          value = item->text(2);
          if (!value.isEmpty())
          {
            tag +=" align=" + value;
          }
          value = item->text(3);
          if (!value.isEmpty())
          {
            tag +=" valign=" + value;
          }
          value = item->text(4);
          if (!value.isEmpty())
          {
            tag +=" class=" + value;
          }
          tag +=">\n";
          item = item->nextSibling();
        }
      }

  		if (quickDlg->useTHead->isChecked())
  		{
  			tag += "  <thead>\n";
  			tag += "    <tr>\n";
 		  	for ( int j=0;j<x;j++)
 			  tag += QString("      <th scope=col>  </th>\n")+space;
  			tag += "    </tr>\n";
  			tag += "  </thead>\n";
  		}
  		if (quickDlg->useTFoot->isChecked())
  		{
  			tag += "  <tfoot>\n";
  			tag += "    <tr>\n";
  			tag += "    </tr>\n";
  			tag += "  </tfoot>\n";
  		}
  	 	tag+="  <tbody>\n";
     	for ( int i=0;i<y;i++)
     	{
 		  	tag += QString("    <tr>\n")+space;
 		  	for ( int j=0;j<x;j++)
 			  tag += QString("      <td>  </td>\n")+space;
 		  	tag += QString("    </tr>\n")+space;
 	 	}
 	 	tag += "  </tbody>\n";
 	 	tag += QString("</table>");

  	write()->insertTag( QuantaCommon::tagCase(tag) );
  }
  delete quickDlg;
}

/** Open color Dialog and insert color in the text */
void QuantaView::slotTagColor(){
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
void QuantaView::slotTagDate(){
  time_t tektime;
  time( &tektime);
  QString stime = ctime( &tektime);

  write()->insertTag( stime);
}

/** for select form */
void QuantaView::slotTagSelect(){
  write()->insertTag(QuantaCommon::tagCase("<select")+ QuantaCommon::attrCase("name")+QuantaCommon::tagCase("=\"\"><option>"),QuantaCommon::tagCase("</select>"));
}

void QuantaView::slotViewInNetscape()
{
  Document *w = write();
  if (w->isModified())
  {
    dontShowSavePreview = "AskForSaveBeforePreview";
    if ( KMessageBox::questionYesNo(this,i18n("The file must be saved before external preview.\n \
         Do you want to save and preview?"),i18n("Save Before Preview"),i18n("&Yes"),i18n("&No"),dontShowSavePreview)
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
    KURL url = quantaApp->project->urlWithPrefix(w->url());

    *show << "netscape" << "-remote" << QString(QString("openURL(")+url.url()+")").data();
    connect( show, SIGNAL(processExited(KProcess *)), this, SLOT(slotNetscapeStatus(KProcess *)));
    show->start( KProcess::NotifyOnExit );
  }
}

void QuantaView::slotViewInKFM()
{
  Document *w = write();
  if (w->isModified())
  {
    dontShowSavePreview = "AskForSaveBeforePreview";
    if ( KMessageBox::questionYesNo(this,i18n("The file must be saved before external preview.\n \
         Do you want to save and preview?"),i18n("Save Before Preview"),i18n("&Yes"),i18n("&No"),dontShowSavePreview)
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
    KURL url = quantaApp->project->urlWithPrefix(w->url());
    *show << "kfmclient" << "exec" << url.url();
    show->start( KProcess::DontCare );
  }
}

void QuantaView::slotViewInLynx()
{
  Document *w = write();
  if (w->isModified())
  {
    dontShowSavePreview = "AskForSaveBeforePreview";
    if ( KMessageBox::questionYesNo(this,i18n("The file must be saved before external preview.\n \
         Do you want to save and preview?"),i18n("Save Before Preview"),i18n("&Yes"),i18n("&No"),dontShowSavePreview)
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
    KURL url = quantaApp->project->urlWithPrefix(w->url());
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

/** check netscape status */
void QuantaView::slotNetscapeStatus(KProcess *proc)
{
  if ( proc->exitStatus() )
  {
    KProcess *show = new KProcess();
    KURL url = quantaApp->project->urlWithPrefix(write()->url());
    *show << "netscape" << url.url();
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
  Document *w = write();

  QString output(QString::fromLocal8Bit(buffer));
  output.truncate(buflen);

  if ( scriptOutputDest == "cursor" )
  	w->insertTag(output);

  if ( scriptOutputDest == "message" ) {

			if ( beginOfScriptOutput ) {
        //if ( !quantaApp->viewMenu->isItemChecked(ID_VIEW_MES) )
        //  quantaApp->slotViewMes();
        quantaApp->messageOutput->clear();
        quantaApp->messageOutput->insertItem(i18n("Script output:\n"));
      }

      quantaApp->messageOutput->showMessage(output);
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

/** get output */
void QuantaView::slotGetScriptError(KProcess *, char *buffer, int buflen)
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

  if ( scriptErrorDest == "message" ) {

			if ( beginOfScriptError ) {
        //if ( !quantaApp->viewMenu->isItemChecked(ID_VIEW_MES) )
        //  quantaApp->slotViewMes();
        quantaApp->messageOutput->clear();
        quantaApp->messageOutput->insertItem(i18n("Script output:\n"));
      }

      quantaApp->messageOutput->showMessage( output );
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

/** insert clipboard contents (but quote them for HTML first) */
void QuantaView::slotPasteHTMLQuoted()
{
  Document *w = write();
  QClipboard *cb = qApp->clipboard();
  QString text = cb->text();

  if ( ( !text.isNull() ) && (!text.isEmpty() ) )
  {
    text.replace( QRegExp( "&" ), "&amp;" );
    text.replace( QRegExp( "<" ), "&lt;" );
    text.replace( QRegExp( "\"" ), "&quot;" );
    unsigned int line, col;
    w->viewCursorIf->cursorPositionReal(&line, &col);
    w->editIf->insertText(line, col, text );
  }
}

/** insert clipboard contents (but quote them as a URL first) */
void QuantaView::slotPasteURLEncoded()
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


/** Kate releated slots. */


void QuantaView::slotUndo ()
{
  dynamic_cast<KTextEditor::UndoInterface*>(write()->doc())->undo();
}

void QuantaView::slotRedo ()
{
  dynamic_cast<KTextEditor::UndoInterface*>(write()->doc())->redo();
}

void QuantaView::slotCut ()
{
  dynamic_cast<KTextEditor::ClipboardInterface*>(write()->view())->cut();
}

void QuantaView::slotCopy ()
{
  dynamic_cast<KTextEditor::ClipboardInterface*>(write()->view())->copy();
}

void QuantaView::slotPaste ()
{
  dynamic_cast<KTextEditor::ClipboardInterface*>(write()->view())->paste();
}

void QuantaView::slotSelectAll ()
{
  dynamic_cast<KTextEditor::SelectionInterface*>(write()->doc())->selectAll();
}

void QuantaView::slotDeselectAll ()
{
  dynamic_cast<KTextEditor::SelectionInterface*>(write()->doc())->clearSelection ();
}

void QuantaView::toggleVertical()
{
  dynamic_cast<KTextEditor::BlockSelectionInterface*>(write()->doc())->toggleBlockSelectionMode();
}



void QuantaView::slotFind ()
{
  write()->kate_view->find();
}

void QuantaView::slotFindAgain ()
{
  write()->kate_view->findAgain(false);
}

void QuantaView::slotFindAgainB ()
{
   write()->kate_view->findPrev();
}

void QuantaView::slotReplace ()
{
   write()->kate_view->replace();
}


void QuantaView::slotIndent()
{
  write()->kate_view->indent();
}

void QuantaView::slotUnIndent()
{
   write()->kate_view->unIndent();
}

void QuantaView::slotCleanIndent()
{
   write()->kate_view->cleanIndent();
}

void QuantaView::slotComment ()
{
  write()->kate_view->comment(); //this is not working correctly in KATE 3.0.x
}

void QuantaView::slotUnComment ()
{
  write()->kate_view->uncomment();
}


void QuantaView::slotApplyWordWrap ()
{
  write()->kate_doc->applyWordWrap();
}

void QuantaView::slotGotoLine ()
{
  write()->kate_view->gotoLine();
}

void QuantaView::slotSpellcheck ()
{
//  write()->kate_doc->spellcheck();
  quantaApp->spellChecker->spellCheck(write()->doc());
}

void QuantaView::toggleBookmark ()
{
  write()->kate_view->toggleBookmark();
}

void QuantaView::clearBookmarks ()
{
  write()->kate_doc->clearMarks();
}

void QuantaView::gotoMark (KTextEditor::Mark *mark)
{
  write()->kate_view->gotoMark (mark);
}

void QuantaView::toggleIconBorder ()
{
  write()->kate_view->toggleIconBorder ();
  qConfig.iconBar = quantaApp->viewBorder->isChecked();
}

void QuantaView::toggleLineNumbers()
{
  write()->kate_view->toggleLineNumbersOn();
  qConfig.lineNumbers = quantaApp->viewLineNumbers->isChecked();
}

void QuantaView::slotEditorOptions()
{
  dynamic_cast<KTextEditor::ConfigInterface *>(write()->doc())->configDialog();
  write()->writeConfig(quantaApp->config);
  quantaApp->config->sync();
}

void QuantaView::setEol(int which)
{
  write()->kate_view->setEol( which );
}

/** insert special character */
void QuantaView::slotInsertChar(const QString &selected)
{
	int begin = selected.find("(")+1;
  int length = selected.find(")") - begin;
	QString part = selected.mid(begin, length);
  write()->insertTag(part);
}

/** Insert a new tag by bringing up the TagDialog. */
void QuantaView::insertNewTag(QString tag, QString attr,bool insertInLine)
{

  Document *w = write();

  TagDialog *dlg = new TagDialog(QuantaCommon::tagFromDTD(w->getDTDIdentifier(),tag), attr, baseURL());
  if (dlg->exec())
  {
   dlg->insertTag(w, insertInLine);
  }

  delete dlg;
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
    base = w->baseURL;
	}

  return base;
}


