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

#include "navigationwidget.h"
#include <language/duchain/declaration.h>

using namespace Xml;
using namespace KDevelop;

class DeclarationNavigationContext : public AbstractNavigationContext {
public:
    DeclarationNavigationContext(KDevelop::DeclarationPointer declaration, KDevelop::TopDUContextPointer topContext) {
        m_declaration = declaration;
        m_topContext = topContext;
    }
    QString name() const {
        return m_declaration->qualifiedIdentifier().toString();
    }
protected:
    KDevelop::DeclarationPointer m_declaration;
    KDevelop::TopDUContextPointer m_topContext;
};

NavigationWidget::NavigationWidget(KDevelop::DeclarationPointer declaration, KDevelop::TopDUContextPointer topContext)
{
    m_topContext = topContext;

    initBrowser(100);
    //The first context is registered so it is kept alive by the shared-pointer mechanism
    m_startContext = NavigationContextPointer(new DeclarationNavigationContext(declaration, m_topContext));
    setContext(m_startContext);
}
