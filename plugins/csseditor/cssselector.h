/***************************************************************************
                          cssselector.h  -  description
                             -------------------
    begin                : mer ago 6 2003
    copyright            : (C) 2003 by gulmini luciano
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

#ifndef CSSSELECTOR_H
#define CSSSELECTOR_H


#include "cssselectors.h"
class QListViewItem;
class QStringList;
/**
  *@author gulmini luciano
  */

class CSSSelector : public CSSSelectorS {
     Q_OBJECT
  private:
     QListViewItem *currentItem;
     QListView *currentListView;
     //QString sourceFileName;
     QString header,
             footer;
            
  public: 
	  CSSSelector(QWidget *parent=0, const char* name=0);
	  ~CSSSelector();
  public slots:
    void openCSSEditor(QListViewItem *);
    
    void addTag();
    void addClass();
    void addID();
    void addPseudo();
    void addAtRules();
    void removeAll();
    void removeSelected();
    void setCurrentItem(QListViewItem* i) { currentItem = i; }
    void setCurrentListView(QWidget*);
    void loadExistingStyleSection(/*QMap<QString,QString> m*/QString);
    
    //void setSourceFileName(const QString& n) { sourceFileName = n; }
    
    void setHeader(const QString& h) { header = h; }
    void setFooter(const QString& f) { footer = f; }
    QString generateStyleSection();
    //QString getSourceFileName() const { return sourceFileName; }
  
   };

#endif
 
