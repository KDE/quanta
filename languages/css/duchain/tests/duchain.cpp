/* This file is part of KDevelop
    Copyright 2010 Niko Sams <niko.sams@gmail.com>

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

#include "duchain.h"

#include <QtTest/QtTest>

#include <language/duchain/parsingenvironment.h>
#include <language/duchain/duchain.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/declaration.h>

QTEST_MAIN(Css::TestDUChain)

namespace Css
{

TestDUChain::TestDUChain()
{
}

void TestDUChain::testContext()
{
    //                 01234567890123456
    QByteArray method("a { color: red; }");

    KDevelop::TopDUContext* top = parse(method, DumpAll);
    DUChainReleaser releaseTop(top);
    KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());

    QCOMPARE(top->childContexts().count(), 1);
    QVERIFY(top->childContexts().first()->range() == KDevelop::RangeInRevision(0, 3, 0, 16));
    KDevelop::Declaration *dec = top->findDeclarationAt(KDevelop::CursorInRevision(0, 0));
    QCOMPARE(dec->qualifiedIdentifier().toString(), QString("a ")); //TODO: this should not include the space
    QVERIFY(dec->range() == KDevelop::RangeInRevision(0, 0, 0, 2)); 
}

void TestDUChain::testPseudoClass()
{
    //                 0123456789012345678901234
    QByteArray method("a:active { color: red; }");

    KDevelop::TopDUContext* top = parse(method, DumpAll);
    DUChainReleaser releaseTop(top);
    KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());

    QCOMPARE(top->childContexts().count(), 1);
    QVERIFY(top->childContexts().first()->range() == KDevelop::RangeInRevision(0, 10, 0, 23));
    KDevelop::Declaration *dec = top->findDeclarationAt(KDevelop::CursorInRevision(0, 0));
    QVERIFY(dec->range() == KDevelop::RangeInRevision(0, 0, 0, 9));
    QCOMPARE(dec->qualifiedIdentifier().toString(), QString("a:active ")); //TODO: this should not include the space
}


}

#include "duchain.moc"
