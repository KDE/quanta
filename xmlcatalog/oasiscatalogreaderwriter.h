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


#ifndef OASISCATALOGREADER_H
#define OASISCATALOGREADER_H

#include "icatalogreaderwriter.h"

#include <QtCore/QString>

class OASISCatalogContainer;
class KDEVXMLCATALOG_EXPORT OASISCatalogReaderWriter : public ICatalogReaderWriter {
public:
    OASISCatalogReaderWriter();
    virtual ~OASISCatalogReaderWriter();
    virtual bool accepts ( const QString& file ) const;
    virtual bool accepts ( const QDomDocument& doc ) const;
    virtual bool accepts ( const ICatalog * catalog ) const;
    
    virtual ICatalog* readCatalog ( const QString& file ) const;
    /** Parses the dom document.
     *  @note The there will be no base path set.
     */
    virtual ICatalog* readCatalog ( const QDomDocument& doc) const;

    virtual bool writeCatalog(const ICatalog* catalog, const QString& file) const;
    
protected:
    virtual void readNode ( const QDomNode& node,  OASISCatalogContainer* c) const;
    
private:
    //Entities enumeration
    struct e;
    //Attributes enumeration
    struct a;
};

#endif // OASISCATALOGREADER_H
