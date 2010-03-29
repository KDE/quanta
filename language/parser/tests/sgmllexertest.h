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

#ifndef MLLEXERTEST_H
#define MLLEXERTEST_H

#include <QObject>

namespace Xml
{

class TestSgmlLexer : public QObject
{
    Q_OBJECT

public:
    TestSgmlLexer();

private Q_SLOTS:
    void testDoctype();
    void testElementName();
    void testStringRefList();
    void testReadUntill();
    void testReadUntillAny();
    void testReadWhileAny();
    void testLexer();
    void testLexerForFun();
};
}

#endif
