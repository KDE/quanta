/*************************************************************************** 
                          xsldbgconfigimpl.cpp  -  description 
                             ------------------- 
    begin                : Fri Jan 4 2002 
    copyright            : (C) 2002 by Keith Isdale 
    email                :  k_isdale@tpg.com.au 
 ***************************************************************************/ 
 
/*********************************************************************************** 
 *                                                                         										* 
 *   This program is free software; you can redistribute it and/or modify       * 
 *   it under the terms of the GNU General Public License as published by  * 
 *   the Free Software Foundation; either version 2 of the License, or         * 
 *   (at your option) any later version.                                   							* 
 *                                                                         										* 
 ************************************************************************************/ 
 
#include <qdialog.h> 
#include <qlineedit.h> 
#include <qcheckbox.h> 
#include <qmessagebox.h> 
 
#include "xsldbgconfigimpl.h" 
#include "xsldbgdebugger.h" 
 
 
LibxsltParam::LibxsltParam(QString name, QString value) 
	: QObject(0L, 0L) 
{ 
	_name = name; 
 _value = value; 
}; 
 
 
LibxsltParam::~LibxsltParam() 
{ 
} 
 
 
QString LibxsltParam::getName() 
{ 
 return _name; 
}; 
 
void LibxsltParam::setName(QString name) 
{ 
	_name = name; 
} 
 
 
QString LibxsltParam::getValue() 
{ 
	return _value; 
} 
 
 
void LibxsltParam::setValue(QString value) 
{ 
	_value = value; 
} 
 
	 
bool LibxsltParam::isValid() 
{ 
	bool result = true; 
	if ((_name.length() > 0) && ( _value.length() == 0)) 
		result = false; 
 
	return result; 
}  		 
 
 
XsldbgConfigImpl::XsldbgConfigImpl(XsldbgDebugger *debugger, 
	QWidget *parent /*=0*/, const char *name /*=0*/) 
		:  XsldbgConfig(parent, name) 
{	 
	this->debugger = debugger; 
	connect(debugger, SIGNAL(parameterItem(QString /* name*/, QString /* value */)), 
		this, SLOT(slotProcParameterItem(QString /* name*/, QString /* value */))); 
	paramIndex= 0; 
	catalogs = false; 
	debug = false; 
	html = false; 
	nonet = false; 
	docbook = false; 
	 
	show(); 
	refresh(); 
} 
 
XsldbgConfigImpl::~XsldbgConfigImpl() 
{ 
  debugger = 0L;   
} 
 
 
QString XsldbgConfigImpl::getSourceFile() 
{ 
  if (xslSourceEdit != 0L) 
    return xslSourceEdit->text(); 
  else 
    return QString(); 
} 
 
 
QString XsldbgConfigImpl::getDataFile() 
{ 
  if (xmlDataEdit != 0L) 
    return xmlDataEdit->text(); 
  else 
    return QString(); 
} 
 
 
QString XsldbgConfigImpl::getOutputFile() 
{ 
  if (outputFileEdit != 0L) 
    return outputFileEdit->text(); 
  else 
    return QString(); 
} 
 
 
LibxsltParam *XsldbgConfigImpl::getParam(int paramNumber) 
{ 
	return paramList.at(paramNumber); 
} 
 
