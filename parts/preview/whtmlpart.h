/***************************************************************************
                          whtmlpart.h  -  description
                             -------------------
    begin                : Fri Aug 18 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2002 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WHTMLPART_H
#define WHTMLPART_H

#include <khtml_part.h>
#include <qstrlist.h>

/**
  *@author Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
  */

class WHTMLPart : public KHTMLPart  {
   Q_OBJECT
public:
  WHTMLPart(QWidget *parent, const char *name=0);
  ~WHTMLPart();

  bool backEnable();
  bool forwardEnable();

public slots:

/** back(), forward() - navigation, using history.
 */
  void forward();
  void back();

  void addToHistory( QString url );
  virtual bool eventFilter(QObject *watched, QEvent *e);

signals:
  void updateStatus( bool back, bool forward );
  void previewHasFocus(bool focus);

protected:
  virtual void urlSelected( const QString &url, int button, int state, const QString &_target, KParts::URLArgs args = KParts::URLArgs());
  virtual KParts::ReadOnlyPart *createPart( QWidget *parentWidget, const char *widgetName,
                                            QObject *parent, const char *name,
                                            const QString &mimetype, QString &serviceName,
                                            QStringList &serviceTypes, const QStringList &params);

private:
  QStrList history;
  unsigned int hpos;
};

#endif
