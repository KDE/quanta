/***************************************************************************
 *   Copyright (C) 2003 by Gulmini Luciano                                          *
 *   gulmini.luciano@student.unife.it                                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "shorthandformer.h"
#include <qstringlist.h>
#include "cssshpropertyparser.h"
#include <qregexp.h>
#include <kdebug.h>
#include "csseditor_globals.h" 

QStringList HTMLColorList(QStringList::split(",",HTMLColors));

ShorthandFormer::ShorthandFormer(){}

ShorthandFormer::ShorthandFormer( QMap<QString,QString> m)
{
  m_properties = m;
  cue_after = QString::null;
  cue_before = QString::null;
  pause_before = QString::null;
  pause_after = QString::null;
  background_color = QString::null;
  background_image = QString::null;
  background_repeat = QString::null;
  background_attachment = QString::null;
  background_position = QString::null;
  border_top_style = QString::null;
  border_top_color = QString::null;
  border_top_width = QString::null;
  border_left_style = QString::null;
  border_left_color = QString::null;
  border_left_width = QString::null;
  border_right_style = QString::null;
  border_right_color = QString::null;
  border_right_width = QString::null;
  border_bottom_style = QString::null;
  border_bottom_color = QString::null;
  border_bottom_width = QString::null;
  outline_style = QString::null;
  outline_color = QString::null;
  outline_width = QString::null;
  list_style_type = QString::null;
  list_style_image = QString::null;
  list_style_position = QString::null;
  font_style = QString::null;
  font_variant = QString::null;
  font_weight = QString::null;
  font_size = QString::null;
  line_height = QString::null;
  font_family = QString::null;
  margin_top = QString::null;
  margin_bottom = QString::null;
  margin_left = QString::null;
  margin_right = QString::null;
  padding_top = QString::null;
  padding_bottom = QString::null;
  padding_left = QString::null;
  padding_right = QString::null;
  
   if(m_properties.contains("cue-after")){
                      cue_after= m_properties["cue-after"];
                      m_properties.remove("cue-after");
                   }
                   if(m_properties.contains("cue-before")){
                     cue_before = m_properties["cue-before"];
                     m_properties.remove("cue-before");
                   }
                   if(m_properties.contains("pause-before")){
                      pause_before= m_properties["pause-before"];
                      m_properties.remove("pause-before");
                   }
                   if(m_properties.contains("pause-after")){
                     pause_after = m_properties["pause-after"];
                     m_properties.remove("pause-after");
                   }
                   if(m_properties.contains("background-color")){
                     background_color = m_properties["background-color"];
                     m_properties.remove("background-color");
                   }
                   if(m_properties.contains("background-image")){
                     background_image = m_properties["background-image"];
                     m_properties.remove("background-image");
                   }
                   if(m_properties.contains("background-repeat")){
                     background_repeat = m_properties["background-repeat"];
                     m_properties.remove("background-repeat");
                   }
                   if(m_properties.contains("background-attachment")){
                     background_attachment = m_properties["background-attachment"];
                     m_properties.remove("background-attachment");
                   }
                   if(m_properties.contains("background-position")){
                     background_position = m_properties["background-position"];
                     m_properties.remove("background-position");
                   }
                   if(m_properties.contains("border-top-style")){
                     border_top_style = m_properties["border-top-style"];
                     m_properties.remove("border-top-style");
                   }
                   if(m_properties.contains("border-top-color")){
                     border_top_color = m_properties["border-top-color"];
                     m_properties.remove("border-top-color");
                   }
                   if(m_properties.contains("border-top-width")){
                     border_top_width = m_properties["border-top-width"];
                     m_properties.remove("border-top-width");
                   }
                   if(m_properties.contains("border-left-style")){
                     border_left_style = m_properties["border-left-style"];
                     m_properties.remove("border-left-style");
                   }
                   if(m_properties.contains("border-left-color")){
                     border_left_color = m_properties["border-left-color"];
                     m_properties.remove("border-left-color");
                   }
                   if(m_properties.contains("border-left-width")){
                     border_left_width = m_properties["border-left-width"];
                     m_properties.remove("border-left-width");
                   }
                   if(m_properties.contains("border-right-style")){
                     border_right_style = m_properties["border-right-style"];
                     m_properties.remove("border-right-style");
                   }
                   if(m_properties.contains("border-right-color")){
                     border_right_color = m_properties["border-right-color"];
                     m_properties.remove("border-right-color");
                   }
                   if(m_properties.contains("border-right-width")){
                     border_right_width= m_properties["border-right-width"];
                     m_properties.remove("border-right-width");
                   }
                   if(m_properties.contains("border-bottom-style")){
                     border_bottom_style = m_properties["border-bottom-style"];
                     m_properties.remove("border-bottom-style");
                   }
                   if(m_properties.contains("border-bottom-color")){
                     border_bottom_color = m_properties["border-bottom-color"];
                     m_properties.remove("border-bottom-color");
                   }
                   if(m_properties.contains("border-bottom-width")){
                     border_bottom_width = m_properties["border-bottom-width"];
                     m_properties.remove("border-bottom-width");
                   }
                   if(m_properties.contains("outline-style")){
                     outline_style = m_properties["outline-style"];
                     m_properties.remove("outline-style");
                   }
                   if(m_properties.contains("outline-color")){
                     outline_color = m_properties["outline-color"];
                     m_properties.remove("outline-color");
                   }
                   if(m_properties.contains("outline-width")){
                     outline_width = m_properties["outline-width"];
                     m_properties.remove("outline-width");
                   }
                   if(m_properties.contains("list-style-type")){
                     list_style_type= m_properties["list-style-type"];
                     m_properties.remove("list-style-type");
                   }
                   if(m_properties.contains("list-style-image")){
                     list_style_image = m_properties["list-style-image"];
                     m_properties.remove("list-style-image");
                   }
                   if(m_properties.contains("list-style-position")){
                     list_style_position = m_properties["list-style-position"];
                     m_properties.remove("list-style-position");
                   }
                   if(m_properties.contains("font-style")){
                     font_style = m_properties["font-style"];
                     m_properties.remove("font-style");
                   }
                   if(m_properties.contains("font-variant")){
                     font_variant = m_properties["font-variant"];
                     m_properties.remove("font-variant");
                   }
                   if(m_properties.contains("font-weight")){
                     font_weight = m_properties["font-weight"];
                     m_properties.remove("font-weight");
                   }
                   if(m_properties.contains("font-size")){
                     font_size = m_properties["font-size"];
                     m_properties.remove("font-size");
                   }
                   if(m_properties.contains("line-height")){
                     line_height= m_properties["line-height"];
                     m_properties.remove("line-height");
                   }
                   if(m_properties.contains("font-family")){
                     font_family = m_properties["font-family"];
                     m_properties.remove("font-family");
                   }
                   if(m_properties.contains("margin-top")){
                     margin_top = m_properties["margin-top"];
                     m_properties.remove("margin-top");
                   }
                   if(m_properties.contains("margin-bottom")){
                     margin_bottom = m_properties["margin-bottom"];
                     m_properties.remove("margin-bottom");
                   }
                   if(m_properties.contains("margin-left")){
                     margin_left = m_properties["margin-left"];
                     m_properties.remove("margin-left");
                   }
                   if(m_properties.contains("margin-right")){
                     margin_right = m_properties["margin-right"];
                     m_properties.remove("margin-right");
                   }
                   if(m_properties.contains("padding-top")){
                     padding_top = m_properties["padding-top"];
                     m_properties.remove("padding-top");
                   }
                   if(m_properties.contains("padding-bottom")){
                     padding_bottom = m_properties["padding-bottom"];
                     m_properties.remove("padding-bottom");
                   }
                   if(m_properties.contains("padding-left")){
                     padding_left = m_properties["padding-left"];
                     m_properties.remove("padding-left");
                   }
                   if(m_properties.contains("padding-right")){
                     padding_right = m_properties["padding-right"];
                     m_properties.remove("padding-right");
                   }
                
}

ShorthandFormer::~ShorthandFormer()
{
}

QString ShorthandFormer::compress()
{                             
  QString props(QString::null);   
  
   props += compressCueProp(); 
   props += compressPauseProp(); 
   props += compressBackgroundProp();
   props += compressFontProp();
   props += compressPaddingProp();    
   props += compressMarginProp();  
   props += compressOutlineProp();
   props += compressListStyleProp();
   props += compressBorderProp();

 
                                       
  QMap<QString,QString>::Iterator it;             
  for ( it = m_properties.begin(); it != m_properties.end(); ++it )           
      props += ( it.key() + " : " + it.data() + "; " );
    
  props.truncate(props.length()-1);//the last white space creates some problem: better remove it
  return props;  
}

QString ShorthandFormer::compressBorderProp(){
  QString props(QString::null);
  
  bool allColorSidesSet = false,
         allStyleSidesSet = false,
         allWidthSidesSet = false;
     
   
  if(!border_left_color.isEmpty())
     if( ( border_left_color == border_top_color ) && ( border_top_color == border_right_color ) && ( border_right_color == border_bottom_color ) )
         allColorSidesSet = true;
          
   if(!border_left_style.isEmpty())  
     if( ( border_left_style == border_top_style ) && ( border_top_style == border_right_style ) && ( border_right_style == border_bottom_style ) )
       allStyleSidesSet = true;        
             
   if(!border_left_width.isEmpty())  
     if( ( border_left_width == border_top_width ) && ( border_top_width == border_right_width ) && ( border_right_width == border_bottom_width ) )
       allWidthSidesSet = true;          
                   
   if ( allColorSidesSet ) {
     if ( allStyleSidesSet ) {
       if ( allWidthSidesSet ) {
         props += "border : " + border_left_color + " " + border_left_style + " " + border_left_width +"; ";

       }
       else {
         props += "border : " + border_left_color + " " + border_left_style +"; "; 
         props += compressBorderWidthProp();
       }
     }
     else {
       if ( allWidthSidesSet ) {
         props += "border : " + border_left_color + " " + border_left_width +"; ";
         props += compressBorderStyleProp();
       }
       else {
         props += "border : " + border_left_color +"; "; 
         props += compressBorderWidthProp();
         props += compressBorderStyleProp();
       }    
     
     }
   }
   else { // allColorSidesSet is false
     if ( allStyleSidesSet ) {
       if ( allWidthSidesSet ) {
         props += "border : " + border_left_style + " " + border_left_width +"; ";
         props += compressBorderColorProp();
       }
       else {
         props += compressBorderStyleProp();
         props += compressBorderWidthProp();
         props += compressBorderColorProp();
       }
     }
     else {
         
         props += compressBorderStyleProp();
         props += compressBorderWidthProp();
         props += compressBorderColorProp();    
     }
   }     
         
  return props;          
}

QString ShorthandFormer::compressBorderStyleProp(){
  return compressImplementation( "border-style" ,border_top_style, border_bottom_style, border_right_style, border_left_style, "none");
}

QString ShorthandFormer::compressBorderWidthProp(){
  return compressImplementation( "border-width" ,border_top_width, border_bottom_width, border_right_width, border_left_width, "medium");
}

QString ShorthandFormer::compressBorderColorProp(){
//because the default value of color property is browser dependant, this method doesn't compress the color value
  QString props(QString::null);
  if( !border_top_color.isEmpty() )
    props += ("border-top-color : " + border_top_color +"; ");
  if( !border_right_color.isEmpty() )
    props += ("border-right-color : " + border_right_color +"; ");
  if( !border_bottom_color.isEmpty() )
    props += ("border-bottom-color : " + border_bottom_color +"; ");
  if( !border_left_color.isEmpty() )
    props += ("border-left-color : " + border_left_color +"; ");      
  return props;
}

QString ShorthandFormer::compressFontProp(){
  QString fontProp(QString::null),
               props(QString::null);
  bool appendLineHeight = false;  
  
  if( font_style.isEmpty() && font_variant.isEmpty() && font_weight.isEmpty() && font_size.isEmpty() && font_family.isEmpty() ) { 
    if( !line_height.isEmpty() )
      props += ("line-height : " + line_height + "; ");
  }
  else {
     if( !font_style.isEmpty() ) 
       fontProp += ( " " + font_style);  
     if( !font_variant.isEmpty() ) 
       fontProp += ( " " + font_variant);
     if( !font_weight.isEmpty() )
       fontProp += ( " " + font_weight); 
     if( !font_family.isEmpty() ) 
       fontProp += ( " " + font_family);
     if( !font_size.isEmpty() ){
       fontProp += ( " " + font_size);
       if( !line_height.isEmpty() ) 
         fontProp += ( "/" + line_height.stripWhiteSpace() );      
     } 
     else 
        if( !line_height.isEmpty() )
          appendLineHeight = true;         
   if( !fontProp.isEmpty() )
    props += ( "font :" + fontProp + "; ");
   if(appendLineHeight) 
    props += ( "line-height : " + line_height + "; ");
  }  
   return props; 
}

QString ShorthandFormer::compressCueProp(){
  return compressImplementation2( "cue", cue_after, cue_before, "none");
}   
       
QString ShorthandFormer::compressPauseProp(){
  return compressImplementation2( "pause", pause_after, pause_before, "0");
}   

QString ShorthandFormer::compressBackgroundProp(){
  QString backgroundProp(QString::null);
  if( !background_color.isEmpty() ) 
    backgroundProp += (" " + background_color );
  if( !background_image.isEmpty() )
    backgroundProp += (" " + background_image );       
  if( !background_repeat.isEmpty() ) 
    backgroundProp += (" " + background_repeat );
  if( !background_attachment.isEmpty() ) 
    backgroundProp += (" " + background_attachment );
  if( !background_position.isEmpty() )                      
    backgroundProp += (" " + background_position );   
  if( !backgroundProp.isEmpty() )
    return ( "background :" + backgroundProp + "; ");
    
  return QString::null;    
}    

QString ShorthandFormer::compressPaddingProp(){
  return compressImplementation( "padding" ,padding_top, padding_bottom, padding_right, padding_left, "medium");
}

QString ShorthandFormer::compressMarginProp(){  
 return compressImplementation( "margin" ,margin_top, margin_bottom, margin_right, margin_left, "0");  
}

QString ShorthandFormer::compressOutlineProp(){
  return compressImplementation3("outline", outline_color, outline_style, outline_width);
}
QString ShorthandFormer::compressListStyleProp(){
  return compressImplementation3("list-type", list_style_type, list_style_image, list_style_position);
}

QString ShorthandFormer::compressImplementation3( QString prop, QString p1, QString p2, QString p3){
QString props(QString::null);
  if( !p1.isEmpty() ) 
    props += (" " + p1 );
  if( !p2.isEmpty() )
    props += (" " + p2 );       
  if( !p3.isEmpty() )                      
    props += (" " + p3 );   
  if( !props.isEmpty() )
    return ( prop + " :" + props + "; ");    
  return QString::null; 
}

QString ShorthandFormer::compressImplementation2( QString prop, QString after, QString before, QString defValue){
  QString props(QString::null);                  
  if(after == before){
    if(!after.isEmpty())
      props+=( prop + " : " + after + "; ");
  }
  else {
    if(before.isEmpty())
      props+=( prop + " : " + defValue + " " + after + "; ");
    else   
      if(pause_after.isEmpty())
        props+=( prop + " : " + before + " " + defValue + "; ");
      else  
        props+=( prop + " : " + before + " " + after + "; ");
  }   
  return props;
}  

QString ShorthandFormer::compressImplementation( QString prop, QString t, QString b, QString r, QString l, QString defValue){
unsigned int boxSide = 0;
 QString props(QString::null), 
              top(t.stripWhiteSpace()),
              bottom(b.stripWhiteSpace()),
              left(l.stripWhiteSpace()),
              right(r.stripWhiteSpace());   
   if( !top.isEmpty() )
     boxSide += 1;  
   if( !bottom.isEmpty() )
     boxSide += 2;
   if( !left.isEmpty() )
     boxSide += 4;
   if( !right.isEmpty() )
     boxSide += 8;    
        
   switch(boxSide) {
     case 1: if( top != defValue ) 
                    props   += ( prop +" : " + top + " " + defValue + " " + defValue+"; "); 
                  break; 
     
     case 2: if( bottom != defValue )
                    props   += ( prop + " : " + defValue + " " + defValue + " " + bottom + "; "); 
                  break; 
     
     case 3: if( top != bottom )
                    props   += ( prop +" : " + top + " " + defValue + " " + bottom + "; ");
                  else
                    if( top!=defValue && top == bottom  )    
                      props   += ( prop +" : " + top + " " + defValue +"; "); 
                  break;
     
     case 4: if( left != defValue )
                    props   += ( prop +" : " + defValue + " " + defValue + " " + defValue +" " + left + "; "); 
                  break; 
     
     case 5: if( left == defValue && top != defValue)
                    props   += ( prop +" : " + top + " " + defValue + " " + defValue+ "; ");
                  else
                    if( left != defValue )    
                      props   += ( prop +" : " + top + " " + defValue + " " + defValue + " " + left + "; "); 
                  break;
     
     case 6: if( left == defValue && bottom != defValue)
                    props   += ( prop +" : " + defValue + " " + defValue + " " + bottom + "; ");   
                  else
                    if( left != defValue )    
                      props   += ( prop +" : " + defValue + " " +defValue + " " + bottom + " " + left + "; ");
                  break;
     
     case 7: if( left != defValue ) 
                      props   += ( prop +" : " + top + " " + defValue + " " + bottom + " " + left + "; ");
                  else 
                     if( top != defValue && top == bottom ) 
                       props   += ( prop +" : " + top + " " + defValue + "; "); 
                     else
                       if( top != defValue || bottom != defValue )
                         props   += ( prop +" : " + top + " " + defValue + " " + bottom  + "; "); 
                  break;
     
     case 8: if( right != defValue )
                    props   += ( prop +" : " + defValue + " " + right + " " + defValue + " " + defValue + "; "); 
                  break; 
     
     case 9: if( right == defValue && top != defValue)
                    props   += ( prop +" : " + top + " " + defValue + " " + defValue + "; ");
                  else
                    if( right != defValue )    
                      props   += ( prop +" : " + top + " " + right + " " + defValue + " " + defValue + "; "); 
                  break;
     
     case 10: if( right == defValue && bottom != defValue)
                      props += ( prop +" : " + defValue + " " + defValue + " " + bottom +"; ");
                    else
                      if( right !=defValue)  
                        props += ( prop +" : " + defValue + " " + right + " " + bottom + " " + defValue + "; ");  
                    break;
     
     case 11: if( right != defValue ) 
                      props   += ( prop +" : " + top + " " + right + " " + bottom + " " + defValue + "; ");
                  else 
                     if( top != defValue && top == bottom ) 
                       props   += ( prop +" : " + top + " " + defValue + "; "); 
                     else
                       if( top != defValue || bottom != defValue )
                         props   += ( prop +" : " + top + " " + defValue + " " + bottom  + "; "); 
                  break;
     
     case 12: if( right != left )
                      props += ( prop +" : " + defValue +" " + right + " " + defValue + " " + left +"; "); 
                    else
                      if( right != defValue && left == right )
                        props += ( prop +" : " + defValue + " " + right + "; " ); 
                    break;
    
     case 13: if( right != defValue && right == left ) 
                      props   += ( prop +" : " + top + " " +right + " " + defValue +"; ");
                    else 
                      if( right != defValue ||  left != defValue ) 
                       props   += ( prop +" : " + top + " " + right + " " + defValue + " " + left +"; "); 
                    break;
     
     case 14: if( right != defValue && right == left ) 
                      props   += ( prop +" : " + defValue + " " +right + " " + bottom +"; ");
                    else 
                      if( right != defValue ||  left != defValue ) 
                       props   += ( prop +" : " + defValue + " " + right + " " +bottom + " " + left +"; "); 
                    break;
     
     case 15: { unsigned int defaultValueOccurence = 0;
                      if( top == defValue )
                        defaultValueOccurence += 1;  
                      if( bottom == defValue )
                        defaultValueOccurence += 2;
                      if( left == defValue )
                        defaultValueOccurence += 4;
                      if( right == defValue )
                        defaultValueOccurence += 8; 
                      switch( defaultValueOccurence ) {
                      case 1: if( left == right )
                                     props   += ( prop +" : " + defValue + " "+ right + " " + bottom + "; "); 
                                   else
                                     props   += ( prop +" : " + right + " " +bottom + " " + left +"; ");  
                                   break;
                      case 2: if( left == right )
                                     props   += ( prop +" : " + top + " " + right + " " + defValue + "; "); 
                                   else
                                     props   += ( prop +" : " + top + " " + right + " " + bottom + " " + defValue +"; ");  
                                   break;
                      case 3: if( left == right )
                                     props   += ( prop +" : " + defValue +" " + right + "; "); 
                                   else
                                     props   += ( prop +" : " + defValue + " " + right + " " + defValue + " " + left +"; ");  
                                   break;
                      case 4: props   += ( prop +" : " + top + " " + right + " " + bottom + " " + defValue + "; ");break;
                      case 5: props   += ( prop +" : " + defValue + " " + right + " " + bottom + " " + defValue + "; ");break;
                      case 6: props   += ( prop +" : " + top+ " " + right + " " + defValue + " " + defValue + "; ");break;
                      case 7: props   += ( prop +" : " + defValue + " " + right + " " + defValue  + " " + defValue + "; ");break;
                      case 8: props   += ( prop +" : " + top + " " + defValue + " " + bottom + " " + left +"; ");break;
                      case 9: props   += ( prop +" : " + defValue + " " + defValue + " " + bottom + " " + left +"; ");break;
                      case 10: props   += ( prop +" : " + top + " " + defValue + " " + defValue + " " + left +"; ");break;
                      case 11: props   += ( prop +" : " + defValue + " " + defValue + " " + defValue + " " + left +"; ");break;
                      case 12:if( top == bottom )
                                     props   += ( prop +" : " + top + " " + defValue + "; "); 
                                   else
                                     props   += ( prop +" : " + top + " " + defValue + " " + bottom + " " + defValue+ "; ");  
                                   break;
                      case 13: props   += ( prop +" : " + defValue + " " + defValue + " " + bottom + " " + defValue + "; ");break;
                      case 14: props   += ( prop +" : " + top + " " + defValue + " " + defValue + " " + defValue +"; ");break;
                      case 15:;break;
                      default:break;
                      }                        
                        }
                        break;
     
     default:break; 
   }
   return props;
}

QMap<QString,QString> ShorthandFormer::expand( QString propertyName, QString propertyValue ){
  CSSSHPropertyParser parser(propertyValue);
  QStringList foundValues = parser.parse();
  
  if( propertyName == "cue" ) 
    return expandCueProp(foundValues);
  if( propertyName == "pause")
    return expandPauseProp(foundValues);  
  if( propertyName == "background")
    return expandBackgroundProp(foundValues);   
  if( propertyName == "border-color")
    return expandBox("color", foundValues);
  if( propertyName == "border-style")
    return expandBox("style", foundValues);
  if( propertyName == "border-width")
    return expandBox("width", foundValues);  
  if( propertyName == "font")
    return  expandFontProp(foundValues);
  if( propertyName == "outline")
    return  expandOutlineProp(foundValues);
  if( propertyName == "list-style")
    return  expandListstyleProp(foundValues);
  if( propertyName == "border-bottom")
    return  expandBoxSide("bottom",foundValues);
  if( propertyName == "border-top")
    return  expandBoxSide("top",foundValues);
  if( propertyName == "border-left")
    return  expandBoxSide("left",foundValues);
  if( propertyName == "border-right")
    return  expandBoxSide("right",foundValues);  
  if( propertyName == "border")
    return  expandBorderProp(foundValues);   
  
  return QMap<QString,QString>();
}

QMap<QString,QString> ShorthandFormer::expandCueProp(QStringList l){
  return expandImplementation("cue",l);
}

QMap<QString,QString> ShorthandFormer::expandPauseProp(QStringList l){
  return expandImplementation("pause",l);
}


QMap<QString,QString>  ShorthandFormer::expandImplementation(QString propertyName, QStringList l){
  QMap<QString,QString> expandedProps;
       if( l.count()==1) {
         expandedProps[propertyName + "-before"] = l[0] ; 
         expandedProps[propertyName + "-after"] = l[0] ; 
         return expandedProps;
       }     
      else
      {
         expandedProps[propertyName + "-before"] = l[0] ; 
         expandedProps[propertyName + "-after"] = l[1] ; 
         return expandedProps;
       } 
}


QMap<QString,QString>  ShorthandFormer::expandBackgroundProp(QStringList l){
  QMap<QString,QString> expandedProps;       
  if(l.count()==1 && l[0] == "inherit"){ // it works also as protection against wrong single value inserted
    expandedProps["background-color"] = l[0];
    expandedProps["background-image"] = l[0];
    expandedProps["background-repeat"] = l[0];
    expandedProps["background-attachment"] =l[0];
    expandedProps["background-position"] = l[0];
  }
  else { 
    QRegExp percentagePattern("\\d*%"),
                    lengthPattern("\\d*ex|em|px"),
                    colorPattern("#[\\w\\d]{6}"),
                    numberPattern("\\d*");
    QStringList::Iterator it = l.begin();

    while (  it != l.end() ) {  
      if( (*it).contains("url(") || ((*it).stripWhiteSpace()) == "none" || ((*it).stripWhiteSpace()) == "inherit" ){
        expandedProps["background-image"] = ((*it));
      }
      else
      if( ((*it).stripWhiteSpace()) == "repeat" || ((*it).stripWhiteSpace()) == "repeat-x" || ((*it).stripWhiteSpace()) == "repeat-y" || ((*it).stripWhiteSpace()) == "no-repeat" || ((*it).stripWhiteSpace()) == "inherit" ) {
        expandedProps["background-repeat"] = ((*it));
      }
      else
      if( ((*it).stripWhiteSpace()) == "scroll" || ((*it).stripWhiteSpace()) == "fixed" || ((*it).stripWhiteSpace()) == "inherit"){
        expandedProps["background-attachment"] = ((*it));
      }
      else
      if( ((*it).stripWhiteSpace()) == "top" || ((*it).stripWhiteSpace()) == "center" || ((*it).stripWhiteSpace()) == "bottom" || ((*it).stripWhiteSpace()) == "left" || ((*it).stripWhiteSpace()) == "right" || (*it).contains(percentagePattern) || (*it).contains(lengthPattern) || ((*it).stripWhiteSpace()) == "inherit"){
        if( expandedProps.contains("background-position") )
          expandedProps["background-position"] = ( expandedProps["background-position"] + " " + ((*it)) );
        else    
          expandedProps["background-position"] = ((*it));
      }
      else
      if( (*it).contains(colorPattern) || HTMLColorList.contains((*it))!=0 || ((*it).stripWhiteSpace()) == "transparent" || ((*it).stripWhiteSpace()) == "inherit" ){
        expandedProps["background-color"] = ((*it));
      }  
      ++it;  
    }     
  }
  return expandedProps;
}


QMap<QString,QString>  ShorthandFormer::expandBox(QString subPropName, QStringList l){
    QMap<QString,QString> expandedProps;       
  switch(l.count()){
  case 1 : expandedProps["border-top-" + subPropName] = l[0];
                expandedProps["border-right-" + subPropName] = l[0];
                expandedProps["border-bottom-" + subPropName] = l[0];
                expandedProps["border-left-" + subPropName] = l[0];
                break;
  case 2 : expandedProps["border-top-" + subPropName] = l[0];
                expandedProps["border-right-" + subPropName] = l[1];
                expandedProps["border-bottom-" + subPropName] = l[0];
                expandedProps["border-left-" + subPropName] = l[1];
                break;
 case 3 : expandedProps["border-top-" + subPropName] = l[0];
                expandedProps["border-right-" + subPropName] = l[1];
                expandedProps["border-bottom-" + subPropName] = l[2];
                expandedProps["border-left-" + subPropName] = l[1];
                break;
 case 4 : expandedProps["border-top-" + subPropName] = l[0];
                expandedProps["border-right-" + subPropName] = l[1];
                expandedProps["border-bottom-" + subPropName] = l[2];
                expandedProps["border-left-" + subPropName] = l[4];
                break;
   default:break;               
  }
  return expandedProps;
}

QMap<QString,QString>  ShorthandFormer::expandFontProp(QStringList l){
  QMap<QString,QString> expandedProps; 
  QRegExp percentagePattern("/\\d*%"),
                   lengthPattern("/\\d*ex|em|px"),
                   numberPattern("/\\d*");
   QStringList fontPseudoSHFormValues;
   fontPseudoSHFormValues.append("caption"); 
   fontPseudoSHFormValues.append("icon");  
   fontPseudoSHFormValues.append("menu");  
   fontPseudoSHFormValues.append("message-box");  
   fontPseudoSHFormValues.append("small-caption");  
   fontPseudoSHFormValues.append("status-bar");
   if( l.count()==1 && fontPseudoSHFormValues.contains(l[0]) != 0) {
    expandedProps["font"] = l[0];
    return expandedProps; 
    }
   else {               
   QStringList::Iterator it = l.begin();
    while (  it != l.end() ) { 
      if( ((*it).stripWhiteSpace()) == "oblique" || ((*it).stripWhiteSpace()) == "italic" || ((*it).stripWhiteSpace()) == "normal" || ((*it).stripWhiteSpace()) == "inherit" ){
        expandedProps["font-style"] = ((*it));
      }       
      else
      if( ((*it).stripWhiteSpace()) == "normal" || ((*it).stripWhiteSpace()) == "small-caps" || ((*it).stripWhiteSpace()) == "inherit"){
          expandedProps["font-variant"] = ((*it)) ;
      }      
      else
      if( ((*it).stripWhiteSpace()) == "900" || ((*it).stripWhiteSpace()) == "800" || ((*it).stripWhiteSpace()) == "700" || ((*it).stripWhiteSpace()) == "600" || ((*it).stripWhiteSpace()) == "500" || ((*it).stripWhiteSpace()) == "400" || ((*it).stripWhiteSpace()) == "300" || ((*it).stripWhiteSpace()) == "200" || ((*it).stripWhiteSpace()) == "100" || ((*it).stripWhiteSpace()) == "lighter" || ((*it).stripWhiteSpace()) == "bolder" || ((*it).stripWhiteSpace()) == "normal" || ((*it).stripWhiteSpace()) == "bold" || ((*it).stripWhiteSpace()) == "inherit"){
        expandedProps["font-weight"] = ((*it));
      }
      else
      if( ((*it).stripWhiteSpace()) == "smaller" || ((*it).stripWhiteSpace()) == "larger" || ((*it).stripWhiteSpace()) == "xx-large" || ((*it).stripWhiteSpace()) == "x-large" || ((*it).stripWhiteSpace()) == "large" || ((*it).stripWhiteSpace()) == "medium" || ((*it).stripWhiteSpace()) == "small" || ((*it).stripWhiteSpace()) == "x-small" || ((*it).stripWhiteSpace()) == "xx-small" || ((*it).stripWhiteSpace()) == "inherit" ){
        expandedProps["font-size"] = ((*it));
      }
      else
      if( (*it).contains(percentagePattern) || (*it).contains(numberPattern) || (*it).contains(lengthPattern) || (*it).stripWhiteSpace() == "/normal" || (*it).stripWhiteSpace() == "/inherit" ) {
        expandedProps["line-height"] = ((*it).remove('/'));
      }
      else
        expandedProps["font-family"] = ((*it));
      ++it;  
    }  
  return expandedProps;
  }
}    

QMap<QString,QString> ShorthandFormer::expandListstyleProp( QStringList l){
  QMap<QString,QString> expandedProps;
  QStringList::Iterator it = l.begin();
     while (  it != l.end() ) { 
      if( (*it).contains("url(") || ((*it).stripWhiteSpace()) == "none" || ((*it).stripWhiteSpace()) == "inherit" ){
        expandedProps["list-style-image"] = ((*it));
      }       
      else
      if( ((*it).stripWhiteSpace()) == "disc" || ((*it).stripWhiteSpace()) == "circle" || ((*it).stripWhiteSpace()) == "square" || ((*it).stripWhiteSpace()) == "decimal" || ((*it).stripWhiteSpace()) == "decimal-leading-zero" || ((*it).stripWhiteSpace()) == "lower-roman" || ((*it).stripWhiteSpace()) == "upper-roman" || ((*it).stripWhiteSpace()) == "lower-greek" || ((*it).stripWhiteSpace()) == "lower-alpha" || ((*it).stripWhiteSpace()) == "lower-latin" || ((*it).stripWhiteSpace()) == "upper-alpha" || ((*it).stripWhiteSpace()) == "upper-latin" || ((*it).stripWhiteSpace()) == "hebrew" || ((*it).stripWhiteSpace()) == "armenian" || ((*it).stripWhiteSpace()) == "georgian" || ((*it).stripWhiteSpace()) == "cjk-ideographic" || ((*it).stripWhiteSpace()) == "hiragana" || ((*it).stripWhiteSpace()) == "katakana" || ((*it).stripWhiteSpace()) == "hiragana-iroha" || ((*it).stripWhiteSpace()) == "katakana-iroha" || ((*it).stripWhiteSpace()) == "none" || ((*it).stripWhiteSpace()) == "inherit"){
          expandedProps["list-style-type"] = ((*it)) ;
      }      
      else
      if( ((*it).stripWhiteSpace()) == "inside" || ((*it).stripWhiteSpace()) == "outside" || ((*it).stripWhiteSpace()) == "inherit"){
        expandedProps["list-style-position"] = ((*it));
      }
      ++it;  
    }  
  return expandedProps;
}

QMap<QString,QString>  ShorthandFormer::expandOutlineProp( QStringList l){
  QMap<QString,QString> expandedProps;
  QRegExp  lengthPattern("\\d*ex|em|px"),
                    colorPattern("#[\\w\\d]{6}");
  QStringList::Iterator it = l.begin();
     while (  it != l.end() ) { 
      if( ((*it).stripWhiteSpace()) == "none" || 
      ((*it).stripWhiteSpace()) == "hidden" || 
      ((*it).stripWhiteSpace()) == "dotted" || 
      ((*it).stripWhiteSpace()) == "dashed" || 
      ((*it).stripWhiteSpace()) == "solid" || 
      ((*it).stripWhiteSpace()) == "double" || 
      ((*it).stripWhiteSpace()) == "groove" ||
      ((*it).stripWhiteSpace()) == "ridge" ||
      ((*it).stripWhiteSpace()) == "inset" ||
      ((*it).stripWhiteSpace()) == "outset" ||    
      ((*it).stripWhiteSpace()) == "inherit" ){
        expandedProps["outline-style"] = ((*it));
      }       
      else
      if(  (*it).contains(colorPattern) || HTMLColorList.contains((*it))!=0 || ((*it).stripWhiteSpace()) == "invert" || ((*it).stripWhiteSpace()) == "inherit"){
          expandedProps["outline-color"] = ((*it)) ;
      }      
      else
      if( (*it).contains(lengthPattern) || ((*it).stripWhiteSpace()) == "thin" || ((*it).stripWhiteSpace()) == "medium" || ((*it).stripWhiteSpace()) == "thick" || ((*it).stripWhiteSpace()) == "inherit"){
        expandedProps["outline-width"] = ((*it));
      }
      ++it;  
    }  
  return expandedProps;
}

QMap<QString,QString>  ShorthandFormer::expandBoxSide(QString subPropName, QStringList l){
  QMap<QString,QString> expandedProps;
  QRegExp  lengthPattern("\\d*ex|em|px"),
                    colorPattern("#[\\w\\d]{6}");
  QStringList::Iterator it = l.begin();                  
     while (  it != l.end() ) { 
      if( ((*it).stripWhiteSpace()) == "none" || 
      ((*it).stripWhiteSpace()) == "hidden" || 
      ((*it).stripWhiteSpace()) == "dotted" || 
      ((*it).stripWhiteSpace()) == "dashed" || 
      ((*it).stripWhiteSpace()) == "solid" || 
      ((*it).stripWhiteSpace()) == "double" || 
      ((*it).stripWhiteSpace()) == "groove" ||
      ((*it).stripWhiteSpace()) == "ridge" ||
      ((*it).stripWhiteSpace()) == "inset" ||
      ((*it).stripWhiteSpace()) == "outset" ||    
      ((*it).stripWhiteSpace()) == "inherit" ){
        expandedProps[subPropName + "-style"] = ((*it));
      }       
      else
      if(  (*it).contains(colorPattern) || HTMLColorList.contains((*it))!=0 || ((*it).stripWhiteSpace()) == "transparent" || ((*it).stripWhiteSpace()) == "inherit"){
          expandedProps[subPropName + "-color"] = ((*it)) ;
      }      
      else
      if( (*it).contains(lengthPattern) || ((*it).stripWhiteSpace()) == "thin" || ((*it).stripWhiteSpace()) == "medium" || ((*it).stripWhiteSpace()) == "thick" || ((*it).stripWhiteSpace()) == "inherit"){
        expandedProps[subPropName + "-width"] = ((*it));
      }
      ++it;  
    }  
  return expandedProps;
}

QMap<QString,QString>  ShorthandFormer::expandBorderProp(QStringList l){
  QMap<QString,QString> expandedProps;
  QRegExp   lengthPattern("\\d*ex|em|px"),
                    colorPattern("#[\\w\\d]{6}");
  QStringList::Iterator it = l.begin();                  
     while (  it != l.end() ) { 
      if( ((*it).stripWhiteSpace()) == "none" || 
      ((*it).stripWhiteSpace()) == "hidden" || 
      ((*it).stripWhiteSpace()) == "dotted" || 
      ((*it).stripWhiteSpace()) == "dashed" || 
      ((*it).stripWhiteSpace()) == "solid" || 
      ((*it).stripWhiteSpace()) == "double" || 
      ((*it).stripWhiteSpace()) == "groove" ||
      ((*it).stripWhiteSpace()) == "ridge" ||
      ((*it).stripWhiteSpace()) == "inset" ||
      ((*it).stripWhiteSpace()) == "outset" ||    
      ((*it).stripWhiteSpace()) == "inherit" ){
        expandedProps["border-top-style"] = ((*it));
        expandedProps["border-left-style"] = ((*it));
        expandedProps["border-right-style"] = ((*it));
        expandedProps["border-bottom-style"] = ((*it));
      }       
      else
      if(  (*it).contains(colorPattern) || HTMLColorList.contains((*it))!=0 || ((*it).stripWhiteSpace()) == "transparent" || ((*it).stripWhiteSpace()) == "inherit"){
          expandedProps["border-top-color"] = ((*it));
          expandedProps["border-left-color"] = ((*it));
          expandedProps["border-right-color"] = ((*it));
          expandedProps["border-bottom-color"] = ((*it));
      }      
      else
      if( (*it).contains(lengthPattern) || ((*it).stripWhiteSpace()) == "thin" || ((*it).stripWhiteSpace()) == "medium" || ((*it).stripWhiteSpace()) == "thick" || ((*it).stripWhiteSpace()) == "inherit"){
        expandedProps["border-top-width"] = ((*it));
        expandedProps["border-left-width"] = ((*it));
        expandedProps["border-right-width"] = ((*it));
        expandedProps["border-bottom-width"] = ((*it));
      }
      ++it;  
    }  
  return expandedProps;
}


