/***************************************************************************
                          stylesheetparser.h  -  description
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

#ifndef STYLESHEETPARSER_H
#define STYLESHEETPARSER_H

#include <qmap.h>
#include <qpair.h>
#include <qobject.h>

/**
  *@author gulmini luciano
  */

class stylesheetParser : public QObject{
  Q_OBJECT
  private:
    
    enum parenthesisKind { opened, closed };
    QString m_styleSheet;
    bool m_stopProcessing;
    unsigned int m_orderNumber;
    QMap<QString, QPair<QString,unsigned int> > m_stylesheetStructure; 
    void parseComment();  
    void parseSelector();
    void parseAtRules1();
    void parseAtRules2();
    int whiteSpaces(int);
    unsigned int numberOfParenthesisInAParenthesisBlock(parenthesisKind p, const QString& b);
    
  public:
    stylesheetParser(const QString& s);
    ~stylesheetParser(){}
    void parse();

    QMap<QString, QPair<QString,unsigned int> > stylesheetStructure() { return m_stylesheetStructure; }
    unsigned int orderNumber() const {return m_orderNumber; }
    
  signals:
    void errorOccurred(const QString&);  
};  
  
#endif
 
