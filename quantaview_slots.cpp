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
#include <qtextedit.h>

// include files for KDE
#include <kapp.h>
#include <klocale.h>
#include <kprocess.h>
#include <kcolordlg.h>
#include <kmessagebox.h>

#include <ktexteditor/configinterface.h>


// application specific includes
#include "document.h"
#include "quanta.h"
#include "quantadoc.h"
#include "quantaview.h"
#include "resource.h"
#include "qextfileinfo.h"
#include "project/project.h"
#include "messages/messageoutput.h"

// dialogs headers
#include "tagdialogs/tagdialog.h"

#include "tagdialogs/tagquickstart.h"
#include "tagdialogs/tagquicklistdlg.h"
#include "tagdialogs/tagquicktable.h"
#include "tagdialogs/cssdialogi.h"
#include "tagdialogs/tagmaildlg.h"
#include "tagdialogs/tagmiscdlg.h"

#include "qdom.h"

void QuantaView::slotEditCurrentTag()
{

  Document *w = write();
  w->currentTag();
  QString tag = w->getTagAttr(0);

  if ( tagsList->find( tag.upper()) != -1 )
  {
    QString attrs = "";
    for (int i=1; i < w->tagAttrNum; i++ )
    {
//      QString *attr = new QString(w->getTagAttr(i));
//      QString *val = new QString(w->getTagAttrValue(i));
//      dlg->insertAttribute(attr,val);
     attrs += QString(w->getTagAttr(i)) + "=" + QString(w->getTagAttrValue(i)) + " ";
    }

    TagDialog *dlg = new TagDialog( tag, attrs );


    if (dlg->exec())
    {
     w->changeCurrentTag( dlg->getAttributes() );
    }

    delete dlg;
  }
  else
  {
    QString message = i18n("Unknown tag : ");
    message += tag;
    app->slotStatusMsg( message.data() );
  }
}

/** edit tag */
void QuantaView::slotInsertCSS()
{

  Document *w = write();

  CSSDialogI *dlg = new CSSDialogI( doc->basePath(), 0L, "CSS dialog", true );
  if ( dlg->exec() ) {
    w->insertTag( dlg->data() );
  }

  delete dlg;
}


/** for <a href> tag */
void QuantaView::slotTagA()
{
  insertNewTag("a");
}

/** insert <img > tag */
void QuantaView::slotTagImg()
{
  insertNewTag("img");
}

/** insert  <font> tag in document */
void QuantaView::slotTagFont()
{
  insertNewTag("font");
}

/** insert tag <basefont> */
void QuantaView::slotTagBaseFont()
{
  insertNewTag("basefont");
}

/** for tag <table> */
void QuantaView::slotTagTable()
{
  insertNewTag("table");
}

/** for row properties */
void QuantaView::slotTagTableRow()
{
  insertNewTag("tr");
}

/** head properties */
void QuantaView::slotTagTableHead()
{
  insertNewTag("th");
}

/** table data properties */
void QuantaView::slotTagTableData()
{
  insertNewTag("td");
}

// tbody
void QuantaView::slotTagTableBody()
{
  insertNewTag("tbody");
}

/** insert TextArea tag */
void QuantaView::slotTagFormTextArea()
{
  insertNewTag("textarea");
}

/** insert Radio Button tag */
void QuantaView::slotTagFormRadio()
{
  insertNewTag("input", "type='radio'");
}

void QuantaView::slotTagForm()
{
  insertNewTag("form");
}

/** insert check button */
void QuantaView::slotTagFormCheck()
{
  insertNewTag("input", "type='checkbox'");
}

/** insert line edit */
void QuantaView::slotTagFormLineEdit()
{
  insertNewTag("input", "type='text'");
}

/** insert password button */
void QuantaView::slotTagFormPas()
{
  insertNewTag("input", "type='password'");
}

