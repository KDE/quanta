/***************************************************************************
                          xsldbgdebugger.cpp  -  description
                             -------------------
    begin                : Tue Jan 1 2002
    copyright            : (C) 2002 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***********************************************************************************
 *                                                                         										*
 *   This program is free software; you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or         *
 *   (at your option) any later version.                                   							*
 *                                                                         										*
 ************************************************************************************/

#include <klocale.h>
#include <kurl.h>

#include <libxml/tree.h>
#include <libxslt/xsltInternals.h>
#include "xsldbgdebugger.h"

#include <libxsldbg/xsldbgthread.h>
#include <libxsldbg/xsldbgevent.h>
#include <libxsldbg/qtnotifier2.h>
#include <libxsldbg/options.h>

extern int xsldbgStop;

#include "xsldbgwalkspeedimpl.h"
#include <qmessagebox.h>

static QString fixLocalPaths(QString & file)
{
    QString result;
    KURL url(file);
    if (url.isLocalFile()){
        // libxml2 does not like file:/<localfilepath>
        result = "file://" + url.encodedPathAndQuery();
    }else{
        result = url.url();
    }
    return result;
}

XsldbgDebugger::XsldbgDebugger()
{
   initialized = FALSE;
   inspector = 0L;
   walkDialog = 0L;
   outputFileActive = FALSE;

   updateText = "";
   lastType =  XSLDBG_MSG_AWAITING_INPUT;
	readMsg = FALSE;
	procMsg = FALSE;
	/* set a slow occurance of  timer events to check for xsldbg commands from user */
	updateTimerID  = startTimer(100);

	connectNotifier(this);
}


XsldbgDebugger::~XsldbgDebugger(){
   if (initialized == TRUE)
	 xsldbgThreadFree();

	if (walkDialog != 0L)
		walkDialog->close(TRUE);

}


void  XsldbgDebugger::setInspector(XsldbgInspector *inspector)
{
  this->inspector = inspector;
}

bool XsldbgDebugger::event(QEvent *e)
{
	if (e == 0L)
		return FALSE;

	if (e->type() != QEvent::User)
		return QObject::event(e);
   else{
		/* we now have a notify message from xsldbg */
	   XsldbgEvent *event = dynamic_cast<XsldbgEvent*>(e);
	   	/* send to this debugger the messages in event */
  	  event->emitMessage(this);
  	 }
	return TRUE;

}


void XsldbgDebugger::timerEvent(QTimerEvent *e)
{
	/* This function runs in the application's thread */

	if (e == 0L && (e->timerId() != updateTimerID))
		return;

	if  ((getInputReady() == 0) && (getInputStatus() == XSLDBG_MSG_AWAITING_INPUT) &&
		(commandQue.count() > 0)){
		 QString msg = commandQue.first();
 		 commandQue.remove(msg);
		::fakeInput((const char*)msg.utf8());
	}

      	if ((updateText.length() > 0) && (getInputStatus() == XSLDBG_MSG_AWAITING_INPUT)){
      		/* flush remainding text to message window */
      		emit showMessage(updateText);
			updateText = "";
			lastType  = XSLDBG_MSG_AWAITING_INPUT;
      	}

}

QString XsldbgDebugger::sourceFileName()
{
	QString fileName;

	if (optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME) != 0L)
		fileName = KURL::decode_string((const char*)optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME));

	return fileName;
}

QString XsldbgDebugger::dataFileName()
{
	QString fileName;

	if (optionsGetStringOption(OPTIONS_DATA_FILE_NAME) != 0L)
		fileName = KURL::decode_string((const char*)optionsGetStringOption(OPTIONS_DATA_FILE_NAME));

	return fileName;
}

QString XsldbgDebugger::outputFileName()
{
	QString fileName;

	if (optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME) != 0L)
		fileName = KURL::decode_string((const char*)optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME));

	return fileName;
}

void XsldbgDebugger::gotoLine(QString fileName, int lineNo, bool breakpoint /*= FALSE*/)
{
	emit lineNoChanged(fileName, lineNo, breakpoint);
}


void XsldbgDebugger::setOption(const char* name, bool value)
{
    QString msg = "setoption ";
	 msg.append(name).append(" ").append(QString::number(value));
	fakeInput(msg, TRUE);
}

void XsldbgDebugger::fakeInput(QString text, bool wait)
{
        Q_UNUSED(wait);
	commandQue.append(text);
}


bool XsldbgDebugger::start()
{
		bool result = FALSE;
	  if ((initialized == FALSE) && !xsldbgThreadInit())
		{
			xsldbgThreadFree();
			qDebug("Init of thread failed\n");
		}
		else
	  {
			initialized = TRUE;
			result = TRUE;
		}
		return result;
}

bool XsldbgDebugger::stop()
{
	if (initialized == TRUE){
		setThreadStatus(XSLDBG_MSG_THREAD_STOP);
	}

	  /* it always succeeds at the moment */
		return TRUE;
}

void XsldbgDebugger::slotConfigure()
{
	if (start() == FALSE)
		return;

	if(inspector == 0L ){
			inspector = new XsldbgInspector(this);
			connect(inspector, SIGNAL(closedWindow()), this, SLOT(slotConfigClosed()));
  }
}


void XsldbgDebugger::slotConfigClosed()
{
	inspector = 0L;
}

void XsldbgDebugger::slotStepCmd()
{
	if (start())
		fakeInput("step", TRUE);
	if (inspector != 0L)
		inspector->refreshVariables();
}

void XsldbgDebugger::slotContinueCmd()
{
	if (start())
			/*this can take a while so don't wait for xsldbg to finish */
			fakeInput("continue", FALSE);
	if (inspector != 0L)
		inspector->refreshVariables();

}

