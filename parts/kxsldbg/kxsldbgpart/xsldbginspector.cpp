/**
   File : xsldbginspector.cpp
   Author : Keith Isdale
   Date : 30th March 2002
   Description : Dialog to inspect stylesheet using xsldbg. Based on
                  file created by uic
 */

#include "xsldbginspector.h"

#include <klocale.h>

#include <qvariant.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qmime.h>
#include <qdragobject.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qiconset.h>

#include "xsldbgdebugger.h"
#include "xsldbgbreakpointsimpl.h"
#include "xsldbglocalvariablesimpl.h"
#include "xsldbgglobalvariablesimpl.h"
#include "xsldbgcallstackimpl.h"
#include "xsldbgtemplatesimpl.h"
#include "xsldbgsourcesimpl.h"
#include "xsldbgentitiesimpl.h"

static QPixmap uic_load_pixmap_XsldbgInspector( const QString &name )
{
    const QMimeSource *m = QMimeSourceFactory::defaultFactory()->data( name );
    if ( !m )
	return QPixmap();
    QPixmap pix;
    QImageDrag::decode( m, pix );
    return pix;
}
/*
 *  Constructs a XsldbgInspector which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
XsldbgInspector::XsldbgInspector( XsldbgDebugger *debugger, QWidget* parent,
				  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    CHECK_PTR(debugger);
    this->debugger = debugger;
    breakpointWidget = 0L;
    localWidget = 0L;
    globalWidget = 0L;
    callStackWidget = 0L;
    templateWidget = 0L;
    sourceWidget = 0L;
    entityWidget = 0L;
    if ( !name )
	setName( "XsldbgInspector" );
    resize( 597, 364 );
    setCaption( i18n( "Xsldbg Inspector" ) );
    setSizeGripEnabled( TRUE );
    XsldbgInspectorLayout = new QGridLayout( this, 1, 1, 11, 6,
					     "XsldbgInspectorLayout");

    tabWidget = new QTabWidget( this, "tabWidget" );
    CHECK_PTR( tabWidget );
    breakpointWidget = new XsldbgBreakpointsImpl( debugger, tabWidget );
    CHECK_PTR( breakpointWidget );
    tabWidget->insertTab( breakpointWidget, i18n( "Breakpoints" ) );

    localWidget = new  XsldbgLocalVariablesImpl( debugger, tabWidget );
    CHECK_PTR( localWidget );
    tabWidget->insertTab( localWidget,
	  QIconSet( uic_load_pixmap_XsldbgInspector( "xsldbg_source.png" ) ),
	  i18n( "Locals" ) );

    globalWidget = new  XsldbgGlobalVariablesImpl( debugger, tabWidget );
    CHECK_PTR( globalWidget );
    tabWidget->insertTab( globalWidget,
	  QIconSet( uic_load_pixmap_XsldbgInspector( "xsldbg_source.png" ) ),
          i18n( "Globals" ));

    callStackWidget = new  XsldbgCallStackImpl( debugger, tabWidget );
    CHECK_PTR( callStackWidget );
    tabWidget->insertTab( callStackWidget,
	  QIconSet( uic_load_pixmap_XsldbgInspector( "xsldbg_source.png" ) ),
	  i18n( "CallStack" ));

    templateWidget = new  XsldbgTemplatesImpl( debugger, tabWidget );
    CHECK_PTR( templateWidget );
    tabWidget->insertTab( templateWidget,
	  QIconSet( uic_load_pixmap_XsldbgInspector( "xsldbg_source.png" ) ),
          i18n( "Templates" ));

    sourceWidget = new  XsldbgSourcesImpl( debugger, tabWidget );
    CHECK_PTR( sourceWidget );
    tabWidget->insertTab( sourceWidget,
	  QIconSet( uic_load_pixmap_XsldbgInspector( "xsldbg_source.png" ) ),
	  i18n( "Sources" ));

    entityWidget = new  XsldbgEntitiesImpl( debugger, tabWidget );
    CHECK_PTR( entityWidget );
    tabWidget->insertTab( entityWidget,
	  QIconSet( uic_load_pixmap_XsldbgInspector( "xsldbg_data.png" ) ),
	  i18n( "Entities" ));

    XsldbgInspectorLayout->addWidget( tabWidget, 0, 1 );
    Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1");

    buttonHelp = new QPushButton( this, "buttonHelp" );
    buttonHelp->setText( i18n( "Help" ) );
    buttonHelp->setAccel( 4144 );
    buttonHelp->setAutoDefault( TRUE );
    Layout1->addWidget( buttonHelp );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setText( i18n( "&OK" ) );
    buttonOk->setAccel( 0 );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonApply = new QPushButton( this, "buttonApply" );
    buttonApply->setText( i18n( "&Apply" ) );
    QToolTip::add(buttonApply, i18n("Apply changes to xsldbg after restarting execution"));
    buttonApply->setAccel( 0 );
    buttonApply->setAutoDefault( TRUE );
    buttonApply->setDefault( TRUE );
    Layout1->addWidget( buttonApply );

    buttonRefresh = new QPushButton( this, "buttonRefresh" );
    buttonRefresh->setText( i18n( "&Refresh" ) );
    QToolTip::add(buttonRefresh, i18n("Refresh values in inspectors from xsldbg"));
    buttonRefresh->setAccel( 0 );
    buttonRefresh->setAutoDefault( TRUE );
    buttonRefresh->setDefault( TRUE );
    Layout1->addWidget( buttonRefresh );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( i18n( "&Cancel" ) );
    buttonCancel->setAccel( 0 );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );

    XsldbgInspectorLayout->addMultiCellLayout( Layout1, 1, 1, 0, 1 );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonApply, SIGNAL ( clicked() ), this, SLOT ( update() ) );
    connect( buttonRefresh, SIGNAL ( clicked() ), this, SLOT ( refresh() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

    hide();
}

/*
 *  Destroys the object and frees any allocated resources
 */
XsldbgInspector::~XsldbgInspector()
{
  debugger = 0L;
  // no need to delete child widgets, Qt does it all for us
}


void XsldbgInspector::accept()
{
  QDialog::accept();
}

void XsldbgInspector::reject()
{
  QDialog::reject();
}


void XsldbgInspector::refresh()
{

  refreshBreakpoints();
  refreshVariables();

  if ( templateWidget != 0L)
    templateWidget->refresh();

  if ( sourceWidget != 0L)
    sourceWidget->refresh();

  if ( entityWidget != 0L)
    entityWidget->refresh();

}

 void XsldbgInspector::refreshBreakpoints()
{
    if ( breakpointWidget != 0L )
      breakpointWidget->refresh();
}


void XsldbgInspector::refreshVariables()
{
    if ( localWidget != 0L )
      localWidget->refresh();

    if (globalWidget != 0L)
      globalWidget->refresh();

    if (callStackWidget != 0L)
      callStackWidget->refresh();
}

#include "xsldbginspector.moc"
