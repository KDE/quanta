/***************************************************************************
                          whtmlpart.cpp  -  description
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
//kde includes
#include <khtmlview.h>
#include <kconfig.h>
#include <kdebug.h>
#include <khtml_settings.h>

//app includes
#include "whtmlpart.h"
#include "whtmlpart.moc"

WHTMLPart::WHTMLPart(QWidget *parentWidget, const char *widgetName,
            QObject *parent, const char *name, GUIProfile prof)
  : KHTMLPart(parentWidget, widgetName, parent, name, prof)
{
  //kdDebug(24000) << "WHTMLPart: " << parentWidget << " " << widgetName << " " << parent << " " << name << this << endl;
   hpos = 0;
   // get settings from konq.
   KConfig konqConfig("konquerorrc");

   konqConfig.setGroup("HTML Settings");

   const KHTMLSettings * set = settings();

   const_cast<KHTMLSettings*>(set)->init( &konqConfig, false );
   view()->installEventFilter(this);

//   setCharset( konqConfig.readEntry("DefaultEncoding") );
//   setEncoding( konqConfig.readEntry("DefaultEncoding") );
//   setStandardFont( konqConfig.readEntry("StandardFont") );
//   setFixedFont( konqConfig.readEntry("FixedFont") );
//   updateFontSize( konqConfig.readNumEntry("FontSize"));
}

WHTMLPart::~WHTMLPart()
{
}

void WHTMLPart::setPreviewedURL(const KURL &url, const QString& text)
{
  m_previewedText = text;
  m_previewedURL = url;
}

bool WHTMLPart::openURL(const KURL& url)
{
  if (url == m_previewedURL)
  {
    KURL previewURL = url;
    previewURL.setFileName("preview-" + url.fileName());
    return KHTMLPart::openURL(previewURL);
  } else
    return KHTMLPart::openURL(url);
}

void  WHTMLPart::urlSelected ( const QString &url, int button, int state, const QString &target, KParts::URLArgs args)
{
  KHTMLPart::urlSelected (url, button, state, target, args);
  KURL cURL = completeURL( url );
//  alternative not tested but used in kdevelop !
//  KURL cURL=KURL(baseURL(),url);
  if (target.isEmpty() || (target == "_self") || (target == "_top") || (target == "_blank") || (target == "_parent") )
    openURL( cURL ) ;
  addToHistory( cURL.url() );
}

void WHTMLPart::forward()
{
  if ( forwardEnable() ) {
    hpos++;
    openURL( KURL( history.at(hpos) ) );

    emit updateStatus( backEnable() , forwardEnable() );
  }
}


void WHTMLPart::back()
{
  if (backEnable())
  {
    hpos--;
    openURL(KURL(history.at(hpos)));

    emit updateStatus(backEnable(), forwardEnable());
  }
}


void WHTMLPart::addToHistory(const QString &url)
{

   if ( history.count() > 0 )
     while ( hpos < history.count()-1  )
         history.removeLast();

   if ( !history.isEmpty() ) hpos++;

   history.append(url);

   hpos = history.count()-1;

   emit updateStatus( backEnable() , forwardEnable() );

}


bool WHTMLPart::backEnable()
{
   return hpos > 0;
}


bool WHTMLPart::forwardEnable()
{
   return hpos < history.count()-1;
}

KParts::ReadOnlyPart *WHTMLPart::createPart( QWidget * parentWidget, const char *widgetName,
                                            QObject *parent, const char *name,
                                            const QString &, QString &,
                                            QStringList &, const QStringList &)
{
  //kdDebug(24000) << "Create WHTMLPart: " << parentWidget << " " << widgetName << " " << parent << " " << name << endl;
  return new WHTMLPart(parentWidget, widgetName, parent, name);
}

bool WHTMLPart::eventFilter(QObject *watched, QEvent *e)
{
  if (watched == view() && e->type() == QEvent::FocusOut)
    emit previewHasFocus(false);
  else
  if (watched == view() && e->type() == QEvent::FocusIn)
    emit previewHasFocus(true);
  return false;
}
