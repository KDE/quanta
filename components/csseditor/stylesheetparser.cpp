/***************************************************************************
                          stylesheetparser.cpp  -  description
                             -------------------
    begin                : gio ago 19 2004
    copyright            : (C) 2004 by gulmini luciano
    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "stylesheetparser.h"
#include <klocale.h>
#include <kdebug.h>

static const QString msg1(i18n("has not been closed")),
                     msg2(i18n("needs an opening parenthesis "));

stylesheetParser::stylesheetParser(const QString& s){ 
  m_styleSheet = s;
  m_styleSheet=m_styleSheet.right(m_styleSheet.length()-whiteSpaces(0));
  m_stopProcessing = false;
  m_orderNumber = 0; 
}
    
int stylesheetParser::whiteSpaces(int d){
  int ws=0;
  for( unsigned int i=d;i<m_styleSheet.length();i++){
    QString temp;
    if(m_styleSheet[i] == ' ' || m_styleSheet[i] == '\n'  || m_styleSheet[i] == '\t' ) 
      ws++;
    else break; 
  }
  return ws;
}

void stylesheetParser::parse(){
  if(!m_stopProcessing){
    if(!m_styleSheet.isEmpty()){
       if(m_styleSheet.startsWith("/*"))
          parseComment();
          else if(m_styleSheet.startsWith("@page"))
              parseAtRules1();
              else if(m_styleSheet.startsWith("@media"))
                        parseAtRules1();
                      else if(m_styleSheet.startsWith("@import"))
                                parseAtRules2();
                              else if(m_styleSheet.startsWith("@charset"))
                                        parseAtRules2();
                                      else parseSelector();
    }    
    else return;
  }  
}

void stylesheetParser::parseComment(){ 
  bool stopProcessingComment=false;
  unsigned int k;
  for(k=2;k<m_styleSheet.length()-1;k++){
    QString temp;
    temp.append(m_styleSheet[k]).append(m_styleSheet[k+1]);
    if(temp=="*/")  {
      k+=2;
      stopProcessingComment=true;
      break;
    }    
  }
  
  if(stopProcessingComment){
    int ws=whiteSpaces(k);
    QPair<QString,unsigned int> tmp(m_styleSheet.left(k+ws),++m_orderNumber);
    m_stylesheetStructure["/*"+QString::number(m_orderNumber,10)]=tmp;
    m_styleSheet=m_styleSheet.right(m_styleSheet.length()-k-ws);
    parse(); 
  }
  
   else {
      m_stopProcessing = true;
      emit errorOccurred(i18n("The comment") + " :\n" +m_styleSheet.mid(0,20) + "...\n "+ msg1);      
      return;
    }
}

unsigned int stylesheetParser::numberOfParenthesisInAParenthesisBlock(parenthesisKind p, const QString& b){
  QChar searchFor = '{';
  if (p == closed)
    searchFor = '}';
  int num = 0;
  int len = b.length();
  bool ignore = false;
  for (int i = 0; i < len; i++)
  {
    if (b[i] == '/' && (i + 1 < len) && b[i + 1] == '*')
      ignore = true;
    if (b[i] == '*' && (i + 1 < len) && b[i + 1] == '/')
      ignore = false;
    if (!ignore && b[i] == searchFor)
      num++;
  }
  return num;
}

int findParanthesis(const QString& string, const QChar &ch, int startPos = 0)
{
  int pos = -1;
  int len = string.length();
  bool ignore = false;
  for (int i = startPos; i < len; i++)
  {
    if (string[i] == '/' && (i + 1 < len) && string[i + 1] == '*')
      ignore = true;
    if (string[i] == '*' && (i + 1 < len) && string[i + 1] == '/')
      ignore = false;
    if (!ignore && string[i] == ch)
    {
      pos = i;
      break;
    }
  }
  return pos;
}

