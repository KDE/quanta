/***************************************************************************
                          csseditor.h  -  description
                             -------------------
    begin                : mer lug 23 11:20:17 CEST 2003
    copyright            : (C) |YEAR| by si2003    email                : simone@localhost
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CSSEDITOR_H
#define CSSEDITOR_H

#include <qwidget.h>
#include "csseditors.h"
#include <qstringlist.h>
#include <qdom.h>
#include <qmap.h>
#include <qlistview.h>
#include <qsignal.h>

class QListViewItem;
class propertySetter;
class KHTMLPart;
class QListViewItem;
class QDomNodeList;
class QVariant;
class QMyHighlighter;


class myCheckListItem : public QCheckListItem
{
  private:
    QSignal *m_sig;
    unsigned int m_checkedChildren;

  public :
    myCheckListItem(QCheckListItem * parent, const QString & text);
    myCheckListItem(QListView * parent, const QString & text);
    ~myCheckListItem();
    void connect( QObject *receiver, const char *member );
    void addCheckedChild();

  protected :
    virtual void activate();
    virtual void stateChange (bool);
};

/** CSSEditor is the base class of the project */
class CSSEditor : public CSSEditorS
{
  Q_OBJECT
  private:
    QMyHighlighter *m_myhi;
    propertySetter *m_ps;
    myCheckListItem *m_currentProp;
    KHTMLPart *m_previewer;
    QDomDocument m_doc;
    QMap<QString,QString> m_properties;
    QString m_selectorName,
                 m_initialProperties,
                 m_Selectors,
                 m_Header,
                 m_Footer,
                 m_InlineStyleContent;
                        
            //sourceFileName;   
   
    void Connect();
    void appendSub(QDomNodeList, myCheckListItem *);
    void buildListView(QDomNodeList, QListView *);
    void updateDisplay();    
    void activatePreview();
    void setCurrentPropOn(const QString& s);
    void addProperty(const QString& property, const QString& value) { m_properties[property] = value; }
    void addAndSetPropertyOn(const QString& property, const QString& value);

  private slots:
    void checkProperty(const QString&);   
    void removeProperty(const QVariant&); 
    void updatePreview();
    void setMiniEditors(QListViewItem*);
    
  public:
    
    CSSEditor(QWidget* parent=0, const char *name=0){};
    CSSEditor( QListViewItem * i, QWidget* parent=0, const char *name=0);
    CSSEditor( QString s, QWidget* parent=0, const char *name=0);
    ~CSSEditor(); 
    void initialize();
    void setSelectors( const QString& s) { m_Selectors = s; }
    void setHeader( const QString& s) { m_Header = s; }
    void setFooter( const QString& s) { m_Footer = s;}
    void setInlineStyleContent( const QString& s){ m_InlineStyleContent = s; }
    void setForInitialPreview(const QString& s) { } 
    QString generateProperties();
    // void setSourceFileName(const QString& n) { sourceFileName = n; }
    void hidePreviewer();

  signals:
    void signalUpdatePreview();
};

#endif
