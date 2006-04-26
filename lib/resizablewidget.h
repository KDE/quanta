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
#ifndef RESIZABLEWIDGET_H
#define RESIZABLEWIDGET_H

#include <qpushbutton.h>
#include <qpoint.h>

class KComboView;
class QMouseEvent;
class MyPushButton;

/**
@file resizableiwidget.h
Resizable widget.
*/

/**
Used to place resizable widgets onto toolbars.
TODO: Put this code into the KDevelop libs instead of resizeablecombo.h/cpp
*/
class ResizableWidget: public QWidget{
    Q_OBJECT
public:
    ResizableWidget(QWidget *view, QWidget *parent = 0);

private:
    MyPushButton *m_sizer;
    QWidget *m_widget;

friend class MyPushButton;
};

class MyPushButton: public QPushButton
{
public:
    MyPushButton(ResizableWidget *parent = 0);

    QPoint pressedPos()
    {
        return m_pressedPos;
    }

protected:
    virtual void mouseReleaseEvent ( QMouseEvent * e );
    virtual void mousePressEvent ( QMouseEvent * e );
    virtual void mouseMoveEvent ( QMouseEvent * e );

private:
    bool m_resizing;
    QPoint m_pressedPos;
    int m_width;
    ResizableWidget *m_widget;
};

#endif
