//----------------------------------------------------------------------------
//    filename             : kmdichildfrmcaption.cpp
//----------------------------------------------------------------------------
//    Project              : KDE MDI extension
//
//    begin                : 07/1999       by Szymon Stefanek as part of kvirc
//                                         (an IRC application)
//    changes              : 09/1999       by Falk Brettschneider to create an
//                           - 06/2000     stand-alone Qt extension set of
//                                         classes and a Qt-based library
//                           2000-2003     maintained by the KDevelop project
//
//    copyright            : (C) 1999-2003 by Szymon Stefanek (stefanek@tin.it)
//                                         and
//                                         Falk Brettschneider
//    email                :  falkbr@kdevelop.org (Falk Brettschneider)
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU Library General Public License as
//    published by the Free Software Foundation; either version 2 of the
//    License, or (at your option) any later version.
//
//----------------------------------------------------------------------------

#include <qpainter.h>
#include <qapplication.h>
#include <qcursor.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>

#include "kmdidefines.h"
#include "kmdichildfrm.h"
#include "kmdichildarea.h"
#include "kmdimainfrm.h"
#include "kmdichildfrmcaption.h"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////
// Class   : KMdiChildFrmCaption
// Purpose : An MDI label that draws the title
//
//
//////////////////////////////////////////////////////////////////////////////

//============== KMdiChildFrmCaption =============//

KMdiChildFrmCaption::KMdiChildFrmCaption(KMdiChildFrm *parent)
:QWidget(parent, "kmdi_childfrmcaption")
{
   m_szCaption    = tr("Unnamed");
   m_bActive      = false;
   m_pParent      = parent;
   setBackgroundMode(NoBackground);
   setFocusPolicy(NoFocus);
   m_bChildInDrag = false;
}

//============== ~KMdiChildFrmCaption =============//

KMdiChildFrmCaption::~KMdiChildFrmCaption()
{
}

//============= mousePressEvent ==============//

void KMdiChildFrmCaption::mousePressEvent(QMouseEvent *e)
{
   if ( e->button() == LeftButton) {
      setMouseTracking(false);
      if (KMdiMainFrm::frameDecorOfAttachedViews() != KMdi::Win95Look) {
         QApplication::setOverrideCursor(Qt::sizeAllCursor,true);
      }
      m_pParent->m_bDragging = true;
      m_offset = mapToParent( e->pos());
   }
   else if ( e->button() == RightButton) {
      m_pParent->systemMenu()->popup( mapToGlobal( e->pos()));
   }
}

//============= mouseReleaseEvent ============//

void KMdiChildFrmCaption::mouseReleaseEvent(QMouseEvent *e)
{
   if ( e->button() == LeftButton) {
      if (KMdiMainFrm::frameDecorOfAttachedViews() != KMdi::Win95Look)
         QApplication::restoreOverrideCursor();
      releaseMouse();
      if(m_pParent->m_bDragging) {
         m_pParent->m_bDragging = false;
         if (m_bChildInDrag) {
            //notify child view
            KMdiChildFrmDragEndEvent ue(e);
            if( m_pParent->m_pClient != 0L) {
               QApplication::sendEvent( m_pParent->m_pClient, &ue);
            }
            m_bChildInDrag = false;
         }
      }
   }
}

//============== mouseMoveEvent =============//
void KMdiChildFrmCaption::mouseMoveEvent(QMouseEvent *e)
{
   if ( !m_pParent->m_bDragging ) {
      return;
   }

   if (!m_bChildInDrag) {
      //notify child view
      KMdiChildFrmDragBeginEvent ue(e);
      if( m_pParent->m_pClient != 0L) {
         QApplication::sendEvent( m_pParent->m_pClient, &ue);
      }
      m_bChildInDrag = true;
   }

   QPoint relMousePosInChildArea = m_pParent->m_pManager->mapFromGlobal( e->globalPos() );

   // mouse out of child area? stop child frame dragging
   if ( !m_pParent->m_pManager->rect().contains( relMousePosInChildArea)) {
      if ( relMousePosInChildArea.x() < 0)
         relMousePosInChildArea.rx() = 0;
      if ( relMousePosInChildArea.y() < 0)
         relMousePosInChildArea.ry() = 0;
      if ( relMousePosInChildArea.x() > m_pParent->m_pManager->width())
         relMousePosInChildArea.rx() = m_pParent->m_pManager->width();
      if ( relMousePosInChildArea.y() > m_pParent->m_pManager->height())
         relMousePosInChildArea.ry() = m_pParent->m_pManager->height();
   }
   QPoint mousePosInChildArea = relMousePosInChildArea - m_offset;

   // set new child frame position
   parentWidget()->move( mousePosInChildArea);
}

//=============== setActive ===============//

