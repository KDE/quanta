/* This file is part of the KDE project
   Copyright (C) 2003 Alexander Dymo <cloudtemple@mksat.net>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

*/
#ifndef RESIZABLEWIDGET_H
#define RESIZABLEWIDGET_H

#include "quantaexport.h"

#include <QPushButton>
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
class LIBQUANTA_EXPORT ResizableWidget: public QWidget{
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