/** submit */
void QuantaView::slotTagFormSubmit()
{
  insertNewTag("input", "type='submit'");
}
/** reset */
void QuantaView::slotTagFormReset()
{
  insertNewTag("input", "type='reset'");
}


/** for <a href=mailto> tag  */
void QuantaView::slotTagMail()
{
	TagMailDlg *mailDlg = new TagMailDlg( this, i18n("E-mail link (mailto)..."));

  if ( mailDlg->exec() ) {
  	QString tag = QString(tagCase("<a"));
  	
  	if ( !QString(mailDlg->lineEmail->text()).isEmpty())
  	{
  		tag += attrCase(" href=\"")+"mailto:"+mailDlg->lineEmail->text();
  	  	if ( !QString(mailDlg->lineSubject->text()).isEmpty())
	   		tag += "?subject="+mailDlg->lineSubject->text();
  	   	tag += "\"";
  	}
  	
  	if ( !QString(mailDlg->titleEdit->text()).isEmpty())
	   		tag += attrCase(" title=\"")+mailDlg->titleEdit->text()+"\"";
    tag += QString(">");
    write()->insertTag(tag,tagCase("</a>"));
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

 TagMiscDlg *miscDlg = new TagMiscDlg(element, addClosingTag, this, i18n("Misc. tag..."));

  if ( miscDlg->exec() ) {
  	QString tag;
  	element = miscDlg->elementName->text();
  	if ( !element.isEmpty())
  	{
  		tag += "<" + attrCase(element)+">";
  	  	if ( (addClosingTag = miscDlg->addClosingTag->isChecked()) == true)
        {
           	write()->insertTag(tag,tagCase( "</"+attrCase(element)+">"));
         } else
         {
		    write()->insertTag(tag,tagCase(""));
         }
  	}  	
  }
  delete miscDlg;
}


/** quick html text generate */
void QuantaView::slotTagQuickStart(){
	TagQuickStart *quickDlg = new TagQuickStart( doc->basePath(), this, i18n("Generate HTML text..."));

  if ( quickDlg->exec() ) {
  	QString tag = QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\">\n")+tagCase("<html>\n")
  	                  +space+tagCase("<head>\n")+space+tagCase("  <title>");
  	if ( !QString(quickDlg->lineTitle->text()).isEmpty())
	   		tag += quickDlg->lineTitle->text();
    tag += tagCase("</title>\n")+space+"  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\n"+space+"  <meta name=\"GENERATOR\" content=\"Quanta Plus\">\n"+space+tagCase("</head>\n")+space+tagCase("<body");
    if ( !QString(quickDlg->lineBGImage->text()).isEmpty())
	   		tag += attrCase(" background=\"")+quickDlg->lineBGImage->text()+"\"";
	  if ( !QString(quickDlg->comboBGColor->currentText()).isEmpty())
	   		tag += attrCase(" bgcolor=\"")+quickDlg->comboBGColor->currentText()+"\"";
	  if ( !QString(quickDlg->comboTextColor->currentText()).isEmpty())
	   		tag += attrCase(" text=\"")+quickDlg->comboTextColor->currentText()+"\"";
	  if ( !QString(quickDlg->comboLinkColor->currentText()).isEmpty())
	   		tag += attrCase(" link=\"")+quickDlg->comboLinkColor->currentText()+"\"";
	  if ( !QString(quickDlg->comboALinkColor->currentText()).isEmpty())
	   		tag += attrCase(" alink=\"")+quickDlg->comboALinkColor->currentText()+"\"";
	  if ( !QString(quickDlg->comboVLinkColor->currentText()).isEmpty())
	   		tag += attrCase(" vlink=\"")+quickDlg->comboVLinkColor->currentText()+"\"";
    tag += QString(">\n")+space+QString("  ");
    write()->insertTag(tag,QString("\n")+space+tagCase("</body>\n")+space+tagCase("</html>"));
  }
  delete quickDlg;
}

