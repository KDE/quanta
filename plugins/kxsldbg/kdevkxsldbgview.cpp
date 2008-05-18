/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2008 Keith Isdale <keith@kdewebdev.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#include "kdevkxsldbgview.h"
#include "kdevkxsldbgplugin.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <klocale.h>

KDevKXSLDbgView::KDevKXSLDbgView(QWidget *parent, KDevKXSLDbgPlugin *thePlugin)
    :QWidget(parent), myplugin(thePlugin) 
{
    QVBoxLayout *frame = new QVBoxLayout();
    setLayout(frame);

    QHBoxLayout *h = new QHBoxLayout();
    frame->addLayout(h);

    newXPath = new QLineEdit(this);
    frame->setStretchFactor(newXPath,5);
    h->addWidget(newXPath);
    xPathBtn = new QPushButton(i18n("Goto XPath"), this);
    h->addWidget(xPathBtn);

    h = new QHBoxLayout();
    frame->addLayout(h);

    newEvaluate = new QLineEdit(this);
    h->addWidget(newEvaluate);
    evaluateBtn = new QPushButton(i18n("Evaluate"), this);
    h->addWidget(evaluateBtn);
    connect( xPathBtn, SIGNAL( clicked() ),
            this, SLOT( slotGotoXPath() ) );
    connect( evaluateBtn, SIGNAL( clicked() ),
            this, SLOT( slotEvaluate() ) );
    //frame->setMinimumWidth(400);
    //frame->setMinimumHeight(80);
}

KDevKXSLDbgView::~KDevKXSLDbgView()
{

}
void KDevKXSLDbgView::slotGotoXPath()
{
    myplugin->slotGotoXPath(newXPath->text());
}


void KDevKXSLDbgView::slotEvaluate()
{
    myplugin->slotEvaluate(newEvaluate->text());
}


#include "kdevkxsldbgview.moc"
