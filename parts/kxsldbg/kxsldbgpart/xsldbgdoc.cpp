/* 
  Handle our documents  
  */ 
 
#include "xsldbgdoc.h" 
 
XsldbgDoc::XsldbgDoc() 
{ 
  fileName = QString(); 
  text = QString(); 
  row = 0; 
  column = 0; 
} 
 
 
XsldbgDoc::XsldbgDoc(QString &fileName, QString &text) 
{ 
  this->fileName = fileName; 
  this->text = text; 
  row = 0; 
  column  = 0; 
} 
 
 
XsldbgDoc::~XsldbgDoc() 
{ 
  /* Empty */ 
} 
 
 
QString XsldbgDoc::getText()  
{ 
  return text;  
} 
 
  
QString XsldbgDoc::getFileName()  
{  
  return fileName;  
} 
 
void XsldbgDoc::setRow(int row)  
{  
  this->row = row;   
} 
 
int XsldbgDoc::getRow()  
{  
  return row;  
} 
 
void XsldbgDoc::setColumn(int column)  
{  
  this->column = column;   
} 
 
 
int XsldbgDoc::getColumn()  
{  
  return column;  
} 