/** do quick list */
void QuantaView::slotTagQuickList(){
	TagQuickListDlg *listDlg = new TagQuickListDlg(this,i18n("Generate list..."));
  if ( listDlg->exec() ) {
  	int i;
    int n = listDlg->spinBoxRows->value();
    		    	
    QString tag;
    if ( listDlg->radioOrdered->isChecked())
    	tag = QString("<ol>\n")+space;
    else tag = QString("<ul>\n")+space;

    for ( i=0;i<n;i++)
      if ( useCloseTag )
        tag += QString("  <li>  </li>\n")+space;
      else
        tag += QString("  <li> \n")+space;

    if ( listDlg->radioOrdered->isChecked())
    	tag += QString("</ol>");
    else tag += QString("</ul>");

    write()->insertTag( tagCase(tag));
  }
  delete(listDlg);
}

/** for quick create table */
void QuantaView::slotTagQuickTable()
{
	int y,x;

	TagQuickTable *quickDlg = new TagQuickTable(write(), this,i18n("Generate table..."));
	
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
  	
 	  	write()->insertTag( tagCase(tag) );
  }
  delete( quickDlg);
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

/** insert Form tag */

/** insert <br> tag */
void QuantaView::slotTagBr(){
  insertTag("br");
}

/** insert  &nbsp; */
void QuantaView::slotTagNbsp(){
  write()->insertTag("&nbsp;");
}

/** insert  &szlig; */
void QuantaView::slotTagSzlig(){
  write()->insertTag("&szlig;");
}

/** insert  &uuml; */
void QuantaView::slotTaguuml(){
  write()->insertTag("&uuml;");
}

/** insert  &ouml; */
void QuantaView::slotTagouml(){
  write()->insertTag("&ouml;");
}

/** insert  &auml; */
void QuantaView::slotTagauml(){
  write()->insertTag("&auml;");
}

/** insert  &Uuml; */
void QuantaView::slotTagUuml(){
  write()->insertTag("&Uuml;");
}

/** insert  &Ouml; */
void QuantaView::slotTagOuml(){
  write()->insertTag("&Ouml;");
}

/** insert  &Auml; */
void QuantaView::slotTagAuml(){
  write()->insertTag("&Auml;");
}

/** insert  &amp; */
void QuantaView::slotTagAmp(){
  write()->insertTag("&amp;");
}

/** insert  &lt; */
void QuantaView::slotTagLt(){
  write()->insertTag("&lt;");
}

/** insert  &gt; */
void QuantaView::slotTagGt(){
  write()->insertTag("&gt;");
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
  write()->insertTag(tagCase("<select")+ attrCase("name")+tagCase("=\"\"><option>"),tagCase("</select>"));
}

void QuantaView::slotViewInNetscape()
{
  if (write()->isModified())
  {
    dontShowSavePreview = "AskForSaveBeforePreview";
    if ( KMessageBox::questionYesNo(this,i18n("The file must be saved before external preview.\n \
         Do you want to save and preview?"),i18n("Save before preview"),i18n("&Yes"),i18n("&No"),dontShowSavePreview)
         == KMessageBox::Yes)
    {
      if (write()->isUntitled())
      {
       getApp()->slotFileSaveAs();
      }
      else
      {
       write()->doc()->save();
      }
    } else
    {
      return;
    }
  }
  if ( !write()->isUntitled() )
  {
    KProcess *show = new KProcess();
    QString url = app->project->urlWithPrefix(write()->url());

    *show << "netscape" << "-remote" << QString(QString("openURL(")+url+")").data();
    connect( show, SIGNAL(processExited(KProcess *)), this, SLOT(slotNetscapeStatus(KProcess *)));
    show->start( KProcess::NotifyOnExit );
  }
}

