/***************************************************************************
                          quantaview_slots.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavsky
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

// include files for KDE
#include <kapp.h>
#include <klocale.h>
#include <kprocess.h>
#include <kcolordlg.h>

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

#include "qdom.h"

void QuantaView::slotEditCurrentTag()
{

  Document *w = write();
  w->currentTag();
  QString tag = w->getTagAttr(0);

  if ( tagsList->find( tag.upper()) != -1 ) {
    TagDialog *dlg = new TagDialog( w );
    dlg->show();
  } else {
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

  delete (dlg);
}


/** for <a href> tag */
void QuantaView::slotTagA()
{
  TagDialog *dlg = new TagDialog( write(), "a");
  dlg->show();
}

/** insert <img > tag */
void QuantaView::slotTagImg()
{
  TagDialog *dlg = new TagDialog( write(), "img");
  dlg->show();
}

/** insert  <font> tag in document */
void QuantaView::slotTagFont()
{
  TagDialog *dlg = new TagDialog( write(), "font");
  dlg->show();
}

/** insert tag <basefont> */
void QuantaView::slotTagBaseFont()
{
  TagDialog *dlg = new TagDialog( write(), "basefont");
  dlg->show();
}

/** for tag <table> */
void QuantaView::slotTagTable()
{
  TagDialog *dlg = new TagDialog( write(), "table");
  dlg->show();
}

/** for row properties */
void QuantaView::slotTagTableRow()
{
  TagDialog *dlg = new TagDialog( write(), "tr");
  dlg->show();
}

/** head properties */
void QuantaView::slotTagTableHead()
{
  TagDialog *dlg = new TagDialog( write(), "th");
  dlg->show();
}

/** table data properties */
void QuantaView::slotTagTableData()
{
  TagDialog *dlg = new TagDialog( write(), "td");
  dlg->show();
}

// tbody
void QuantaView::slotTagTableBody()
{
  TagDialog *dlg = new TagDialog( write(), "tbody");
  dlg->show();
}

/** insert TextArea tag */
void QuantaView::slotTagFormTextArea()
{
  TagDialog *dlg = new TagDialog( write(), "textarea");
  dlg->show();
}

/** insert Radio Button tag */
void QuantaView::slotTagFormRadio()
{
  TagDialog *dlg = new TagDialog( write(), "input", "type='radio'");
  dlg->show();
}

void QuantaView::slotTagForm()
{
  TagDialog *dlg = new TagDialog( write(), "form");
  dlg->show();
}

/** insert check button */
void QuantaView::slotTagFormCheck()
{
  TagDialog *dlg = new TagDialog( write(), "input", "type='checkbox'");
  dlg->show();
}

/** insert line edit */
void QuantaView::slotTagFormLineEdit()
{
  TagDialog *dlg = new TagDialog( write(), "input", "type='text'");
  dlg->show();
}

/** insert password button */
void QuantaView::slotTagFormPas()
{
  TagDialog *dlg = new TagDialog( write(), "input", "type='password'");
  dlg->show();
}

/** submit */
void QuantaView::slotTagFormSubmit()
{
  TagDialog *dlg = new TagDialog( write(), "input", "type='submit'");
  dlg->show();
}
/** reset */
void QuantaView::slotTagFormReset()
{
  TagDialog *dlg = new TagDialog( write(), "input", "type='reset'");
  dlg->show();
}


/** for <a href=mailto> tag  */
void QuantaView::slotTagMail()
{
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
  delete(quickDlg);
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

	TagQuickTable *quickDlg = new TagQuickTable(this,i18n("Generate table..."));
	
  if ( quickDlg->exec() ) {
  	y = quickDlg->spinBoxRows->value();
  	x = quickDlg->spinBoxCollums->value();
  	
  	QString tag = QString("<table>\n")+space;
  	
 	  for ( int i=0;i<y;i++) {
 		  tag += QString("  <tr>\n")+space;
 		  for ( int j=0;j<x;j++)
 			  tag += QString("    <td>  </td>\n")+space;
 		  tag += QString("  </tr>\n")+space;
 	  }
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
  write()->save();
  if ( !write()->isUntitled() ) 
  {
    KProcess *show = new KProcess();
    *show << "netscape" << "-remote" << QString(QString("openURL(")+write()->url().url()+")").data();
    connect( show, SIGNAL(processExited(KProcess *)), this, SLOT(slotNetscapeStatus(KProcess *)));
    show->start( KProcess::NotifyOnExit );
  }
}

void QuantaView::slotViewInKFM()
{
  write()->save();
  if ( !write()->isUntitled() ) 
  {
    KProcess *show = new KProcess();
    *show << "kfmclient" << "exec" << write()->url().url();
    show->start( KProcess::DontCare );
  }
}

void QuantaView::slotViewInLynx()
{
  write()->save();
  if ( !write()->isUntitled() ) 
  {
    KProcess *show = new KProcess();
    *show << "konsole" 
          << "--nohist" 
          << "--notoolbar"
          << "--caption"
          << "Lynx Preview - Quanta"
          << "-e"
          << "lynx"
          << write()->url().url();
    show->start( KProcess::DontCare );
  }
}

/** check netscape status */
void QuantaView::slotNetscapeStatus(KProcess *proc)
{
  if ( proc->exitStatus() ) 
  {
    KProcess *show = new KProcess();
    *show << "netscape" << write()->url().url();
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
		 if ( beginOfScriptOutput )
        write()->setText("");
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
		 if ( beginOfScriptError )
        write()->setText("");
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
        write()->insertText( text );
    }
}

/** insert clipboard contents (but quote them as a URL first) */
void QuantaView::slotPasteURLEncoded()
{
    QClipboard *cb = qApp->clipboard();
    QString text = cb->text();

    if ( ( !text.isNull() ) && (!text.isEmpty() ) ) {
        text = KURL::encode_string( text );
        write()->insertText( text );
    }
}
