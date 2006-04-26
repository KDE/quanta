/***************************************************************************
    begin                : Fri Jul 22 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef SIMPLETREEVIEWITEM_H
#define SIMPLETREEVIEWITEM_H



#include <k3listview.h>
#include <kurl.h>

class K3ListView;
/**
	@author Andras Mantia <amantia@kde.org>
*/
class SimpleTreeViewItem : public K3ListViewItem
{
public:
  SimpleTreeViewItem(const KUrl& url, const QString& treePath, K3ListView* parent);
  SimpleTreeViewItem(const KUrl& url, const QString& treePath, K3ListViewItem* parent, K3ListViewItem *after);
  ~SimpleTreeViewItem();
  KUrl url() const {return m_url;}
  void setURL(const KUrl& url) {m_url = url;}
  QString treePath() const {return m_treePath;}
  void setTreePath(const QString& treePath) {m_treePath = treePath;}
  QString key(int column, bool ascending) const;
  /**
  makes compare independent from locale
  */
  int compare( Q3ListViewItem *i, int col, bool ascending ) const;

private:
  void init(const KUrl& url, const QString& treePath);

  KUrl m_url;
  QString m_treePath;
};

#endif
