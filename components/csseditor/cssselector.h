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

#include <qmap.h>
#include <qpair.h>
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
                 m_callingFrom,
                 m_fileToPreview;
    QMap<QString, QPair<QString,unsigned int> > m_currentStylesheetStructure;
    unsigned int m_orderNumber;
    bool m_stopProcessingStylesheet;

    void Connect();

  public:
    CSSSelector(QWidget *parent=0, const char* name=0);
    ~CSSSelector();
    void loadCSSContent(const QString& s);
    void setHeader(const QString& h) { m_header = h; }
    void setFooter(const QString& f) { m_footer = f; }
    void enableApplyToFile();
    void setCallingFrom(const QString& cf){ m_callingFrom = cf ;}
    void setFileToPreview(const QString& s){ m_fileToPreview=s;}
    bool errorOnProcessingStylesheet() const { return m_stopProcessingStylesheet; }
    QString generateFormattedStyleSection();

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
    void setStylesheetProcessing(const QString&);
};

#endif

