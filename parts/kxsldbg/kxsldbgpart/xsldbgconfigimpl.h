/***************************************************************************
                          xsldbgconfigimpl.h  -  description
                             -------------------
    begin                : Fri Jan 4 2002
    copyright            : (C) 2002 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***********************************************************************************
 *   This program is free software; you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or         *
 *   (at your option) any later version.                                   							*
 *                                                                         										*
 ************************************************************************************/

#ifndef XSLDBGCONFIGIMPL_H
#define XSLDBGCONFIGIMPL_H

/**
  *@author Keith Isdale
  */

#include "xsldbgconfig.h"
#include "xsldbgdialogbase.h"

#include <qptrlist.h>


class LibxsltParam : public QObject
{
	public:
	LibxsltParam(QString name, QString value);
	~LibxsltParam();

	QString getName();
	void setName(QString name);
	QString getValue();
	void setValue(QString value);
	bool isValid();

private:
	QString _name;
	QString _value;
};


class XsldbgDebugger;

class XsldbgConfigImpl : public XsldbgConfig, public XsldbgDialogBase {
  Q_OBJECT

public:
	XsldbgConfigImpl(XsldbgDebugger *debugger, QWidget *parent=0, const char *name=0);
	~XsldbgConfigImpl();

	LibxsltParam *getParam(int paramNumber);
	LibxsltParam *getParam(QString name);
	int getParamCount();
	void addParam(QString name, QString value);
	void deleteParam(QString name);
	void repaintParam();

	QString getSourceFile();
	QString getDataFile();
	QString getOutputFile();
	bool debugEnabled() {return debug; };
	bool catalogsEnabled() {return catalogs; };
	bool htmlEnabled() {return html; };
	bool docbookEnabled() {return docbook; };
	bool nonetEnabled() {return nonet; };
	bool novalidEnabled() {return novalid; };
	bool nooutEnabled() {return noout; };
	bool timingEnabled() {return timing; };
	bool profileEnabled() {return profile; };


	/** return true if all data ok */
	bool isValid(QString &errorMsg);

	/** Update changes to xsldbg*/
	void update();

	/** refresh data from source */
	void refresh();


public slots:

	/** Set xsl source file*/
	void slotSourceFile(QString);

	/** Set xml data file*/
	void slotDataFile(QString);

	/** Set file name for the default output of transformed data*/
	void slotOutputFile(QString);

	/**Choose the XSL source file */
	void slotChooseSourceFile();

	/**Choose the XML data file */
	void slotChooseDataFile();


	/**Choose the output file */
	void slotChooseOutputFile();

	/** Configure dialog has request that a param be added */
	void slotAddParam();

	/** Configure dialog has request that a param be deleted */
	void slotDeleteParam();

	/** Configure dialog has request that a next param be shown */
	void slotNextParam();

	/** Configure dialog has request that a prev param be shown */
	void slotPrevParam();

	/** Apply the changes */
	void slotApply();

	/** Ignore any changes and hide dialog  */
	void slotCancel();

	/** Process notification of add parameter to view, First parameter
			is QString::null to indicate start of parameter list notfication */
	void slotProcParameterItem(QString  name, QString value);


private:
	int paramIndex;
	QPtrList<LibxsltParam> paramList;

	XsldbgDebugger *debugger;

	bool catalogs;
	bool debug;
	bool html;
	bool docbook;
	bool nonet;
	bool novalid;
	bool noout;
	bool timing;
	bool profile;
};
#endif