void stylesheetParser::parseSelector(){
  int closingParenthesisPos = findParanthesis(m_styleSheet, '}');
   if(closingParenthesisPos==-1) {
    m_stopProcessing = true;
    emit errorOccurred(i18n("The selector") + " :\n" +m_styleSheet.mid(0,20) + "...\n "+ msg1);      
    return;
  }
    
   QString temp(m_styleSheet.left(closingParenthesisPos+1));    
  
  if(numberOfParenthesisInAParenthesisBlock(closed,temp) < numberOfParenthesisInAParenthesisBlock(opened,temp)){
    m_stopProcessing = true;
    emit errorOccurred(i18n("The selector") + " :\n" +m_styleSheet.mid(0,20) + "...\n "+ msg1);      
    return;
  }  
  
  if(numberOfParenthesisInAParenthesisBlock(closed,temp) > numberOfParenthesisInAParenthesisBlock(opened,temp)){
    m_stopProcessing = true;
    emit errorOccurred(i18n("The selector") + " :\n" +m_styleSheet.mid(0,20) + "...\n "+ msg2);      
    return;
  }    
 
   int closingParentheses = 1,
        openingParentheses = 0;
  while(true){ 
    openingParentheses = numberOfParenthesisInAParenthesisBlock(closed,m_styleSheet.left(closingParenthesisPos+1)); //m_styleSheet.left(closingParenthesisPos+1).contains("{");
        
    if(openingParentheses==closingParentheses){
      QString selectorName=m_styleSheet.left(findParanthesis(m_styleSheet, '{')/*m_styleSheet.find("{")*/).stripWhiteSpace(),
                   selectorValue=m_styleSheet.mid(findParanthesis(m_styleSheet, '{')/*m_styleSheet.find("{")*/+1, closingParenthesisPos - m_styleSheet.find("{") -1);    
        
      selectorName.remove("\n").remove("\t");
      selectorValue.remove("\n").remove("\t");
      QPair<QString,unsigned int> tmp(selectorValue,++m_orderNumber);
            
      if (m_stylesheetStructure.contains(selectorName))
      {
        uint i = 2;
        QString s = selectorName + QString("-v%1").arg(i);
        while (m_stylesheetStructure.contains(s))
        {          
         i++;
         s = selectorName + QString("-v%1").arg(i);
        }
        selectorName = s;
      }
      m_stylesheetStructure[selectorName]=tmp; 
      break;
    } 
    else {
      closingParenthesisPos = findParanthesis(m_styleSheet, '{',closingParenthesisPos+1)/*m_styleSheet.find("}",closingParenthesisPos+1)*/;
      closingParentheses++;
    } 
  }

  int ws=whiteSpaces(closingParenthesisPos+1);
  m_styleSheet=m_styleSheet.right(m_styleSheet.length()-closingParenthesisPos-1-ws);
  parse(); 
}

void stylesheetParser::parseAtRules1(){
//TODO this needs to be fixed : in case the at rule is not properly closed the parser hangs
  if(m_styleSheet.find("{") == -1) {
    m_stopProcessing = true;
    emit errorOccurred(m_styleSheet.mid(0,20) + "...\n " + msg2);      
    return;
  }

  int closingParenthesisPos = m_styleSheet.find("}"),
       closingParentheses = 0;
      
  if(closingParenthesisPos==-1) return;
  else closingParentheses = 1;     

  int openingParentheses=0;
  while(true){
   openingParentheses = m_styleSheet.left(closingParenthesisPos+1).contains("{");

    if(openingParentheses==closingParentheses) 
      break;   
    else {
      closingParenthesisPos = m_styleSheet.find("}",closingParenthesisPos+1);
      if( closingParenthesisPos!= -1 )
        closingParentheses++;
      else {
        m_stopProcessing = true;
        emit errorOccurred(m_styleSheet.mid(0,20) + "...\n " + msg1);      
        return;
      }  
    } 
  }

  int ws=whiteSpaces(closingParenthesisPos+1);
  QPair<QString,unsigned int> tmp(m_styleSheet.left(closingParenthesisPos+1+ws),++m_orderNumber);
  m_stylesheetStructure["@rule"+QString::number(m_orderNumber,10)]=tmp;
  m_styleSheet=m_styleSheet.right(m_styleSheet.length()-closingParenthesisPos-1-ws);
  parse(); 
}

void stylesheetParser::parseAtRules2(){
//TODO this needs to be fixed : in case the at rule is not properly closed the parser hangs
  int semicolonPos = m_styleSheet.find(";");
      
  if(semicolonPos==-1) {
    m_stopProcessing = true;
    emit errorOccurred(m_styleSheet.mid(0,20) + "...\n " + msg1);      
    return;
  }
 
  int ws=whiteSpaces(semicolonPos+1);
  QPair<QString,unsigned int> tmp(m_styleSheet.left(semicolonPos+1+ws),++m_orderNumber);
  m_stylesheetStructure["@rule"+QString::number(m_orderNumber,10)]=tmp;
  m_styleSheet=m_styleSheet.right(m_styleSheet.length()-semicolonPos-1-ws);
  parse(); 
}

#include "stylesheetparser.moc"
