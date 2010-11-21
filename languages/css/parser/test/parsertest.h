/*
   This file is part of KDevelop
   Copyright 2009 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef CSS_PARSERTEST_H
#define CSS_PARSERTEST_H

#include <QObject>

#include <duchain/tests/duchaintestbase.h>

namespace Css
{

class TestParser : public DUChainTestBase
{
    Q_OBJECT

private Q_SLOTS:
    void parser_data();
    void parser();

    void multiline();

    void htmlStyleElement();
    void htmlInlineStyle();
    void htmlInlineStyle2();
    void htmlInlineStyle3();

};

}

#endif