LibxsltParam *XsldbgConfigImpl::getParam(QString name) 
{ 
	LibxsltParam *param = 0L; 
	for (param = paramList.first(); param != 0L; param = paramList.next()) 
	{ 
		if (param->getName() == name) 
			break; 
  }		 
 
	return param; 
} 
 
 
int XsldbgConfigImpl::getParamCount() 
{ 
	return paramList.count(); 
} 
 
 
void XsldbgConfigImpl::addParam(QString name, QString value) 
{ 
	LibxsltParam *param; 
	if ((name.length() == 0) | (value.length() == 0))	 
		return; 
 
	param = getParam(name); 
	if (param == 0L) 
	{ 
		param = new LibxsltParam(name, value); 
		if (param != 0L) 
		  paramList.append(param);	 
 
  }else 
		param->setValue(value);	 
} 
 
 
void XsldbgConfigImpl::deleteParam(QString name) 
{ 
	bool isOk = false; 
	LibxsltParam *param; 
	if (name.length() == 0) 
		return; 
 
	param = getParam(name); 
	if (param != 0L) 
		isOk = paramList.remove(param); 
 
	if (isOk == false) 
	 qDebug(" Param %s dosn't exist", (char*)name.latin1()); 
	else 
	  qDebug("Deleted param %s", (char*)name.latin1()); 
} 
 
 
bool XsldbgConfigImpl::isValid(QString &errorMsg) 
{ 
  bool isOK = TRUE; 
  errorMsg = ""; 
  if (xslSourceEdit->text().isEmpty()) 
    errorMsg.append( "\t\"XSL source\" \n"); 
  if (xmlDataEdit->text().isEmpty()) 
    errorMsg.append("\t\"XML data\" \n"); 
  if (outputFileEdit->text().isEmpty()) 
    errorMsg.append("\t\"Output file\" \n");  
  if (!errorMsg.isEmpty()){ 
    errorMsg.prepend("Missing values for \n"); 
    isOK = FALSE; 
  }else if (( xslSourceEdit->text() == outputFileEdit->text())  || 
	    (xmlDataEdit->text() == outputFileEdit->text())){ 
    errorMsg.append("Output file is the same as either XSL Source or "  
		    "XML Data file\n"); 
    isOK = FALSE; 
  } 
 
  // make it a warning when parameters are invalid 
  LibxsltParam *param; 
  QString emptyParams = ""; 
  for (param = paramList.first(); param != 0L; param = paramList.next()) 
    { 
      if (!param->isValid()){ 
	if (emptyParams.isEmpty()) 
	  emptyParams = param->getName(); 
	else 
	  emptyParams.append(", "). append(param->getName()); 
      } 
    } 
         
  if (!emptyParams.isEmpty()){ 
    errorMsg.append("The following libxslt parameters are empty\n\t"); 
    errorMsg.append(emptyParams); 
  } 
		 
  return isOK;	 
} 
 
 
/*we previously said that isValid() == true so we must commit our changes */ 
void XsldbgConfigImpl::update() 
{ 
	QString msg; 
	if (debugger == 0L) 
	  return; 
 
	/* update source, data, output file name if needed */ 
	if (debugger->sourceFileName() != xslSourceEdit->text()) 
		slotSourceFile(xslSourceEdit->text()); 
 
	if (debugger->dataFileName() != xmlDataEdit->text()) 
		slotDataFile(xmlDataEdit->text()); 
 
	if (debugger->outputFileName() != outputFileEdit->text()) 
		slotOutputFile(outputFileEdit->text()); 
		 
	/* ensure entered param are updated */ 
	slotAddParam();	 
	 
	if (debugger->start() == false) 
		return ; /* User has killed xsldbg and we can't restart it */ 
	 
	/* always update the libxslt parameters */ 
	debugger->fakeInput("delparam", true); 
	 
 
	 
 	LibxsltParam *param; 
 	for (param = paramList.first(); param != 0L; param = paramList.next()) 
 	{ 
 		if (debugger->start() == false) 
 			return ; /* User has killed xsldbg and we can't restart it */ 
 		if (param->isValid()){ 
 			msg = "addparam ";					 
 			msg.append(param->getName()).append(" ").append(param->getValue());	 
 			debugger->fakeInput(msg, true);							 
 		} 
 	}		 
 	 
 	/* now set the xsldbg options*/ 	 
 	if (catalogsChkBox->isChecked() != catalogs){ 
 		catalogs =  catalogsChkBox->isChecked(); 
		debugger->setOption("catalogs", catalogs); 
	 } 
	 if (debugChkBox->isChecked() != debug){ 
	 	debug=  debugChkBox->isChecked(); 
 		debugger->setOption("debug", debug);	 
 	 } 
 	 if (htmlChkBox->isChecked() != html){ 
  	 	html =  htmlChkBox->isChecked(); 	 
   	debugger->setOption("html", html); 
 	 } 
 	 if (docbookChkBox->isChecked() != docbook){ 
 	 	docbook	 =  docbookChkBox->isChecked(); 
 		debugger->setOption("docbook", docbook); 
 	 } 
 	 if (nonetChkBox->isChecked() != nonet){ 
 	 	nonet	 =  nonetChkBox->isChecked(); 
 	 	debugger->setOption("nonet", nonet); 
 	 } 
 	 if (novalidChkBox->isChecked() != novalid){ 
 	 	novalid	 =  novalidChkBox->isChecked(); 
 	 	debugger->setOption("novalid", novalid); 
 	 } 
 	 if (nooutChkBox->isChecked() != noout){ 
 	 	noout	 =  nooutChkBox->isChecked(); 
 	 	debugger->setOption("noout", noout); 
 	 } 
 	 if (timingChkBox->isChecked() != timing){ 
 	 	timing	 =  timingChkBox->isChecked(); 
 	  debugger->setOption("timing", timing); 
 	 } 
 	 if (profileChkBox->isChecked() != profile){ 
 	 	profile	 =  profileChkBox->isChecked(); 
 	 	debugger->setOption("profile", profile); 
 	 }	 
 	 
	debugger->setOption("preferhtml", true); 	  	 
	debugger->setOption("utf8input", true); 	  	 
        debugger->slotRunCmd(); 
        hide(); 
} 
 
	 
void XsldbgConfigImpl::refresh() 
{ 
  /* 
	if (debugger->start() == false) 
		return ; 
 
	qDebug("XsldbgConfigImpl::refresh");	 
	xslSourceEdit->setText(debugger->sourceFileName()); 
	xmlDataEdit->setText(debugger->dataFileName());	 
	outputFileEdit->setText(debugger->outputFileName()); 
  */ 
	/* we'll get the list of parameters via paramItem(..) in this class */ 
  /*	debugger->fakeInput("showparam", true); 		 
   */ 
 
} 
 
 
void XsldbgConfigImpl::slotSourceFile(QString xslFile) 
{ 
	if (debugger->start() == false) 
		return ; /* User has killed xsldbg and we can't restart it */ 
	 
	qDebug("Source file change"); 
	QString msg("source "); 
	msg.append(xslFile); 
	debugger->fakeInput(msg, true); 
 
} 
 
