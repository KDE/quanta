#include "tagaction.h"
#include <qdom.h>

TagAction::TagAction( QDomElement *element, QObject* parent, const char* name, QuantaView *view )
  : view_(view),
    tag(element),
    KAction( element->attribute("text",""), 0, parent, element->attribute("name","") )
  
{
   if ( view_ )
        connect( this, SIGNAL(activated()), SLOT(insertTag()) );
}


TagAction::~TagAction()
{
}



void TagAction::insertTag()
{
    if ( !view_ ) 
       return;
/*    if ( tag->attribute() ) {
       if ( closeTag_.isNull() )
            view_->insertTag( openTag_ );
       else
            view_->write()->insertTag( openTag_, closeTag_ );
    }
*/
    
}
