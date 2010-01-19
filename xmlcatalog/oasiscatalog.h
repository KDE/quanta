/*
Copyright (C) 2009  Ruan Strydom <ruan@jcell.co.za>

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

/*
TODO This is a bad implementation of the oasis catalog and needs reviewing.
*/


#ifndef OASISCATALOG_H
#define OASISCATALOG_H

#include "icatalog.h"
#include "icatalogentry.h"
#include "icataloggroup.h"

#include <QtXml/QDomDocument>

class OASISCatalogReaderWriter;
class OASISCatalogContainer;
class OASISCatalogGroup;
class OASISCatalog;


/** Used when reading writing the catalog.
 * The catalog should always start with an existing document and
 * the reader writer should set the elements.
 * If a new entry is added to the catalog a new dom-element should be added to the parent dom-node.
 * also when properties are set the relevant attributes or values should be set.
 * This is so that the xml format and elements stay the same as the user would expect it.
 */
class IOASISCatalogDomNode {
public:
    IOASISCatalogDomNode();
    virtual ~IOASISCatalogDomNode();

    virtual QDomElement element() const {
        return m_element;
    }
protected:
    QDomElement m_element;
    virtual void setElement( QDomElement e ) {
        m_element = e;
    }
};


/** An entry in the catalog or group.
 */
class OASISCatalogEntry : public ICatalogEntry, public IOASISCatalogDomNode {
    friend class OASISCatalogReaderWriter;
    friend class OASISCatalogContainer;
public:
    static const QString ParameterUrl;
    static const QString ParameterBase;
    static const QString ParameterId;

    virtual QString systemId() const {
        return m_systemId;
    }

    virtual QString publicId() const {
        return m_publicId;
    }

    virtual ICatalogGroup* group() const;

    virtual ICatalog* catalog() const;

    virtual OASISCatalogContainer* parent() const {
        return m_parent;
    }

    virtual const QString & base() const;


    virtual QVariant parameter(const QString& key) const;

    virtual void setParameter(const QString& key, const QVariant& value);


    virtual void setPublicId( const QString& pi ) {
        m_publicId = pi;
    }

    virtual void setSystemId( const QString& si ) {
        m_systemId = si;
    }

    virtual const QString& id() const {
        return m_id;
    }

    virtual void setId( const QString& i ) {
        m_id = i;
        if (!m_element.isNull())
            m_element.setAttribute(ParameterId, i);
    }

    virtual QString URI() const {
        return m_uri;
    }

    virtual void setURI( const QString& u ) {
        m_uri = u;
    }

    virtual QString URL() const {
        return m_url;
    }

    virtual void setURL( const QString& u ) {
        m_url = u;
        if (!m_element.isNull())
            m_element.setAttribute(ParameterUrl, u);
    }

    virtual void setBase( const QString& b ) {
        m_base = b;
        if (!m_element.isNull())
            m_element.setAttribute(ParameterBase, b);
    }

    virtual void setParent( OASISCatalogContainer* p ) {
        m_parent = p;
    }
protected:
    QString m_publicId;
    QString m_systemId;
    QString m_uri;
    QString m_url;
    QString m_id;
    QString m_base;
    OASISCatalogContainer * m_parent;
    explicit OASISCatalogEntry();
    virtual ~OASISCatalogEntry();
};




/** A container as base for group and catalog.
 * <a href="http://www.oasis-open.org/committees/download.php/14809/xml-catalogs.html">See the OASIS Xml Catalogs specification.<a>
 * <p>
 * <b>Schema namespaces (The same as Apache Xerces):<b>
 * This will first try to lookup the schema in the catalog by its target namespace
 * if it has one using the catalog's uri  entries. If the schema has no target namespace,
 * or the namespace is unavailable or no mapping for the namespace could be found the resolver
 * will then try to locate the schema using any location hints provided. These location hints
 * are interpreted to be system identifiers.
 * </p>
 * The OASIS catalog group can have most of the same elements as a catalog, it may also delegate to other catalogs.
 */
class OASISCatalogContainer : public ICatalog , public IOASISCatalogDomNode {
    friend class OASISCatalogReaderWriter;
public:
    static const QString PreferSystem;
    static const QString PreferPublic;
    static const QString ParameterBase;
    static const QString ParameterId;
    static const QString ParameterPrefer;

    virtual const QString & id() const {
        return m_id;
    }

    /** The base URI.
     *  If this is empty it will return the parents base.
     */
    virtual const QString & base() const;

    /** OASISCatalogContainer::PreferSystem or OASISCatalogContainer::PreferPublic.
     *  If this is empty it will return the parents preference.
     *  @note <a href="http://www.oasis-open.org/committees/download.php/14809/xml-catalogs.html">See the OASIS Xml Catalogs specification.<a>
     */
    virtual const QString & prefer() const;

    /** Returns the URI mapping in the catalog for the given external identifier or QString::null if no mapping exists.
     *  If the URI reference is an URN in the publicid namespace it is
     *  converted into a public identifier by URN "unwrapping" as specified in the XML Catalogs specification
     *  and then resolution is performed.
     *  @return Returns a URL to the DTD or Schema or QString::null if unsuccessfull.
     */
    virtual QString resolveSystemId ( const QString& systemId ) const;

    /** Returns the URI mapping in the catalog for the given external identifier or null if no mapping exists.
     *  Public identifiers are normalized before comparison. If the URI reference is an URN in the publicid namespace it is
     *  converted into a public identifier by URN "unwrapping" as specified in the XML Catalogs specification
     *  and then resolution is performed.
     *  @return Returns a URL to the DTD or Schema or QString::null if unsuccessfull.
     */
    virtual QString resolvePublicId ( const QString& publicId ) const;

