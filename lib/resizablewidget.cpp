/* This file is part of the KDE project
   Copyright (C) 2003 Alexander Dymo <cloudtemple@mksat.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
#include "resizablewidget.h"

#include <qevent.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qwhatsthis.h>

#include <klocale.h>

static const char * resize_xpm[] = {
"9 18 2 1",
" 	c None",
".	c #000000",
"    .    ",
"         ",
"    .    ",
"         ",
"    .    ",
"         ",
"  . . .  ",
" ..   .. ",
".........",
" ..   .. ",
"  . . .  ",
"         ",
"    .    ",
"         ",
"    .    ",
"         ",
"    .    ",
"         "};

ResizableWidget::ResizableWidget(QWidget *view, QWidget *parent):
    QWidget(parent), m_sizer(0), m_widget(view)
{
    QHBoxLayout *l = new QHBoxLayout(this);
    view->setParent(this);
    l->addWidget(view);

    m_sizer = new MyPushButton(this);
    m_sizer->setIcon(QIcon(resize_xpm));
    m_sizer->setWhatsThis(i18n("Drag this to resize the widget."));
    l->addWidget(m_sizer);
}

void MyPushButton::mousePressEvent( QMouseEvent * e )
{
    m_resizing = true;
    m_pressedPos = e->globalPos();
    m_width = m_widget->m_widget->width();
    QPushButton::mousePressEvent(e);
}

void MyPushButton::mouseReleaseEvent( QMouseEvent * e )
{
    m_resizing = false;
    QPushButton::mouseReleaseEvent(e);
}

void MyPushButton::mouseMoveEvent( QMouseEvent * e )
{
    if (m_resizing)
        m_widget->m_widget->setMinimumWidth(m_width + e->globalPos().x() - pressedPos().x());

    QPushButton::mouseMoveEvent(e);
}

MyPushButton::MyPushButton( ResizableWidget * parent )
    :QPushButton(parent), m_resizing(false), m_widget(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setFlat(true);
    m_width = m_widget->m_widget->width();
}


#include "resizablewidget.moc"
