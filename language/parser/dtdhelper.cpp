/*****************************************************************************
 * Copyright (c) 2010 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#include "dtdhelper.h"

#include <QtCore/QHash>
#include <QtCore/QString>
#include <xmlcatalog/cataloghelper.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/duchain.h>
#include <language/duchain/duchainlock.h>
#include <interfaces/icore.h>
#include <interfaces/iruncontroller.h>
#include <interfaces/ilanguagecontroller.h>
#include <interfaces/ilanguage.h>
#include <language/interfaces/ilanguagesupport.h>
#include <language/backgroundparser/parsejob.h>
#include <QThread>
#include <threadweaver/WeaverInterface.h>
#include <language/backgroundparser/backgroundparser.h>
#include <threadweaver/ThreadWeaver.h>
#include <xmlcatalog/publicid.h>
#include <language/duchain/declaration.h>

using namespace Xml;
using namespace KDevelop;

DtdHelper::DtdHelper() : AbstractDtdHelper() {}

DtdHelper::~DtdHelper() {}

DtdHelper::DtdHelper(DtdHelperData* helper): AbstractDtdHelper(), d(helper) {}


class DUChainDTDHelper : public DtdHelperData {
public:
    DUChainDTDHelper() : DtdHelperData() {};
    DUChainDTDHelper(IndexedTopDUContext top) : DtdHelperData() {
        topContext = top;
    }

    virtual ~DUChainDTDHelper() {}

    virtual bool isNull() const {return !topContext.isValid();}

    virtual bool cdataElement(const QString& elementName) const {
        if(isNull()) return false;
        DUChainReadLocker lock;
        QList<Declaration *> list = topContext.data()->findDeclarations(QualifiedIdentifier(elementName), SimpleCursor::invalid(), AbstractType::Ptr() );
        foreach(Declaration * dec, list) {
            if(!dec) continue;
            //TODO return something
        }
        return false;
    }
    virtual bool closeOptional(const QString& elementName) const {
        if(isNull()) return false;
        DUChainReadLocker lock;
        QList<Declaration *> list = topContext.data()->findDeclarations(QualifiedIdentifier(elementName), SimpleCursor::invalid(), AbstractType::Ptr() );
        foreach(Declaration * dec, list) {
            if(!dec) continue;
            //TODO return something
        }
        return false;
    }

    virtual bool emptyElement(const QString& elementName) const {
        if(isNull()) return false;
        DUChainReadLocker lock;
        QList<Declaration *> list = topContext.data()->findDeclarations(QualifiedIdentifier(elementName), SimpleCursor::invalid(), AbstractType::Ptr() );
        foreach(Declaration * dec, list) {
            if(!dec) continue;
            //TODO return something
        }
        return false;
    }

    virtual bool hasChild(const QString& elementName, const QString& child) const {
        if(isNull()) return true;
        DUChainReadLocker lock;
        QList<Declaration *> list = topContext.data()->findDeclarations(QualifiedIdentifier(elementName), SimpleCursor::invalid(), AbstractType::Ptr());
        foreach(Declaration * dec, list) {
            if(!dec) continue;
            if(dec->kind() == Declaration::Type && dec->internalContext()) {
                QList<Declaration *> children = dec->internalContext()->findDeclarations(QualifiedIdentifier(child), SimpleCursor::invalid(), AbstractType::Ptr());
                if(children.size() > 0)
                    return true;
                else
                    return false;
            }
        }
        return true;
    }

protected:
    IndexedTopDUContext topContext;
};

//<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
class Html4LooseHelper : public DtdHelperData {
public:
    Html4LooseHelper() : DtdHelperData() {
    }
    virtual ~Html4LooseHelper() {}
    
    virtual bool isNull() const {return false;}

    virtual bool cdataElement(const QString& elementName) const {
        static QHash<QString, char> list = ( {
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
        if (list.contains(elementName.toUpper())) {
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
    virtual ~Html4StrictHelper() {}
};

//<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN" "http://www.w3.org/TR/html4/frameset.dtd">
class Html4FrameHelper : public Html4LooseHelper {
public:
    Html4FrameHelper() : Html4LooseHelper() {
    }
    virtual ~Html4FrameHelper() {}
};


class XmlHelper : public DtdHelperData {
public:
    XmlHelper() : DtdHelperData() {
    }
    virtual ~XmlHelper() {}

    virtual bool cdataElement(const QString& elementName) const {
        Q_UNUSED(elementName);
        //All cdata must be in <![CDATA[
        return false;
    }

    virtual bool closeOptional(const QString& elementName) const {
        Q_UNUSED(elementName);
        //Does not exist in XML
        return false;
    }

    virtual bool emptyElement(const QString& elementName) const {
        Q_UNUSED(elementName);
        //Safe to asume false
        return false;
    }

    virtual bool hasChild(const QString& elementName, const QString& child) const {
        Q_UNUSED(elementName);
        Q_UNUSED(child);
        //Should never be called because xml have no closeOptional elements.
        return true;
    }
    virtual bool isNull() const {return false;}
};

static DtdHelper html4LooseHelper(new Html4LooseHelper);
static DtdHelper html4StrictHelper(new Html4StrictHelper);
static DtdHelper html4FrameHelper(new Html4FrameHelper);
static DtdHelper xmlHelper(new XmlHelper);

DtdHelper DtdHelper::instance(const QString& publicId, const QString& systemId, const QString& URI, const QString& doctype, const KMimeType::Ptr mime, const KUrl& path)
{
    QString pubId = PublicId::decodeURN(publicId);

    if ((!pubId.isNull() && pubId == "-//W3C//DTD HTML 4.01//EN") ||
        (!pubId.isNull() && pubId == "-//W3C//DTD HTML 4.0//EN") ||
            (!systemId.isNull() && systemId == "http://www.w3.org/TR/html4/strict.dtd") ||
            (!systemId.isNull() && systemId == "http://www.w3.org/TR/REC-html40/strict.dtd")) {
        return html4StrictHelper;
    }

    if ((!pubId.isNull() && pubId == "-//W3C//DTD HTML 4.01 Transitional//EN") ||
            (!systemId.isNull() && systemId == "http://www.w3.org/TR/html4/loose.dtd")) {
        return html4LooseHelper;
    }

    if ((!pubId.isNull() && pubId == "-//W3C//DTD HTML 4.01 Frameset//EN") ||
            (!systemId.isNull() && systemId == "http://www.w3.org/TR/html4/frameset.dtd")) {
        return html4FrameHelper;
    }
    
    DtdHelper helper = instanceForName(doctype);
    if(!helper.isNull())
        return helper;
    
    helper = instanceForMime(mime);
    if(!helper.isNull())
        return helper;

    //Create a duchain helper
    /*
    KUrl url = CatalogHelper::resolve(pubId, systemId, URI, doctype, mime, path);
    if(url.isValid()) {
        IndexedTopDUContext itop;
        {
            DUChainReadLocker lock;
            TopDUContext * top = DUChain::self()->chainForDocument(url);
            if(top)
                itop = top->indexed();
        }
        if(!itop.isValid()) {
            ILanguage *lang = ICore::self()->languageController()->language("Sgml");
            if(lang && lang->languageSupport()) {
                ParseJob * job = lang->languageSupport()->createParseJob(url);
                ThreadWeaver::Weaver::instance()->enqueue(job);
                //TODO Dodgy
                int count = 0;
                while(job && !job->isFinished() && ++count < 9999) {
                    usleep(1000);
                }
                {
                    DUChainReadLocker lock;
                    if(job && job->duChain().data())
                        itop = job->duChain().data()->indexed();
                }
                if(job) job->deleteLater();
            }
        }
        if(itop.isValid()) {
            return DtdHelper(new DUChainDTDHelper(itop));
        }
    }
    */

    return DtdHelper();
}


DtdHelper DtdHelper::instanceForMime(KMimeType::Ptr mime)
{
    if (!mime || !mime->isValid())
        return DtdHelper();
    if (mime->is("application/xml")
            || mime->is("text/xml")
            || mime->is("application/xslt+xml")
            || mime->is("application/wsdl+xml")
            || mime->is("application/x-wsdl")) {
        return xmlHelper;
    }

    if (mime->is("application/xhtml+xml")) {
        return xmlHelper;
    }

    if(mime->is("text/html"));
        return html4LooseHelper;

    return DtdHelper();
}

DtdHelper DtdHelper::instanceForName(const QString& name)
{
    if(name.trimmed().toLower() == "html");
        return html4LooseHelper;

    if (name.trimmed().toLower() == "xml") {
        return xmlHelper;
    }

    return DtdHelper();
}