    /** Returns the URI mapping in the catalog for the given URI reference or null if no mapping exists.
     *  URI comparison is case sensitive.
     *  @return The resolved URI or QString::null if unsuccessfull.
     */
    virtual QString resolveUri ( const QString& uri ) const;

    /** Resolves either a publicId or a systemId based on preference.
     *  @see OASISCatalogContainer::prefer()
     *  @return Returns a URL to the DTD or Schema or QString::null if unsuccessfull.
     */
    virtual QString resolve ( const QString& publicId, const QString& systemId ) const;

    /** Return the applicable SYSTEM system identifier.
     * <p>If a SYSTEM entry exists in the Catalog
     * for the system ID specified, return the mapped value.</p>
     * @return QString::null if unsuccesfull.
     */
    virtual QString rewriteSystemId ( const QString &systemId ) const;

    /**
     * @return QString::null if unsuccesfull.
     */
    virtual QString rewriteUri ( const QString &uri ) const;

    virtual QList< ICatalogGroup* > groups() const;

    virtual QList< ICatalogEntry* > entries() const;

    virtual OASISCatalogContainer* parent() const {
        return m_parent;
    }

    virtual QVariant parameter(const QString& key) const;

    virtual void setParameter(const QString& key, const QVariant& value);

    virtual ICatalogEntry* addPublicEntry(const QString& publicId, const QString& url, const QHash< QString, QVariant >& parameters);
    
    virtual ICatalogEntry* addSystemEntry(const QString& systemId, const QString& url, const QHash< QString, QVariant >& parameters);

    virtual ICatalogEntry* addUriEntry(const QString& uri, const QString& url, const QHash< QString, QVariant >& parameters);

    virtual void removeEntry(const ICatalogEntry* entry);

    virtual void setId( const QString& i ) {
        m_id = i;
        if (!m_element.isNull())
            m_element.setAttribute(ParameterId, i);
    }

    virtual void setBase( const QString& b ) {
        m_base = b;
        if (!m_element.isNull())
            m_element.setAttribute(ParameterBase, b);
    }

    virtual void setPrefer( const QString& p ) {
        m_prefer = p;
        if (!m_element.isNull())
            m_element.setAttribute(ParameterPrefer, p);
    }
protected:

    /**Used to rewrite a string or replace part of it.
    *  Usage and meaning depends on its context.
    *  These should probably go into their own respective classes. */
    typedef struct OASISRewrite {
public:
        QString id;
        QString part;
        QString replacement;
        QString base;
        QDomElement element;
    } OASISRewrite;

    /**Used to delegate to another catalog.
    *  Usage and meaning depends on its context.
    *  These should probably go into their own respective classes.*/
    typedef struct OASISDelegate {
public:
        QString id;
        QString part;
        QString catalog;
        QString base;
        QDomElement element;
    } OASISDelegate;

    QString m_id;
    QString m_base;
    QString m_prefer;

    QList< OASISCatalogGroup* > m_groups;
    QList< OASISCatalogEntry* > m_entries;
    QList<OASISRewrite> m_rewriteSystem;
    QList<OASISRewrite> m_systemSuffix;
    QList<OASISRewrite> m_rewriteUri;
    QList<OASISRewrite> m_uriSuffix;
    QList<OASISDelegate> m_delegatePublic;
    QList<OASISDelegate> m_delegateSystem;
    QList<OASISDelegate> m_delegateUri;

    OASISCatalogContainer * m_parent;
    //These are the nextCatalog entries;
    OASISCatalogContainer * m_catalogs;

    explicit OASISCatalogContainer();
    virtual ~OASISCatalogContainer();
};


/** The catalog group.
 *  @see OASISCatalogContainer
 */
class OASISCatalogGroup : public OASISCatalogContainer, public ICatalogGroup {
    friend class OASISCatalogReaderWriter;
    friend class OASISCatalogContainer;
public:
    virtual ICatalog* catalog() const;

    virtual QList< ICatalogEntry* > entries() const {
        return OASISCatalogContainer::entries();
    }

    virtual ICatalogEntry* addPublicEntry(const QString& publicId, const QString& url, const QHash< QString, QVariant >& parameters) {
        return OASISCatalogContainer::addPublicEntry(publicId, url, parameters);
    }
    
    virtual ICatalogEntry* addSystemEntry(const QString& systemId, const QString& url, const QHash< QString, QVariant >& parameters) {
        return OASISCatalogContainer::addSystemEntry(systemId, url, parameters);
    }

virtual ICatalogEntry* addUriEntry(const QString& uri, const QString& url, const QHash< QString, QVariant >& parameters) {
        return OASISCatalogContainer::addUriEntry(uri, url, parameters);
    }

    virtual void removeEntry(const ICatalogEntry* entry) {
        OASISCatalogContainer::removeEntry(entry);
    }

protected:
    explicit OASISCatalogGroup();
    virtual ~OASISCatalogGroup();
};



/** An OASIS XML Catalog implementation.
 * @see OASISCatalogContainer
 */
class OASISCatalog : public OASISCatalogContainer {
    friend class OASISCatalogReaderWriter;
public:
    OASISCatalog();
    virtual ~OASISCatalog();
    virtual QString resolve ( const QString& publicId, const QString& systemId ) const;
    virtual QString resolveUri ( const QString& uri ) const;
    virtual QString resolveSystemId ( const QString& systemId ) const;
    virtual QString resolvePublicId ( const QString& publicId ) const;
protected:
    QString m_file;
    QDomDocument m_doc;
};

#endif // OASISCATALOG_H
