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
    QListViewItem *m_currentItem;
    QListView *m_currentListView;
    QString m_header,
                 m_footer,
                 m_initialPreviewText;  
    //QString m_sourceFileName;             
    bool m_hideEditorPreviewer; 
    void Connect();   
    
            
  public: 
    CSSSelector(QWidget *parent=0, const char* name=0);
    ~CSSSelector();
    
  public :
    void loadExistingStyleSection(QString);
    void loadCSSFileContent(QString s){ loadExistingStyleSection(s); }
    void setForInitialPreview(const QString& s) { m_initialPreviewText = s;}   
    void setHeader(const QString& h) { m_header = h; }
    void setFooter(const QString& f) { m_footer = f; }
    QString generateStyleSection();  
    QString generateFormattedStyleSection();         
    //void setSourceFileName(const QString& n) { m_sourceFileName = n; }
    //QString sourceFileName() const { return m_sourceFileName; }    
    void hideEditorPreviewer(bool b){ m_hideEditorPreviewer = b; }
    
  private slots: 
    void openCSSEditor(QListViewItem *);  
    void addTag();
    void addClass();
    void addID();
    void addPseudo();
    void removeAll();
    void removeSelected();     
    void setCurrentItem(QListViewItem* i) { m_currentItem = i; }
    void setCurrentListView(QWidget*);
    void setDTDTags(const QString&);
};

#endif
 
