
#include "colorrequester.h"
#include <klineedit.h>
#include <kcombobox.h>
#include <kcolordialog.h>
#include <qtooltip.h>
#include <qiconset.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kaccel.h>

class colorRequester::colorRequesterPrivate{
  public:
    KLineEdit *edit;
    colorRequesterPrivate() { edit = 0L; }
    void setText( const QString& text ) { edit->setText( text ); }
    void connectSignals( QObject *receiver ) { connect( edit, SIGNAL( textChanged( const QString& )),receiver, SIGNAL( textChanged( const QString& ))); }
};

colorRequester::colorRequester(QWidget *parent, const char* name) : QHBox(parent,name){
  d = new colorRequesterPrivate;
  init();
}
colorRequester::~colorRequester(){
  delete myColorDialog;
  delete d;
}

void colorRequester::init()
{
     myColorDialog    = 0L;

     if ( !d->edit )
     d->edit = new KLineEdit( this, "line edit" );

     myButton = new KPushButton( this, "kfile button");
     QIconSet iconSet = SmallIconSet(QString::fromLatin1("xpaint"));
     QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );
     myButton->setIconSet( iconSet );
     myButton->setFixedSize( pixMap.width()+8, pixMap.height()+8 );
     QToolTip::add(myButton, i18n("Open color dialog"));

     setSpacing( KDialog::spacingHint() );

     QWidget *widget = (QWidget*) d->edit;
     setFocusProxy( widget );

     d->connectSignals( this );
     connect( myButton, SIGNAL( clicked() ), this, SLOT( openColorDialog() ));

 
     KAccel *accel = new KAccel( this );
     accel->insert( KStdAccel::Open, this, SLOT( openColorDialog() ));
     accel->readSettings();
 }

void colorRequester::openColorDialog(){
  QColor myColor;
  int result = KColorDialog::getColor( myColor );
  if ( result == KColorDialog::Accepted ){
    d->edit->setText(myColor.name());
    emit textChanged(myColor.name());
  }
}
 
KLineEdit * colorRequester::lineEdit() const{
  return d->edit;
}
KPushButton * colorRequester::button() const{
  return myButton;
}




#include "colorrequester.moc"