void XsldbgDebugger::slotRunCmd()
{
	if (start())
			/*this can take a while so don't wait for xsldbg to finish */
			fakeInput("run", FALSE);

	if (inspector != 0L)
		inspector->refresh();
}


void XsldbgDebugger::slotWalkSpeed(int speed)
{
		if ((speed >= 0) && (speed <= 9)){
			if (start()){
				if (optionsGetIntOption(OPTIONS_WALK_SPEED) == WALKSPEED_STOP){
					 // start walking at speed requested
					QString msg("walk ");
					msg.append(QString::number(speed));
					fakeInput(msg, TRUE);
				} else
					// This will take effect imediately
					optionsSetIntOption(OPTIONS_WALK_SPEED, speed);
			}
		}else
			qDebug("Invalid walk speed %d", speed);
}

void XsldbgDebugger::slotWalkCmd()
{
	if  (walkDialog == 0L )
		walkDialog = new XsldbgWalkSpeedImpl (this);

	if (walkDialog != 0L)
		/* if the user changes the speed the dialog will call back slotWalkSpeed(int)  */
		walkDialog->show();
}

void XsldbgDebugger::slotWalkStopCmd()
{
    xsldbgStop = 1;
}

void XsldbgDebugger::slotTraceCmd()
{
	if (start())
			/*this can take a while so don't wait for xsldbg to finish */
			fakeInput("trace", FALSE);
}

void XsldbgDebugger::slotBreakCmd(QString fileName, int lineNumber)
{
  if (outputFileActive == TRUE){
    QMessageBox::information(0L, i18n("Operation Failed"),
			     i18n("Can't set/edit breakpoints on the output file."),
			     QMessageBox::Ok);
    return ;
  }

	QString msg("break -l \"");
	msg.append(fixLocalPaths(fileName)).append("\" ").append(QString::number(lineNumber));

	if (start())
		fakeInput(msg, TRUE);

	if (inspector != 0L)
		inspector->refreshBreakpoints();
}

void XsldbgDebugger::slotBreakCmd(QString templateName, QString modeName)
{

  if (outputFileActive == TRUE){
    QMessageBox::information(0L, i18n("Operation Failed"),
			     i18n("Can't set/edit breakpoints on the output file."),
			     QMessageBox::Ok);
    return ;
  }
	QString msg("break \"");
	msg.append(templateName).append("\" \"").append(modeName).append("\"");
	if (start())
		fakeInput(msg, TRUE);

	if (inspector != 0L)
		inspector->refreshBreakpoints();
}

void XsldbgDebugger::slotEnableCmd(QString fileName, int lineNumber)
{

  if (outputFileActive == TRUE){
    QMessageBox::information(0L, i18n("Operation Failed"),
			     i18n("Can't set/edit breakpoints on the output file."),
			     QMessageBox::Ok);
    return ;
  }

  QString msg("enable -l \"");
	msg.append(fixLocalPaths(fileName)).append("\" ").append(QString::number(lineNumber));
	if (start())
		fakeInput(msg, TRUE);

	if (inspector != 0L)
		inspector->refreshBreakpoints();
}


void XsldbgDebugger::slotEnableCmd(int id)
{

  if (outputFileActive == TRUE){
    QMessageBox::information(0L, i18n("Operation Failed"),
			     i18n("Can't set/edit breakpoints on the output file."),
			     QMessageBox::Ok);
    return ;
  }

  QString msg("enable ");
	msg.append(QString::number(id));
	if (start())
		fakeInput(msg, TRUE);

	if (inspector != 0L)
		inspector->refreshBreakpoints();
}


void XsldbgDebugger::slotDeleteCmd(QString fileName, int lineNumber)
{

  if (outputFileActive == TRUE){
    QMessageBox::information(0L, i18n("Operation Failed"),
			     i18n("Can't set/edit breakpoints on the output file."),
			     QMessageBox::Ok);
    return ;
  }

	QString msg("delete -l \"");
	msg.append(fixLocalPaths(fileName)).append("\" ").append(QString::number(lineNumber));
	if (start())
		fakeInput(msg, TRUE);
	if (inspector != 0L)
		inspector->refreshBreakpoints();
}


void XsldbgDebugger::slotDeleteCmd(int id)
{

  if (outputFileActive == TRUE){
    QMessageBox::information(0L, i18n("Operation Failed"),
			     i18n("Can't set/edit breakpoints on the output file."),
			     QMessageBox::Ok);
    return ;
  }
	QString msg("delete ");
	msg.append(QString::number(id));
	if (start())
		fakeInput(msg, TRUE);
	if (inspector != 0L)
		inspector->refreshBreakpoints();
}


void XsldbgDebugger::slotSourceCmd()
{
  if (start()){
    outputFileActive = FALSE;
    fakeInput("source", TRUE);
  }
}


void XsldbgDebugger::slotDataCmd()
{
  if (start()){
    outputFileActive = FALSE;
    fakeInput("data", TRUE);
  }
}

void XsldbgDebugger::slotShowDocument()
{

  if (outputFileName().length() > 0){
        outputFileActive = TRUE;
	gotoLine(outputFileName(), 1);
  }
}


void XsldbgDebugger::slotExitCmd()
{
	/*	showMessage("\nExit command disabled in Quanta for the moment\n");*/
		stop();
}


void XsldbgDebugger::slotCatCmd(QString xPathExpression){
	QString msg("cat ");
	msg.append(xPathExpression);

	if (start())
		/*this can take a while so don't wait for xsldbg to finish */
		fakeInput(msg, FALSE);

}

void  XsldbgDebugger::slotCdCmd(QString xPathExpression){
	QString msg("cd ");
	msg.append(xPathExpression);

	if (start())
		fakeInput(msg, TRUE);
}


#include "xsldbgdebugger.moc"
