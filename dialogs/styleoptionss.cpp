/****************************************************************************
** Form implementation generated from reading ui file 'styleoptionss.ui'
**
** Created: Tue Oct 31 23:15:28 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "styleoptionss.h"

#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a StyleOptionsS which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
StyleOptionsS::StyleOptionsS( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "StyleOptionsS" );
    resize( 355, 351 ); 
    setCaption( tr( "Form2"  ) );
    grid = new QGridLayout( this ); 
    grid->setSpacing( 6 );
    grid->setMargin( 11 );

    checkTagsCapital = new QCheckBox( this, "checkTagsCapital" );
    checkTagsCapital->setText( tr( "Use upper case symbols for tags"  ) );

    grid->addWidget( checkTagsCapital, 0, 0 );

    checkAttrCapital = new QCheckBox( this, "checkAttrCapital" );
    checkAttrCapital->setText( tr( "Use upper case symbols for attributes of tags"  ) );

    grid->addWidget( checkAttrCapital, 1, 0 );

    checkEndTag = new QCheckBox( this, "checkEndTag" );
    checkEndTag->setText( tr( "Insert end of tag if applicable"  ) );

    grid->addWidget( checkEndTag, 2, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    grid->addItem( spacer, 3, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
StyleOptionsS::~StyleOptionsS()
{
    // no need to delete child widgets, Qt does it all for us
}

