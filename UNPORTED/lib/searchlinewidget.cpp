/***************************************************************************
    begin                : Tue Jul 19 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#include "searchlinewidget.h"
#include "filterableitemif.h"

#include <QWidget>
#include <QTreeWidgetItem>

/**
 
  small helper class to find the right listviewitems 
 
 */
class SearchLine : public KTreeWidgetSearchLine
{
public:
  SearchLine(SearchLineWidget *parent, QTreeWidget *treeWidget = 0)
      : KTreeWidgetSearchLine(parent, treeWidget), m_widget(parent)
  {};

  ~SearchLine() {};

protected:

  bool itemMatches (const QTreeWidgetItem *item, const QString &s) const
  {
    const FilterableItemIf * fItem = dynamic_cast<const FilterableItemIf *>(item);
    if (fItem)
      return fItem->filter(s);

    return KTreeWidgetSearchLine::itemMatches(item, s);
  };

private:
  SearchLineWidget * m_widget;
};


SearchLineWidget::SearchLineWidget(QWidget *parent, QTreeWidget * treeWidget)
    : KTreeWidgetSearchLineWidget(parent, treeWidget)
{}


SearchLineWidget::~SearchLineWidget()
{}


KTreeWidgetSearchLine * SearchLineWidget::createSearchLine(QTreeWidget * treeWidget) const
{
  return new SearchLine(const_cast<SearchLineWidget*>(this), treeWidget);
}

#include "searchlinewidget.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