void QuantaView::slotViewInKFM()
{
  if (write()->isModified())
  {
    dontShowSavePreview = "AskForSaveBeforePreview";
    if ( KMessageBox::questionYesNo(this,i18n("The file must be saved before external preview.\n \
         Do you want to save and preview?"),i18n("Save before preview"),i18n("&Yes"),i18n("&No"),dontShowSavePreview)
         == KMessageBox::Yes)
    {
      if (write()->isUntitled())
      {
       getApp()->slotFileSaveAs();
      }
      else
      {
       write()->doc()->save();
      }
    } else
    {
      return;
    }
  }
  if ( !write()->isUntitled() )
  {
    KProcess *show = new KProcess();
    QString url = app->project->urlWithPrefix(write()->url());
    *show << "kfmclient" << "exec" << url;
    show->start( KProcess::DontCare );
  }
}

void QuantaView::slotViewInLynx()
{
  if (write()->isModified())
  {
    dontShowSavePreview = "AskForSaveBeforePreview";
    if ( KMessageBox::questionYesNo(this,i18n("The file must be saved before external preview.\n \
         Do you want to save and preview?"),i18n("Save before preview"),i18n("&Yes"),i18n("&No"),dontShowSavePreview)
         == KMessageBox::Yes)
    {
      if (write()->isUntitled())
      {
       getApp()->slotFileSaveAs();
      }
      else
      {
       write()->doc()->save();
      }
    } else
    {
      return;
    }
  }
  if ( !write()->isUntitled() )
  {
    KProcess *show = new KProcess();
    QString url = app->project->urlWithPrefix(write()->url());
    *show << "konsole"
          << "--nohist"
          << "--notoolbar"
          << "--caption"
          << "Lynx Preview - Quanta"
          << "-e"
          << "lynx"
          << url;
    show->start( KProcess::DontCare );
  }
}

/** check netscape status */
void QuantaView::slotNetscapeStatus(KProcess *proc)
{
  if ( proc->exitStatus() ) 
  {
    KProcess *show = new KProcess();
    QString url = app->project->urlWithPrefix(write()->url());
    *show << "netscape" << url;
    show->start( KProcess::DontCare );
  }
}

