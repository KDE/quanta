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
  WHTMLPart(QWidget *parentWidget = 0, const char *widgetname = 0,
            QObject *parent = 0, const char *name = 0, GUIProfile prof = DefaultGUI );
  virtual ~WHTMLPart();

  /** Specify the URL where the preview was started and the
  content of this url as well.*/
  void setPreviewedURL(const KURL &url, const QString &text);
  bool backEnable();
  bool forwardEnable();
  virtual bool openURL(const KURL &url);

public slots:

/** back(), forward() - navigation, using history.
 */
  void forward();
  void back();

  void addToHistory(const QString &url);
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
  KURL m_previewedURL;
  QString m_previewedText;
  QStrList history;
  unsigned int hpos;
};

#endif
