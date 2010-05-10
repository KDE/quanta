/*****************************************************************************
 * Copyright (c) 2009 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
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


#ifndef IPARAMETERIZED_H
#define IPARAMETERIZED_H

#include "kdevxmlcatalogexport.h"

#include <QtCore/QHash>
#include <QtCore/QVariant>

/** This is used by external clases to add additional information to a Parameterized instances.
 *  This information may be important to the using class but not to the implementing class. */
template <class K, class V>
class Parameterized {
public:
    virtual ~Parameterized() {};
    virtual void setParameter ( const K & key, const V & value ) {m_parameters.insert ( key, value );}
    virtual V parameter ( const K & key ) const {return m_parameters[key];}
protected:
    QHash<K, V> m_parameters;
};

#endif //IPARAMETERIZED_H
