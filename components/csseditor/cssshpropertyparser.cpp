/***************************************************************************
 *   Copyright (C) 2003 by Gulmini Luciano                                          *
 *   gulmini.luciano@student.unife.it                                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/
#include "cssshpropertyparser.h"
#include <qregexp.h>
//#include <kdebug.h>

CSSSHPropertyParser::CSSSHPropertyParser(const QString& s){
  QStringList l1,
                     l2=QStringList::split(",",s);

  for ( QStringList::Iterator it = l2.begin(); it != l2.end(); ++it ) {
    QString temp;
    temp=removeBeginningWhiteSpaces((*it));
    temp=removeEndingWhiteSpaces(temp);
    l1.append(temp);
  }

  m_propertyToParse = l1.join(",");// we eliminte blanks before after a comma in things like "something" , something    , serif
}

CSSSHPropertyParser::~CSSSHPropertyParser(){}

QString CSSSHPropertyParser::removeEndingWhiteSpaces(const QString& s){
  int index = s.length()-1;
  while(s[index] == ' ' )  index--;
  return s.left(index+1);
}

QString CSSSHPropertyParser::removeBeginningWhiteSpaces(const QString& s){
  int index = 0;
  while(s[index] == ' ' ) index++;
  return s.right(s.length()-index);
}

QString CSSSHPropertyParser::extractFunctionList(){
  QRegExp functionListPattern("\\s*([a-zA-Z0-9_]*\\([\\W\\w]*\\))\\s*");
  functionListPattern.search(m_propertyToParse);
  return functionListPattern.cap(1);
}

QString CSSSHPropertyParser::extractQuotedStringList(){
  QString temp;
  bool stop = false;
  unsigned int i=0;
  while(!stop && i<m_propertyToParse.length() ){
    if( m_propertyToParse[i] == ' ' ){
      if( ( temp.contains("\"") + temp.contains("\'") )%2 == 0 ) stop = true;
      else temp += m_propertyToParse[i];
    }
    else temp += m_propertyToParse[i];
    i++;
  }
  return temp;
}

QString CSSSHPropertyParser::extractURIList(){//extract things like url('...') or url("..") or url("..."), url(.....
    //kdDebug(24000) << "\n\n\nextractURIList()\n\n\n";
  QRegExp URIListPattern("\\s*(url\\([\\W\\w]*\\))\\s*");
  URIListPattern.search(m_propertyToParse);
  return URIListPattern.cap(1);
}

QStringList CSSSHPropertyParser::parse(){
  QStringList tokenList;
  bool stop = false;
  m_propertyToParse = removeBeginningWhiteSpaces(m_propertyToParse);

  while(!stop){
    QString temp;
    for(unsigned int i=0;i<m_propertyToParse.length()  ;i++){
      if(m_propertyToParse[i] == ' ') break;// tokens are delimited by a blank
      temp+=m_propertyToParse[i];
    }

    if(temp.contains("url(") !=0 ){
      QString foundURIList = extractURIList();
      m_propertyToParse = removeBeginningWhiteSpaces(m_propertyToParse.remove(foundURIList));
      tokenList.append(foundURIList);
    }
    else
    if(temp.contains("(")!=0){
      QString foundFunctionList = extractFunctionList();
      m_propertyToParse = removeBeginningWhiteSpaces(m_propertyToParse.remove(foundFunctionList));
      tokenList.append(foundFunctionList);
    }
    else
    if(temp.contains("'")!=0 || temp.contains("\"")!=0 || temp.contains(",")!=0){
      QString foundQuotedStringList = extractQuotedStringList();
      m_propertyToParse = removeBeginningWhiteSpaces(m_propertyToParse.remove(foundQuotedStringList));
      tokenList.append(foundQuotedStringList);
    }
    else
    if(temp.contains("/")!=0){ //treat the presence of line-height in font shorthand form
      m_propertyToParse = removeBeginningWhiteSpaces(m_propertyToParse.remove(temp));
      tokenList.append(temp.section("/",0,0));
      tokenList.append("/"+temp.section("/",1,1));
    }
    else {
      tokenList.append(temp);
      int tempPos = m_propertyToParse.find(temp);
      m_propertyToParse = removeBeginningWhiteSpaces(m_propertyToParse.remove(tempPos,temp.length()));
    }
    if( m_propertyToParse.isEmpty() ) stop = true;
  }
  return tokenList;
}




