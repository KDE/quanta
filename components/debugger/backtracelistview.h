/***************************************************************************
                           Backtracelistview.h
                         ------------------------
    begin                : 2005-07-31
    copyright            : (C) 2005 Linus McCabe

 ***************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/


#ifndef BACKTRACELISTVIEW_H
#define BACKTRACELISTVIEW_H

#include <klistview.h>
#include <kpopupmenu.h>
#include <qptrlist.h>

class BacktraceListview;


enum BacktraceType
{
  File = 0,
  Eval
};


class BacktraceListviewItem : public KListViewItem
{
  private:
    BacktraceType m_type;
    QString m_filename;
    QString m_func;
    long m_line;
    long m_level;

  public:
    BacktraceListviewItem();
    BacktraceListviewItem(BacktraceListview* view);

    // Type
    BacktraceType type() const { return m_type; }
    void setType(BacktraceType type) { m_type = type; }

    // Filename
    QString filename() const { return m_filename; }
    void setFilename(const QString &filename) { m_filename = filename; }
    
    // Function
    QString func() const { return m_func; }
    void setFunc(const QString &func) { m_func = func; }
    
    // Line
    long line() const { return m_line; }
    void setLine(long line) { m_line= line; }
    
    // Level
    long level() const { return m_level; }
    void setLevel(long level) { m_level = level; }

};

class BacktraceListview : public KListView
{
  Q_OBJECT

  public:

    BacktraceListview(QWidget *parent = 0, const char *name = 0);
    ~BacktraceListview();

    void backtraceShow(int level, BacktraceType type, const QString& filename, long line, const QString& func);

    void clear();

  public slots:
    void slotBacktraceDoubleClick(QListViewItem *item, const QPoint &point, int column);

  private:
    void keyPressEvent(QKeyEvent *e);
    void jumpHistory(QListViewItem *item);

};

#endif
