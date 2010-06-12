/***************************************************************************
*   Copyright (C) 2005 by Jens Herden                                     *
*   jens@kdewebdev.org                                                    *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef HTMLPART_H
#define HTMLPART_H

#include <qwidget.h>
#include <QString>

//kdevelop includes
#include <khtml_part.h>


class HTMLPreviewPart;

class HTMLPart: public KHTMLPart
{
  Q_OBJECT
public:
  explicit HTMLPart(HTMLPreviewPart *part);
  ~HTMLPart();

protected slots:

  virtual void slotDuplicate();
  virtual void slotOpenInNewWindow(const KUrl &url);

private:
  HTMLPreviewPart *m_part;
};


#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
