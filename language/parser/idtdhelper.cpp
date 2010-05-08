/*
Copyright (C) 2010  Ruan Strydom <ruan@jcell.co.za>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "idtdhelper.h"

#include <QtCore/QHash>
#include <QtCore/QString>

using namespace Xml;

// NOTE this serves as a quick implementation.
// TODO use a dtd parser and a catalog this helper facades should wrap a proper schema/dtd library.
// NOTE See bottom of source for popular dtd's


//<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
class Html4LooseHelper : public IDtdHelper {
public:
    Html4LooseHelper() : IDtdHelper() {
    }
    ~Html4LooseHelper() {}

    virtual bool cdataElement(const QString& elementName) const {
        static QHash<QString, char> list = ({
            QHash<QString, char> list;
            list.insert("SCRIPT", 0);
            list.insert("STYLE", 0);
            list;
        });
        return list.contains(elementName.toUpper());
    }

    virtual bool closeOptional(const QString& elementName) const {
        static QHash<QString, char> list = split("BASEFONT | BR | AREA | LINK | IMG | PARAM | HR | P | DT | DD | LI | INPUT | OPTION | THEAD | TBODY | TFOOT | COLGROUP | COL | TR | TH | TD | FRAME | ISINDEX | BASE | META");
        return list.contains(elementName.toUpper());
    }

    virtual bool emptyElement(const QString& elementName) const {
        static QHash<QString, char> list =  split("BASEFONT | BR | AREA | LINK | IMG | PARAM | HR | INPUT | COL | FRAME | ISINDEX | BASE | META");
        return list.contains(elementName.toUpper());
    }
    
    //close optional and not empty
    virtual bool hasChild(const QString& elementName, const QString& child) const {
        static QHash<QString, QHash<QString, char> > list = ( {
            QHash<QString, QHash<QString, char> > list;

            QHash<QString, char> inlineElements = split("TT | I | B | U | S | STRIKE | BIG | SMALL | EM | STRONG | DFN | CODE | SAMP | KBD | VAR | CITE | ABBR | ACRONYM | A | IMG | APPLET | OBJECT | FONT | BASEFONT | BR | SCRIPT | MAP | Q | SUB | SUP | SPAN | BDO | IFRAME | INPUT | SELECT | TEXTAREA | LABEL | BUTTON | #PCDATA | CDATA");

            QHash<QString, char> flowElements = split("P | H1 | H2 | H3 | H4 | H5 | H6 | UL | OL |  DIR | MENU | PRE | DL | DIV | CENTER | NOSCRIPT | NOFRAMES | BLOCKQUOTE | FORM | ISINDEX | HR | TABLE | FIELDSET | ADDRESS | #PCDATA | CDATA");
            flowElements.unite(inlineElements);
            
            list.insert("P", inlineElements);
            list.insert("DT", inlineElements);
            list.insert("DD", flowElements);
            list.insert("LI", flowElements);
            list.insert("OPTION", split(""));
            list.insert("THEAD", split("TR"));
            list.insert("TBODY", split("TR"));
            list.insert("TFOOT", split("TR"));
            list.insert("COLGROUP", split("COL"));
            list.insert("TR", split("TH | TD"));
            list.insert("TH", flowElements);
            list.insert("TD", flowElements);
            
            list;
        });
        if(list.contains(elementName.toUpper())) {
            return list[elementName.toUpper()].contains(child.toUpper());
        }
        return true;
    }
    
    protected:
        QHash<QString, char> split(const QString &str) const {
            QHash<QString, char> ret;
            QStringList list = str.split("|");
            foreach(QString s, list) {
                ret.insert(s.trimmed(), 0);
            }
            return ret;
        }
};

//<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
class Html4StrictHelper : public Html4LooseHelper {
public:
    Html4StrictHelper() : Html4LooseHelper() {
    }
    ~Html4StrictHelper() {}
};

//<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN" "http://www.w3.org/TR/html4/frameset.dtd">
class Html4FrameHelper : public Html4LooseHelper {
public:
    Html4FrameHelper() : Html4LooseHelper() {
    }
    ~Html4FrameHelper() {}
};


class XmlHelper : public IDtdHelper {
public:
    XmlHelper() : IDtdHelper() {
    }
    ~XmlHelper() {}

    bool cdataElement(const QString& elementName) const {
        Q_UNUSED(elementName);
        //All cdata must be in <![CDATA[
        return false;
    }

    bool closeOptional(const QString& elementName) const {
        Q_UNUSED(elementName);
        //Does not exist in XML
        return false;
    }

    bool emptyElement(const QString& elementName) const {
        Q_UNUSED(elementName);
        //Safe to asume false
        return false;
    }
    
    bool hasChild(const QString& elementName, const QString& child) const {
        Q_UNUSED(elementName); Q_UNUSED(child);
        //Should never be called because xml have no closeOptional elements.
        return true;
    }
};

IDtdHelper::IDtdHelper()
{

}

IDtdHelper::~IDtdHelper()
{

}

static const Html4LooseHelper * html4LooseHelper = new Html4LooseHelper;
static const Html4StrictHelper * html4StrictHelper = new Html4StrictHelper;
static const Html4FrameHelper * html4FrameHelper = new Html4FrameHelper;
static const XmlHelper * xmlHelper = new XmlHelper;

const IDtdHelper * IDtdHelper::instance(const QString& publicId, const QString& systemId)
{
    //TODO other popular dtd's goes here.

    //TODO xml project has a public id encoder decoder

    if ((!publicId.isNull() && publicId == "-//W3C//DTD HTML 4.01//EN") ||
            (!publicId.isNull() && publicId == "-//W3C//DTD HTML 4.0//EN") ||
            (!systemId.isNull() && systemId == "http://www.w3.org/TR/html4/strict.dtd") ||
            (!systemId.isNull() && systemId == "http://www.w3.org/TR/REC-html40/strict.dtd")) {
        return html4StrictHelper;
    }

    if ((!publicId.isNull() && publicId == "-//W3C//DTD HTML 4.01 Transitional//EN") ||
            (!systemId.isNull() && systemId == "http://www.w3.org/TR/html4/loose.dtd")) {
        return html4LooseHelper;
    }

    if ((!publicId.isNull() && publicId == "-//W3C//DTD HTML 4.01 Frameset//EN") ||
            (!systemId.isNull() && systemId == "http://www.w3.org/TR/html4/frameset.dtd")) {
        return html4FrameHelper;
    }

    //xhmlt

    return 0;
}


const IDtdHelper* IDtdHelper::instanceForMime(KMimeType::Ptr mime)
{
    if (!mime || !mime->isValid())
        return 0;
    if (mime->is("application/xml")
            || mime->is("text/xml")
            || mime->is("application/xslt+xml")
            || mime->is("application/wsdl+xml")
            || mime->is("application/x-wsdl")) {
        return xmlHelper;
    }

    if (mime->is("text/html")) {
        return html4LooseHelper;
    }
    
    if (mime->is("application/xhtml+xml")) {
        return xmlHelper;
    }
    
    return 0;
}

const Xml::IDtdHelper* IDtdHelper::instanceForName(const QString& name)
{
    if(name.trimmed().toLower() == "html") {
        return html4LooseHelper;
    }
    
    if(name.trimmed().toLower() == "xml") {
        return html4LooseHelper;
    }
    
    return 0;
}



/*
(X)HTML Doctype Declarations List

HTML 4.01

Strict

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
"http://www.w3.org/TR/html4/strict.dtd">

Transitional

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">

Frameset

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN"
"http://www.w3.org/TR/html4/frameset.dtd">


XHTML 1.0

Strict

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

Transitional

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

Frameset

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">

XHTML 1.1 - DTD:

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">


XHTML Basic 1.1 - DTD:

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.1//EN"
"http://www.w3.org/TR/xhtml-basic/xhtml-basic11.dtd">


HTML 5 [NOT a standard yet]

<!DOCTYPE HTML>

MathML Doctype Declarations

MathML 2.0 - DTD:

<!DOCTYPE math PUBLIC "-//W3C//DTD MathML 2.0//EN"
"http://www.w3.org/TR/MathML2/dtd/mathml2.dtd">


MathML 1.01 - DTD:

<!DOCTYPE math SYSTEM
"http://www.w3.org/Math/DTD/mathml1/mathml.dtd">


Compound documents doctype declarations

XHTML + MathML + SVG - DTD:

<!DOCTYPE html PUBLIC
"-//W3C//DTD XHTML 1.1 plus MathML 2.0 plus SVG 1.1//EN"
"http://www.w3.org/2002/04/xhtml-math-svg/xhtml-math-svg.dtd">


XHTML + MathML + SVG Profile (XHTML as the host language) - DTD:

<!DOCTYPE html PUBLIC
"-//W3C//DTD XHTML 1.1 plus MathML 2.0 plus SVG 1.1//EN"
"http://www.w3.org/2002/04/xhtml-math-svg/xhtml-math-svg.dtd">


XHTML + MathML + SVG Profile (Using SVG as the host) - DTD:

<!DOCTYPE svg:svg PUBLIC
"-//W3C//DTD XHTML 1.1 plus MathML 2.0 plus SVG 1.1//EN"
"http://www.w3.org/2002/04/xhtml-math-svg/xhtml-math-svg.dtd">


Optional doctype declarations

Beyond the specificities of (X)HTML processing, Doctype declarations in XML languages are only useful to declare named entities and to facilitate the validation of documents based on DTDs. This means that in many XML languages, doctype declarations are not necessarily useful.

The list below is provided only if you actually need to declare a doctype for these types of documents.

SVG 1.1 Full - DTD:

<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">


SVG 1.0 - DTD:

<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.0//EN"
"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd">


SVG 1.1 Basic - DTD:

<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1 Basic//EN"
"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11-basic.dtd">


SVG 1.1 Tiny - DTD:

<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1 Tiny//EN"
"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11-tiny.dtd">


Historical doctype declarations

The doctype declarations below are valid, but have mostly an historical value — a doctype declaration of a more recent equivalent ought to be used in their stead.

HTML 2.0 - DTD:

<!DOCTYPE html PUBLIC "-//IETF//DTD HTML 2.0//EN">

HTML 3.2 - DTD:

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">

XHTML Basic 1.0 - DTD:

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML Basic 1.0//EN"
"http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd">

*/
