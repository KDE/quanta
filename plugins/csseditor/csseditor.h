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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kapplication.h>
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
class KTempFile;



class myCheckListItem : public QCheckListItem
{
  private:
    QSignal *sig;
    unsigned int checkedChildren;

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
    KTempFile *testFile;
    QMyHighlighter *myhi;
    propertySetter *ps;
    myCheckListItem *currentProp;
    KHTMLPart *previewer;
    QDomDocument doc,
                            shortHandDefDoc ;
    QMap<QString,QString> properties;
    QString selectorName,
                initialProperties,
	        Selectors,
	        Header,
	        Footer,
                InlineStyleContent;
    
    QString initialPreviewText;            
            //sourceFileName;
    QStringList SHFormList;    
	    


    void Connect();
    void appendSub(QDomNodeList, myCheckListItem *);
    void buildListView(QDomNodeList, QListView *);

  public:
    CSSEditor(QWidget* parent=0, const char *name=0);
    CSSEditor( QListViewItem * i, QWidget* parent=0, const char *name=0);
    CSSEditor( QString s, QWidget* parent=0, const char *name=0);
    ~CSSEditor();
    void addProperty(const QString& property, const QString& value) { properties[property] = value; }
    void addAndSetPropertyOn(const QString& property, const QString& value);
    void setForInitialPreview(const QString& s) { initialPreviewText = s; }
    
    void initShortHandForm();
    //QString fromShortHandForm(QString,QString){}
    //QString toShortHandForm(QString s){}
    
    void setCurrentPropOn(const QString& s);

  public slots:
    void removeProperty(const QVariant&);
    void setMiniEditors(QListViewItem*);
    void checkProperty(const QString&);
    void updatePreview();
    void activatePreview();
    void initialize();
   // void setSourceFileName(const QString& n) { sourceFileName = n; }

    void setSelectors( const QString& s) { Selectors = s; }
    void setHeader( const QString& s) { Header = s; }
    void setFooter( const QString& s) { Footer = s;}
    void setInlineStyleContent( const QString&);

    QString generateProperties();

  signals:

    void signalUpdatePreview();
};

#endif