void KMdiChildFrmCaption::setActive(bool bActive)
{
   if( m_bActive == bActive)
      return;

   //    Ensure the icon's pixmap has the correct bg color
   m_pParent->m_pWinIcon->setBackgroundColor(bActive
   ? m_pParent->m_pManager->m_captionActiveBackColor
   : m_pParent->m_pManager->m_captionInactiveBackColor);
   m_pParent->m_pUnixIcon->setBackgroundColor(bActive
   ? m_pParent->m_pManager->m_captionActiveBackColor
   : m_pParent->m_pManager->m_captionInactiveBackColor);

   m_bActive = bActive;
   repaint( false);
}

//=============== setCaption ===============//

void KMdiChildFrmCaption::setCaption(const QString& text)
{
   m_szCaption = text;
   repaint(false);
}

//============== heightHint ===============//

int KMdiChildFrmCaption::heightHint()
{
   int hght=m_pParent->m_pManager->m_captionFontLineSpacing+3;
   if (KMdiMainFrm::frameDecorOfAttachedViews() == KMdi::Win95Look) {
      if(hght<18)hght=18;
   }
   else if (KMdiMainFrm::frameDecorOfAttachedViews() == KMdi::KDE1Look) {
      if(hght<20)hght=20;
   }
   else if (KMdiMainFrm::frameDecorOfAttachedViews() == KMdi::KDELook) {
      if(hght<16)hght=16;
   }
   else {   // kde2laptop look
      hght -= 4;
      if(hght<14)hght=14;
   }
   return hght;
}

//=============== paintEvent ==============//

void KMdiChildFrmCaption::paintEvent(QPaintEvent *)
{
   QPainter p(this);
   QRect r=rect();
   p.setFont(m_pParent->m_pManager->m_captionFont);
   if(m_bActive){
      p.fillRect(r,m_pParent->m_pManager->m_captionActiveBackColor);
      p.setPen(m_pParent->m_pManager->m_captionActiveForeColor);
   } else {
      p.fillRect(r,m_pParent->m_pManager->m_captionInactiveBackColor);
      p.setPen(m_pParent->m_pManager->m_captionInactiveForeColor);
   }
   //Shift the text after the icon
   if (KMdiMainFrm::frameDecorOfAttachedViews() == KMdi::Win95Look)
      r.setLeft(r.left()+m_pParent->icon()->width()+3);
   else if (KMdiMainFrm::frameDecorOfAttachedViews() == KMdi::KDE1Look)
      r.setLeft(r.left()+22);
   else if (KMdiMainFrm::frameDecorOfAttachedViews() == KMdi::KDELook)
      r.setLeft(r.left()+m_pParent->icon()->width()+3);
   else  // kde2laptop look
      r.setLeft(r.left()+30);

   int captionWidthForText = width() - 4*m_pParent->m_pClose->width() - m_pParent->icon()->width() - 5;
   QString text = abbreviateText( m_szCaption, captionWidthForText);
   p.drawText( r, AlignVCenter|AlignLeft|SingleLine, text);

}

//=============== abbreviateText ===============//

QString KMdiChildFrmCaption::abbreviateText(QString origStr, int maxWidth)
{
   QFontMetrics fm = fontMetrics();
   int actualWidth = fm.width( origStr);

   int realLetterCount = origStr.length();
   int newLetterCount;
   if (actualWidth != 0) {
      newLetterCount = (maxWidth * realLetterCount) / actualWidth;
   }
   else {
      newLetterCount = realLetterCount; // should be 0 anyway
   }
   int w = maxWidth+1;
   QString s = origStr;
   if (newLetterCount <= 0) {
      s = "";
   }
   while((w > maxWidth) && (newLetterCount >= 1)) {
      if( newLetterCount < realLetterCount) {
         if(newLetterCount > 3)
            s = origStr.left( newLetterCount/2) + "..." + origStr.right( newLetterCount/2);
         else {
            if(newLetterCount > 1)
               s = origStr.left( newLetterCount) + "..";
            else
               s = origStr.left(1);
         }
      }
      QFontMetrics fm = fontMetrics();
      w = fm.width(s);
      newLetterCount--;
   }
   return s;
}

//============= mouseDoubleClickEvent ===========//

void KMdiChildFrmCaption::mouseDoubleClickEvent(QMouseEvent *)
{
   m_pParent->maximizePressed();
}

//============= slot_moveViaSystemMenu ===========//

void KMdiChildFrmCaption::slot_moveViaSystemMenu()
{
   setMouseTracking(true);
   grabMouse();
   if (KMdiMainFrm::frameDecorOfAttachedViews() != KMdi::Win95Look)
      QApplication::setOverrideCursor(Qt::sizeAllCursor,true);
   m_pParent->m_bDragging = true;
   m_offset = mapFromGlobal( QCursor::pos());
}

#ifndef NO_INCLUDE_MOCFILES
#include "kmdichildfrmcaption.moc"
#endif