/** insert tags from tree view */
void QuantaView::slotInsertTagFromTree(QString name)
{
	QExtFileInfo file( name );
	file.convertToRelative( doc->basePath() );
	QString shortName = file.filePath();
	
	if ( QDir::match( fileMaskImage, name) )
	{
		write()->insertTag( tagCase("<img")+attrCase(" src=\"")+shortName+"\">");
	}
	else {
	  write()->insertTag( tagCase("<a")+attrCase(" href=\"")+shortName+"\">",tagCase("</a>"));
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

  QString output(buffer);
  output.truncate(buflen);

  if ( scriptOutputDest == "cursor" )
  	write()->insertTag(output);

  if ( scriptOutputDest == "message" ) {

			if ( beginOfScriptOutput ) {
        //if ( !app->viewMenu->isItemChecked(ID_VIEW_MES) )
        //  app->slotViewMes();
        app->messageOutput->clear();
        app->messageOutput->insertItem("Script output:\n");
      }

      app->messageOutput->showMessage(output);
  }	

  if ( scriptOutputDest == "new" ) 
  {
		 if ( beginOfScriptOutput )
        doc->openDocument( KURL() );
     write()->insertTag(output);
  }

  if ( scriptOutputDest == "replace" ) 
  {
		 if ( beginOfScriptOutput ) write()->editIf->clear();
     write()->insertTag(output);
  }

  beginOfScriptOutput = false;

}

/** get output */
void QuantaView::slotGetScriptError(KProcess *, char *buffer, int buflen)
{

  QString output(buffer);
  output.truncate(buflen);

  if ( scriptErrorDest == "merge" ) {
  	scriptErrorDest = scriptOutputDest;
  	beginOfScriptError = beginOfScriptOutput;
  }

  if ( scriptErrorDest == "cursor" )
  	write()->insertTag(output);

  if ( scriptErrorDest == "message" ) {

			if ( beginOfScriptError ) {
        //if ( !app->viewMenu->isItemChecked(ID_VIEW_MES) )
        //  app->slotViewMes();
        app->messageOutput->clear();
        app->messageOutput->insertItem("Script output:\n");
      }
        
      app->messageOutput->showMessage( output );  	
  }	

  if ( scriptErrorDest == "new" ) 
  {
		 if ( beginOfScriptError )
        doc->openDocument( KURL() );
     write()->insertTag(output);
  }

  if ( scriptErrorDest == "replace" ) 
  {
		 if ( beginOfScriptError ) write()->editIf->clear();
     write()->insertTag(output);
  }

  beginOfScriptError = false;

}

/** insert clipboard contents (but quote them for HTML first) */
void QuantaView::slotPasteHTMLQuoted()
{
    QClipboard *cb = qApp->clipboard();
    QString text = cb->text();

    if ( ( !text.isNull() ) && (!text.isEmpty() ) ) {
        text.replace( QRegExp( I18N_NOOP( "&" ) ), I18N_NOOP( "&amp;" ) );
        text.replace( QRegExp( I18N_NOOP( "<" ) ), I18N_NOOP( "&lt;" ) );
        text.replace( QRegExp( I18N_NOOP( "\"" ) ), I18N_NOOP( "&quot;" ) );
        unsigned int line, col;
        write()->viewCursorIf->cursorPosition(&line, &col);
        write()->editIf->insertText(line, col, text );
    }
}

/** insert clipboard contents (but quote them as a URL first) */
void QuantaView::slotPasteURLEncoded()
{
    QClipboard *cb = qApp->clipboard();
    QString text = cb->text();

    if ( ( !text.isNull() ) && (!text.isEmpty() ) ) {
        text = KURL::encode_string( text );
        unsigned int line, col;
        write()->viewCursorIf->cursorPosition(&line, &col);
        write()->editIf->insertText(line, col, text );
    }
}


/** Kate releated slots. */


void QuantaView::slotUndo ()
{
  write()->kate_doc->undo();
}

void QuantaView::slotRedo ()
{
  write()->kate_doc->redo();
}

void QuantaView::slotCut ()
{
  write()->kate_view->cut();
}

void QuantaView::slotCopy ()
{
  write()->kate_view->copy();
}

void QuantaView::slotPaste ()
{
  write()->kate_view->paste();
}

void QuantaView::slotSelectAll ()
{
  write()->kate_doc->selectAll();
}

void QuantaView::slotDeselectAll ()
{
  write()->kate_doc->clearSelection ();
}

void QuantaView::toggleVertical()
{
  write()->kate_doc->toggleBlockSelectionMode();
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

void QuantaView::slotComment ()
{
   write()->kate_view->comment();
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
   write()->kate_doc->spellcheck();
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
}

void QuantaView::toggleLineNumbers()
{
  write()->kate_view->toggleLineNumbersOn();
}

void QuantaView::editorOptions()
{
  dynamic_cast<KTextEditor::ConfigInterface *>(write()->doc())->configDialog();
  write()->writeConfig(app->config);
}

void QuantaView::setEol(int which)
{
   write()->kate_view->setEol( which );
}

/** insert special character */
void QuantaView::slotInsertChar(const QString &selected){
	int begin = selected.find("(")+1;
    int length = selected.find(")") - begin;
	QString part = selected.mid(begin, length);
    write()->insertTag(part);
}

/** Insert a new tag by bringing up the TagDialog. */
void QuantaView::insertNewTag(QString tag, QString attr,bool insertInLine)
{
  Document *w = write();

  TagDialog *dlg = new TagDialog(tag, attr);
  dlg->setBasePath(w); //It is very important to call this function!!!
  if (dlg->exec())
  {
   dlg->insertTag(w, insertInLine);
  }

  delete dlg;
}
