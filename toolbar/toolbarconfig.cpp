/****************************************************************************
** Form implementation generated from reading ui file 'toolbarconfig.ui'
**
** Created: Sat Oct 14 22:41:47 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "toolbarconfig.h"

#include <klocale.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const char* const image0_data[] = { 
"22 22 91 2",
"Qt c None",
".9 c #000000",
".7 c #000008",
".5 c #000408",
"#y c #001018",
"#x c #001420",
"#i c #001829",
".4 c #080008",
"#c c #080408",
".6 c #080410",
".8 c #080810",
".a c #080c18",
".H c #081018",
".d c #081420",
"#n c #081829",
"#k c #081c29",
"#b c #082031",
".# c #083c52",
".n c #08405a",
".t c #084462",
".h c #08506a",
".2 c #085d7b",
"#w c #086d8b",
".0 c #087994",
".l c #08b2cd",
".N c #08b6cd",
"#d c #103c5a",
".3 c #104c6a",
".1 c #10698b",
"#t c #106d8b",
".Q c #107194",
"#a c #107594",
".V c #10819c",
".G c #1081a4",
".s c #1089ac",
".m c #108dac",
".g c #1095b4",
".Z c #109dbd",
"#r c #10a5c5",
".C c #10b2cd",
".Y c #10b6cd",
".O c #10b6d5",
".D c #10cade",
".w c #10cede",
".F c #18b6cd",
".y c #18bacd",
".B c #18c2de",
".v c #18c6de",
"#s c #18cee6",
"#m c #18dade",
".r c #20dae6",
".x c #20dee6",
".R c #295073",
"#u c #295973",
".U c #296d8b",
"#v c #297194",
".M c #29c2de",
"## c #29d6e6",
".c c #31597b",
".X c #31c2d5",
".q c #31cade",
".u c #31d6e6",
".W c #39aec5",
".A c #39cade",
".P c #39e6ee",
"#q c #41b2cd",
"#h c #41e2e6",
".T c #4a7194",
".E c #4ae6ee",
".L c #52bad5",
".S c #627d9c",
"#p c #62bed5",
"#g c #62cade",
"#j c #6acee6",
".I c #73a5c5",
".z c #7b99bd",
".b c #7b9dbd",
".i c #7ba1bd",
".o c #83a5c5",
"#. c #83deee",
"#l c #83e6f6",
".e c #8baecd",
".K c #8bc2de",
"#f c #8be6f6",
".p c #94d6e6",
".k c #94d6ee",
"#e c #cdf2ff",
"#o c #d5f2ff",
".f c #def2ff",
".J c #e6f6ff",
".j c #ffffff",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQt.#.aQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQt.#.b.c.dQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQt.#.e.f.g.h.dQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQt.#.i.j.k.l.m.n.dQtQtQtQtQtQtQt",
"QtQtQtQtQtQt.#.o.j.p.q.r.l.s.t.dQtQtQtQtQtQt",
"QtQtQtQtQt.#.o.j.k.u.v.w.x.y.m.t.dQtQtQtQtQt",
"QtQtQtQt.#.z.j.p.A.B.l.C.D.E.F.G.n.HQtQtQtQt",
"QtQtQt.#.I.J.K.L.M.N.O.l.B.w.P.m.Q.t.aQtQtQt",
"QtQt.#.R.S.T.U.V.W.X.N.Y.D.Z.0.1.2.3.#.aQtQt",
"QtQt.4.5.6.7.8.9.k#..A.C###a#b.5#c.7.4.9QtQt",
"QtQtQtQtQtQtQt#d#e#f#g.Y#h#a#iQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQt.#.j#f#j.C.E#a#kQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQt#d.j#l#g.Y#m#a#nQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQt.##o#p#q#r#s#t#kQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQt#d#u#v#w.Q#t.3#xQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQt.5.d.d#n.d#n#y.6QtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt",
"QtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQtQt"};

static const char* const image1_data[] = { 
"22 22 64 1",
". c None",
"o c #000000",
"h c #080008",
"a c #083c52",
"U c #087594",
"1 c #0899b4",
"P c #08a1bd",
"z c #08b2cd",
"Z c #08bacd",
"s c #08bad5",
"0 c #08bed5",
"D c #08c2d5",
"# c #103c5a",
"2 c #104862",
"T c #105073",
"Q c #106d8b",
"n c #107594",
"G c #107994",
"B c #10799c",
"F c #10819c",
"6 c #1099b4",
"w c #10b2cd",
"5 c #10b6d5",
"7 c #10bad5",
"O c #10c2d5",
"y c #10c6de",
"S c #18617b",
"R c #186583",
"8 c #18bed5",
"E c #18cede",
"t c #18d2de",
"g c #2085a4",
"v c #20c2d5",
"N c #29bede",
"r c #29c6de",
"Y c #31c6de",
"4 c #39c6de",
"x c #39dade",
"A c #41dee6",
"K c #52aac5",
"b c #5aaecd",
"M c #5ac6de",
"J c #62b2cd",
"u c #73d2e6",
"I c #7bbad5",
"H c #7bbed5",
"q c #7bd2e6",
"m c #8bd6e6",
"9 c #8bdaee",
"k c #94d2e6",
"X c #94daee",
"j c #9cdaee",
"L c #9cdeee",
"l c #ace2ee",
"3 c #b4f2ff",
"C c #bde6f6",
"W c #c5eeff",
"V c #cdf2ff",
"c c #d5eaf6",
"f c #deeaff",
"i c #e6f6ff",
"e c #f6f6ff",
"d c #f6faff",
"p c #ffffff",
"......................",
"......................",
"......................",
".......#a#a#a#a.......",
".......abcdefgh.......",
".......#ijklmno.......",
".......apqrstnh.......",
".......#puvwxno.......",
".......apqyzABh.......",
"..a#a#a#CuDwEFooohoo..",
"..hGHIJKLMNzOPBQRSTo..",
"...hUViWXYZwz0Et12o...",
"....hG3u4s5zwDA62o....",
".....hU3D7zwDA12o.....",
"......hG3D7DA62o......",
".......hU38A12o.......",
"........hG962o........",
".........hU2o.........",
"..........ho..........",
"......................",
"......................",
"......................"};


/* 
 *  Constructs a ToolBarConfig which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ToolBarConfig::ToolBarConfig( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    QPixmap image0( ( const char** ) image0_data );
    QPixmap image1( ( const char** ) image1_data );
    if ( !name )
	setName( "ToolBarConfig" );
    resize( 450, 400 );
    setCaption( i18n( "MyDialog"  ) );
    setSizeGripEnabled( TRUE );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    GroupBox2 = new QGroupBox( this, "GroupBox2" );
    GroupBox2->setTitle( i18n( "ToolBars"  ) );
    GroupBox2->setColumnLayout(0, Qt::Vertical );
    GroupBox2->layout()->setSpacing( 0 );
    GroupBox2->layout()->setMargin( 0 );
    grid_2 = new QGridLayout( GroupBox2->layout() );
    grid_2->setAlignment( Qt::AlignTop );
    grid_2->setSpacing( 6 );
    grid_2->setMargin( 11 );

    listToolbar = new QListView( GroupBox2, "listToolbar" );
    listToolbar->addColumn( i18n( "Toolbar" ) );

    grid_2->addMultiCellWidget( listToolbar, 0, 4, 0, 0 );

    buttonToolbarUp = new QPushButton( GroupBox2, "buttonToolbarUp" );
    buttonToolbarUp->setText( i18n( ""  ) );
    buttonToolbarUp->setMaximumSize( QSize( 32, 32 ) );
    buttonToolbarUp->setEnabled( FALSE );
    buttonToolbarUp->setPixmap( image0 );

    grid_2->addWidget( buttonToolbarUp, 0, 1 );

    buttonToolbarDown = new QPushButton( GroupBox2, "buttonToolbarDown" );
    buttonToolbarDown->setText( i18n( ""  ) );
    buttonToolbarDown->setMaximumSize( QSize( 32, 32 ) );
    buttonToolbarDown->setEnabled( FALSE );
    buttonToolbarDown->setPixmap( image1 );

    grid_2->addWidget( buttonToolbarDown, 0, 2 );

    buttonToolBarRemove = new QPushButton( GroupBox2, "buttonToolBarRemove" );
    buttonToolBarRemove->setText( i18n( "Remove"  ) );

    grid_2->addMultiCellWidget( buttonToolBarRemove, 4, 4, 1, 2 );

    buttonToolbarEdit = new QPushButton( GroupBox2, "buttonToolbarEdit" );
    buttonToolbarEdit->setText( i18n( "Edit"  ) );

    grid_2->addMultiCellWidget( buttonToolbarEdit, 3, 3, 1, 2 );

    buttonToolbarNew = new QPushButton( GroupBox2, "buttonToolbarNew" );
    buttonToolbarNew->setText( i18n( "New"  ) );

    grid_2->addMultiCellWidget( buttonToolbarNew, 2, 2, 1, 2 );
    QSpacerItem* spacer = new QSpacerItem( 20, 53, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_2->addMultiCell( spacer, 1, 1, 1, 2 );

    grid->addWidget( GroupBox2, 0, 0 );

    GroupBox2_2 = new QGroupBox( this, "GroupBox2_2" );
    GroupBox2_2->setTitle( i18n( "Items"  ) );
    GroupBox2_2->setColumnLayout(0, Qt::Vertical );
    GroupBox2_2->layout()->setSpacing( 0 );
    GroupBox2_2->layout()->setMargin( 0 );
    grid_3 = new QGridLayout( GroupBox2_2->layout() );
    grid_3->setAlignment( Qt::AlignTop );
    grid_3->setSpacing( 6 );
    grid_3->setMargin( 11 );

    listAction = new QListView( GroupBox2_2, "listAction" );
    listAction->addColumn( i18n( "Items" ) );

    grid_3->addMultiCellWidget( listAction, 0, 4, 0, 0 );

    buttonActionUp = new QPushButton( GroupBox2_2, "buttonActionUp" );
    buttonActionUp->setText( i18n( ""  ) );
    buttonActionUp->setMaximumSize( QSize( 32, 32 ) );
    buttonActionUp->setEnabled( FALSE );
    buttonActionUp->setPixmap( image0 );

    grid_3->addWidget( buttonActionUp, 0, 1 );

    buttonActionDown = new QPushButton( GroupBox2_2, "buttonActionDown" );
    buttonActionDown->setText( i18n( ""  ) );
    buttonActionDown->setMaximumSize( QSize( 32, 32 ) );
    buttonActionDown->setEnabled( FALSE );
    buttonActionDown->setPixmap( image1 );

    grid_3->addWidget( buttonActionDown, 0, 2 );

    buttonActionRemove = new QPushButton( GroupBox2_2, "buttonActionRemove" );
    buttonActionRemove->setText( i18n( "Remove"  ) );

    grid_3->addMultiCellWidget( buttonActionRemove, 4, 4, 1, 2 );

    buttonActionEdit = new QPushButton( GroupBox2_2, "buttonActionEdit" );
    buttonActionEdit->setText( i18n( "Edit"  ) );

    grid_3->addMultiCellWidget( buttonActionEdit, 3, 3, 1, 2 );

    buttonActionNew = new QPushButton( GroupBox2_2, "buttonActionNew" );
    buttonActionNew->setText( i18n( "New"  ) );

    grid_3->addMultiCellWidget( buttonActionNew, 2, 2, 1, 2 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 53, QSizePolicy::Fixed, QSizePolicy::Expanding );
    grid_3->addMultiCell( spacer_2, 1, 1, 1, 2 );

    grid->addWidget( GroupBox2_2, 1, 0 );

    hbox = new QHBoxLayout; 
    hbox->setSpacing( 6 );
    hbox->setMargin( 0 );

    buttonHelp = new QPushButton( this, "buttonHelp" );
    buttonHelp->setText( i18n( "&Help"  ) );
    buttonHelp->setAutoDefault( TRUE );
    hbox->addWidget( buttonHelp );
    QSpacerItem* spacer_3 = new QSpacerItem( 227, 20, QSizePolicy::Expanding, QSizePolicy::Fixed );
    hbox->addItem( spacer_3 );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setText( i18n( "&OK"  ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    hbox->addWidget( buttonOk );

    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( i18n( "&Cancel"  ) );
    buttonCancel->setAutoDefault( TRUE );
    hbox->addWidget( buttonCancel );

    grid->addLayout( hbox, 2, 0 );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( buttonToolbarUp, SIGNAL( clicked() ), this, SLOT( toolbarUp() ) );
    connect( buttonToolbarDown, SIGNAL( clicked() ), this, SLOT( toolbarDown() ) );
    connect( buttonToolbarNew, SIGNAL( clicked() ), this, SLOT( toolbarNew() ) );
    connect( buttonToolbarEdit, SIGNAL( clicked() ), this, SLOT( toolbarEdit() ) );
    connect( buttonToolBarRemove, SIGNAL( clicked() ), this, SLOT( toolbarRemove() ) );
    connect( buttonActionUp, SIGNAL( clicked() ), this, SLOT( actionUp() ) );
    connect( buttonActionDown, SIGNAL( clicked() ), this, SLOT( actionDown() ) );
    connect( buttonActionNew, SIGNAL( clicked() ), this, SLOT( actionNew() ) );
    connect( buttonActionEdit, SIGNAL( clicked() ), this, SLOT( actionEdit() ) );
    connect( buttonActionRemove, SIGNAL( clicked() ), this, SLOT( actionRemove() ) );
    connect( listToolbar, SIGNAL( selectionChanged() ), this, SLOT( toolbarSelected() ) );
    connect( listAction, SIGNAL( selectionChanged() ), this, SLOT( actionSelected() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ToolBarConfig::~ToolBarConfig()
{
    // no need to delete child widgets, Qt does it all for us
}

void ToolBarConfig::actionDown()
{
    qWarning( "ToolBarConfig::actionDown(): Not implemented yet!" );
}

void ToolBarConfig::actionEdit()
{
    qWarning( "ToolBarConfig::actionEdit(): Not implemented yet!" );
}

void ToolBarConfig::actionNew()
{
    qWarning( "ToolBarConfig::actionNew(): Not implemented yet!" );
}

void ToolBarConfig::actionRemove()
{
    qWarning( "ToolBarConfig::actionRemove(): Not implemented yet!" );
}

void ToolBarConfig::actionSelected()
{
    qWarning( "ToolBarConfig::actionSelected(): Not implemented yet!" );
}

void ToolBarConfig::actionUp()
{
    qWarning( "ToolBarConfig::actionUp(): Not implemented yet!" );
}

void ToolBarConfig::toolbarSelected()
{
    qWarning( "ToolBarConfig::toolbarSelected(): Not implemented yet!" );
}

void ToolBarConfig::toolbarDown()
{
    qWarning( "ToolBarConfig::toolbarDown(): Not implemented yet!" );
}

void ToolBarConfig::toolbarEdit()
{
    qWarning( "ToolBarConfig::toolbarEdit(): Not implemented yet!" );
}

void ToolBarConfig::toolbarNew()
{
    qWarning( "ToolBarConfig::toolbarNew(): Not implemented yet!" );
}

void ToolBarConfig::toolbarRemove()
{
    qWarning( "ToolBarConfig::toolbarRemove(): Not implemented yet!" );
}

void ToolBarConfig::toolbarUp()
{
    qWarning( "ToolBarConfig::toolbarUp(): Not implemented yet!" );
}

void ToolBarConfig::updateActionsList()
{
    qWarning( "ToolBarConfig::updateActionsList(): Not implemented yet!" );
}

void ToolBarConfig::updateToolbarsList()
{
    qWarning( "ToolBarConfig::updateToolbarsList(): Not implemented yet!" );
}

