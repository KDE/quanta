/***************************************************************************
                          xsldbgdebugger.h  -  description
                             -------------------
    begin                : Tue Jan 1 2002
    copyright            : (C) 2002 by keith Isdale
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

#ifndef XSLDBGDEBUGGER_H
#define XSLDBGDEBUGGER_H

#include <qevent.h>
#include "libxsldbg/xsldbgmsg.h"   /* needed for XsldbgMessageEnum */
#include "libqtnotfier/xsldbgdebuggerbase.h"

#include "xsldbginspector.h"


/**
  *@author Keith Isdale
  */

class QStringList;

class XsldbgWalkSpeedImpl;

class XsldbgDebugger : public XsldbgDebuggerBase {
  Q_OBJECT

public:
	XsldbgDebugger();
	~XsldbgDebugger();

	/** */
	void setInspector(XsldbgInspector *inspector);

	/** Get the name of source file from xsldbg*/
	QString sourceFileName();

	/** Get the name of data file from xsldbg*/
	QString dataFileName();

	/** Get the name of output file from xsldbg*/
	QString outputFileName();

	/** Tell quanta to move its cursor to file and line number supplied */
	void gotoLine(QString fileName, int lineNo, bool breakpoint = false);

	/** Set a xsldbg option */
	void setOption(const char *name, bool value);

	bool event(QEvent *e);
	void timerEvent(QTimerEvent *e);

	void setOutputFileActive(bool b) {outputFileActive = b;};
private:
	/** */
	bool outputFileActive;

public slots:
	/** start xsldbg */
	bool start();

	/** stop xsldbg */
	bool stop();

	/** send text to xsldbg, if wait is true then wait for the reply */
	void fakeInput(QString text, bool wait);

	/** Display configure dialog */
	void slotConfigure();

	/** Config window closed */
	void slotConfigClosed();

  /** Step to next instruction */
  void slotStepCmd();

  /** Continue to next break point */
  void slotContinueCmd();

  /** Restart xsldbg with selected xsl/xml file */
  void slotRunCmd();

  /** Walk throught shtylesheet at a fixed speed
  	 @param speed : Must be a value of 0  to 9, where 1 mean fast, 9 means slow, and 0 means stop
  */
  void slotWalkSpeed(int speed);

  /** Present a dialog for use to choose walk speed */
  void slotWalkCmd();

  /** Stop walking imediately */
  void slotWalkStopCmd();

  /**
  	Start xslbg's trace mode, where output will be sent to to message window.
  		Can only be be stopped by using "exit" tool button and killing xsldbg */
  void slotTraceCmd();

	/** Add breakpoint at given file and line number */
	void slotBreakCmd(QString fileName, int lineNumber);

	/** Add breakpoint at given template name*/
	void slotBreakCmd(QString templateName, QString modeName);

	/** Toggle the enabling of break point at given file and line number */
	void slotEnableCmd(QString fileName, int lineNumber);

	/** Enable break point for given breakpoint id */
	void slotEnableCmd(int id);

	/** Delete break point at given file and line number */
	void slotDeleteCmd(QString fileName, int lineNumber);

	/** Delete break point for given breakpoint id */
	void slotDeleteCmd(int id);

	/** Switch to view of XSL source */
	void slotSourceCmd();

	/* switch to the output document  */
	void slotShowDocument();

	/** Switch to view of XML data */
	void slotDataCmd();

  /** Stop xsldg, must use start after this command  */
  void slotExitCmd();

  /** Print the result of evaluating xPathExpression to the message window */
  void slotCatCmd(QString xPathExpression);

  /** Move to line specified by xPathExpression */
  void slotCdCmd(QString xPathExpression);

signals: // Signals

private:
	bool initialized;

	 /* xsldbg config and inspection dialog */
	XsldbgInspector *inspector;

	/**Walk speed dialog  */
	XsldbgWalkSpeedImpl *walkDialog;
	bool readMsg;
	bool procMsg;

	/* copy text to output only if no furher output us requested */
	QString updateText;
	XsldbgMessageEnum lastType;
	int updateTimerID;
	bool addMsg;

	QStringList commandQue;

};

#endif
