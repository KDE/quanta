/***************************************************************************
   $Id$

    Copyright (C) 2000 Scott Manson
                       SDManson@alltel.net

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/


#include "syntaxdocument.h"
#include <qfile.h>
#include <kdebug.h>
#include <kstddirs.h>
#include <klocale.h>
#include <kmessagebox.h>

SyntaxDocument::SyntaxDocument() : QDomDocument()
{
    QString syntaxPath = locate("data", "kwrite/syntax.xml");
    if( !syntaxPath.isEmpty() )
    {
        QFile f( syntaxPath );
        if ( f.open(IO_ReadOnly) )
            setContent(&f);
        else
            KMessageBox::error( 0L, i18n("Can't open %1").arg(syntaxPath) );
        f.close();
    } else
        KMessageBox::error( 0L, i18n("File share/apps/kwrite/syntax.xml not found ! Check your installation!") );
}

SyntaxDocument::~SyntaxDocument()
{

}
/*syntax of xml file whitespace is optional
 <language name=XXX>
  <keywords>
   <keyword> XXX </keyword>
  </keywords>
  <types>
  <type XXX </type>
  </types>
</language>

*/
QStringList& SyntaxDocument::finddata(const QString& langName,const QString& type)
{
	QDomElement docElem = documentElement();
	QDomNode n = docElem.firstChild();
		while ( !n.isNull() ) {
			if ( n.isElement()) {
				QDomElement e = n.toElement(); //e.tagName is language
				QDomNode child=e.firstChild(); // child.toElement().tagname() is keywords/types
				QDomNode grandchild=child.firstChild(); // grandchild.tagname is keyword/type

// at this point e.attribute("name") should equal langName
// and we can use type==keyword as an index to child list
				if(e.attribute("name").compare(langName)==0 ) { //&& grandchild.toElement().tagName().compare(type) == 0 ){
					QDomNodeList childlist=n.childNodes();
					QDomNodeList grandchildlist=childlist.item(type=="keyword" ? 0 : 1).childNodes();
					for(uint i=0; i< grandchildlist.count();i++)
						m_data+=grandchildlist.item(i).toElement().text().stripWhiteSpace();
					return m_data;
				}
			}
		n = n.nextSibling();
	}
	return m_data;
}
