#include "toolbarconfigi.h"

/* 
 *  Constructs a ToolBarConfigI which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ToolBarConfigI::ToolBarConfigI( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : ToolBarConfig( parent, name, modal, fl )
{
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ToolBarConfigI::~ToolBarConfigI()
{
    // no need to delete child widgets, Qt does it all for us
}

/* 
 * public slot
 */
void ToolBarConfigI::actionDown()
{
    qWarning( "ToolBarConfigI::actionDown() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::actionEdit()
{
    qWarning( "ToolBarConfigI::actionEdit() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::actionNew()
{
    qWarning( "ToolBarConfigI::actionNew() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::actionRemove()
{
    qWarning( "ToolBarConfigI::actionRemove() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::actionSelected()
{
    qWarning( "ToolBarConfigI::actionSelected() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::actionUp()
{
    qWarning( "ToolBarConfigI::actionUp() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::toolbarSelected()
{
    qWarning( "ToolBarConfigI::toolbarSelected() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::toolbarDown()
{
    qWarning( "ToolBarConfigI::toolbarDown() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::toolbarEdit()
{
    qWarning( "ToolBarConfigI::toolbarEdit() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::toolbarNew()
{
    qWarning( "ToolBarConfigI::toolbarNew() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::toolbarRemove()
{
    qWarning( "ToolBarConfigI::toolbarRemove() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::toolbarUp()
{
    qWarning( "ToolBarConfigI::toolbarUp() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::updateActionsList()
{
    qWarning( "ToolBarConfigI::updateActionsList() not yet implemented!" ); 
}
/* 
 * public slot
 */
void ToolBarConfigI::updateToolbarsList()
{
    qWarning( "ToolBarConfigI::updateToolbarsList() not yet implemented!" ); 
}

