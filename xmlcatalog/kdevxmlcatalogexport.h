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

#ifndef KDEVXMLCATALOGEXPORT_H
#define KDEVXMLCATALOGEXPORT_H

/* needed for KDE_EXPORT macros */
#include <kdemacros.h>

#ifndef KDEVXMLCATALOG_EXPORT
# ifdef MAKE_KDEVXMLCATALOG_LIB
#  define KDEVXMLCATALOG_EXPORT KDE_EXPORT
# else
#  define KDEVXMLCATALOG_EXPORT KDE_IMPORT
# endif
#endif


#endif //KDEVXMLCATALOGEXPORT_H