void XsldbgConfigImpl::slotDataFile(QString xmlFile) 
{ 
	if (debugger->start() == false) 
		return ; /* User has killed xsldbg and we can't restart it */ 
	 
	qDebug("Data file change"); 
	QString msg("data "); 
	msg.append(xmlFile); 
	debugger->fakeInput(msg, true); 
 
} 
 
void XsldbgConfigImpl::slotOutputFile(QString outputFile) 
{ 
	if (debugger->start() == false) 
 			return ; /* User has killed xsldbg and we can't restart it */ 
	 
	qDebug("Data output file change"); 
	QString msg("output "); 
	msg.append(outputFile); 
	debugger->fakeInput(msg, true); 
 
} 
 
 
void XsldbgConfigImpl::slotChooseSourceFile() 
{ 
	  QString fileName; 
		fileName = QFileDialog::getOpenFileName(QString::null, "*.xsl; *.XSL; *.Xsl \n *.*", 0L, 
										"Choose XSL source to debug"); 
	 
	if ((fileName != QString::null) && (fileName.length() > 0)) 
		xslSourceEdit->setText(fileName); 
} 
 
 
void XsldbgConfigImpl::slotChooseDataFile() 
{ 
	QString fileName; 
	fileName = QFileDialog::getOpenFileName(QString::null, "*.xml; *.XML; *.Xml \n*.docbook \n *.html;*.HTML; *.htm ; *HTM \n *.*", 0L, 
									"Choose XML data  to debug"); 
	if ((fileName != QString::null) && (fileName.length() > 0)) 
		xmlDataEdit->setText(fileName); 
} 
	 
 
void XsldbgConfigImpl::slotChooseOutputFile() 
{ 
	QString fileName; 
	fileName = QFileDialog::getSaveFileName(QString::null, "*.xml; *.XML; *.Xml \n*.docbook \n *.txt; *.TXT \n *.htm;*.HTM;*.htm;*.HTML \n*.*", 0L, 
									"Choose output file for XSL transformation"); 
	if ((fileName != QString::null) && (fileName.length() > 0)) 
		outputFileEdit->setText(fileName); 
} 
 
 
void XsldbgConfigImpl::repaintParam() 
{ 
	if (paramIndex < getParamCount()){ 
		LibxsltParam *param = getParam(paramIndex); 
		parameterNameEdit->setText(param->getName()); 
		parameterValueEdit->setText(param->getValue());	 
	}else{ 
		parameterNameEdit->setText(""); 
		parameterValueEdit->setText("");	 
	} 
} 
 
void XsldbgConfigImpl::slotAddParam() 
{ 
	addParam(parameterNameEdit->text(), parameterValueEdit->text()); 
	if (paramIndex < getParamCount()) 
		paramIndex++; 
		 
  	repaintParam();	 
} 
 
void XsldbgConfigImpl::slotDeleteParam() 
{ 
   deleteParam(parameterNameEdit->text()); 
   repaintParam(); 
} 
 
 
void XsldbgConfigImpl::slotNextParam() 
{ 
	addParam(parameterNameEdit->text(), parameterValueEdit->text()); 
	if (paramIndex < getParamCount()) 
		paramIndex++; 
		 
   repaintParam(); 
} 
 
void XsldbgConfigImpl::slotPrevParam() 
{ 
	addParam(parameterNameEdit->text(), parameterValueEdit->text()); 
	if (paramIndex > 0) 
		paramIndex--; 
			 
   repaintParam(); 
} 
 
void XsldbgConfigImpl::slotProcParameterItem(QString name, QString value) 
{ 
	if (name == QString::null){ 
		paramList.clear(); 
		paramIndex = 0;		 
		parameterNameEdit->setText(""); 
		parameterValueEdit->setText("");	 
	}else{ 
		addParam(name, value); 
		if(paramList.count() == 1){ 
			parameterNameEdit->setText(name); 
			parameterValueEdit->setText(value);	 
		} 
	} 
} 
 
 
 
void XsldbgConfigImpl::slotApply() 
{ 
   
  QString msg; 
  if (isValid(msg)){ 
    if (!msg.isEmpty()) 
      QMessageBox::information(this, "Suspect configuration",  
			     msg, QMessageBox::Ok); 
    update(); 
  }else{ 
    QMessageBox::information(this, "Incomplete or invalid configuration",  
			     msg, QMessageBox::Ok);     
  } 
     
} 
 
 
void XsldbgConfigImpl::slotCancel() 
{ 
  hide(); 
} 
