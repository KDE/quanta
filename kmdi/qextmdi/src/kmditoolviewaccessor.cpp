//----------------------------------------------------------------------------
//    filename             : kmditoolviewaccessor.h
//----------------------------------------------------------------------------
//    Project              : KDE MDI extension
//
//    begin                : 08/2003       by Joseph Wenninger (jowenn@kde.org)
//    changes              : ---
//    patches              : ---
//
//    copyright            : (C) 2003 by Joseph Wenninger (jowenn@kde.org)
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


#include "kmditoolviewaccessor.h"
#include "kmditoolviewaccessor.moc"
#include "kmditoolviewaccessor_p.h"
#if 0
# include "kmdiguiclient.h"
#endif
#include "kmdimainfrm.h"
#include <kmdidockwidget.h>
#include <kdebug.h>


KMdiToolViewAccessor::KMdiToolViewAccessor( KMdiMainFrm *parent , QWidget *widgetToWrap):QObject(parent) {
	mdiMainFrm=parent;
	d=new KMdiToolViewAccessorPrivate();
	if (widgetToWrap->inherits("KDockWidget")) {
		d->widgetContainer=dynamic_cast<KDockWidget*>(widgetToWrap);
		d->widget=d->widgetContainer->getWidget();
	} else {
		d->widget=widgetToWrap;
		d->widgetContainer= parent->createDockWidget( widgetToWrap->name(),
                                              (widgetToWrap->icon()?(*(widgetToWrap->icon())):QPixmap()),
                                              0L,  // parent
                                              widgetToWrap->caption(),
                                              widgetToWrap->caption() );
		d->widgetContainer->setWidget(widgetToWrap);
	}
	d->widget->installEventFilter(this);
}

KMdiToolViewAccessor::KMdiToolViewAccessor( KMdiMainFrm *parent) {
	mdiMainFrm=parent;
	d=new KMdiToolViewAccessorPrivate();
}

KMdiToolViewAccessor::~KMdiToolViewAccessor() {
	delete d;
}

QWidget *KMdiToolViewAccessor::wrapperWidget() {
	if (!d->widgetContainer) {
		d->widgetContainer=mdiMainFrm->createDockWidget( "KMdiToolViewAccessor::null",QPixmap());
		connect(d->widgetContainer,SIGNAL(widgetSet(QWidget*)),this,SLOT(setWidgetToWrap(QWidget*)));
	}
	return d->widgetContainer;
}

QWidget *KMdiToolViewAccessor::wrappedWidget() {
	return d->widget;
}


void KMdiToolViewAccessor::setWidgetToWrap(QWidget *widgetToWrap) {
	Q_ASSERT(!(d->widget));
	Q_ASSERT(!widgetToWrap->inherits("KDockWidget"));
	disconnect(d->widgetContainer,SIGNAL(widgetSet(QWidget*)),this,SLOT(setWidgetToWrap(QWidget*)));
	delete d->widget;
        d->widget=widgetToWrap;
	KDockWidget *tmp=d->widgetContainer;
	if (!tmp) {
		tmp=mdiMainFrm->createDockWidget( widgetToWrap->name(),
			widgetToWrap->icon()?(*(widgetToWrap->icon())):QPixmap(),
			0L,  // parent
                        widgetToWrap->caption(),
                        widgetToWrap->caption() );

		d->widgetContainer= tmp;
	} else {
		tmp->setCaption(widgetToWrap->caption());
		tmp->setTabPageLabel(widgetToWrap->caption());
		tmp->setPixmap(widgetToWrap->icon()?(*(widgetToWrap->icon())):QPixmap());
		tmp->setName(widgetToWrap->name());
	}
	tmp->setWidget(widgetToWrap);
	mdiMainFrm->m_pToolViews.insert(widgetToWrap,this);
#if 0	
	if (mdiMainFrm->m_mdiGUIClient)
		mdiMainFrm->m_mdiGUIClient->addToolView(this);
#endif		
	d->widget->installEventFilter(this);
}


bool KMdiToolViewAccessor::eventFilter(QObject *o, QEvent *e) {
	if (e->type()==QEvent::IconChange) {
		d->widgetContainer->setPixmap(d->widget->icon()?(*d->widget->icon()):QPixmap());
	}
	return false;
}

void KMdiToolViewAccessor::show(KDockWidget::DockPosition pos, QWidget* pTargetWnd ,int percent) {
  Q_ASSERT(d->widgetContainer);
  if (!d->widgetContainer) return;
  if (pos == KDockWidget::DockNone) {
     d->widgetContainer->setEnableDocking(KDockWidget::DockNone);
     d->widgetContainer->reparent(mdiMainFrm, Qt::WType_TopLevel | Qt::WType_Dialog, QPoint(0,0), true); //pToolView->isVisible());
   }
   else {   // add (and dock) the toolview as DockWidget view

      KDockWidget *pCover=d->widgetContainer;

      KDockWidget* pTargetDock = 0L;
        // Should we dock to ourself?
      bool DockToOurself = FALSE;
      if(mdiMainFrm->m_pDockbaseAreaOfDocumentViews)
      {
        if (pTargetWnd == mdiMainFrm->m_pDockbaseAreaOfDocumentViews->getWidget()) DockToOurself = TRUE;
      }
      // this is not inheriting QWidget*, its plain impossible that this condition is true
      //if (pTargetWnd == this) DockToOurself = TRUE;
      if (DockToOurself) pTargetDock = mdiMainFrm->m_pDockbaseAreaOfDocumentViews;
      else if(pTargetWnd != 0L) {
         pTargetDock = mdiMainFrm->dockManager->findWidgetParentDock( pTargetWnd);
         if (!pTargetDock) {
            if (pTargetWnd->parentWidget()) {
               pTargetDock = mdiMainFrm->dockManager->findWidgetParentDock( pTargetWnd->parentWidget());
            }
         }
      }
      if (pTargetDock) {
	      if (mdiMainFrm->m_managedDockPositionMode && mdiMainFrm->m_pMdi) {
			KDockWidget *dw1=pTargetDock->findNearestDockWidget(pos);
                        if (dw1)
                        pCover->manualDock(dw1,KDockWidget::DockCenter,percent);
                        else
                        pCover->manualDock ( pTargetDock, pos, 20 );
	
	      }
      else
      	pCover->manualDock( pTargetDock, pos, percent);
//check      pCover->show();
      }
   }

}

void KMdiToolViewAccessor::hide() {
	Q_ASSERT(d->widgetContainer);
	if (!d->widgetContainer) return;
	d->widgetContainer->undock();
}
