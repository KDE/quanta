/***************************************************************************
                          parseroptions.h  -  description
                             -------------------
    begin                : Wed Sep 20 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PARSEROPTIONS_H
#define PARSEROPTIONS_H

#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QGroupBox;
class QLabel;
class QSpinBox;

class ParserOptions : public QWidget
{ 
    Q_OBJECT

public:
    ParserOptions( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ParserOptions();

    QGroupBox* GroupBox2;
    QComboBox* comboMBM;
    QComboBox* comboRBM;
    QComboBox* comboLBM;
    QComboBox* comboDoubleClick;
    QLabel* TextLabel2_2;
    QLabel* TextLabel3_2;
    QLabel* TextLabel4_2;
    QLabel* TextLabel5_2;
    QLabel* TextLabel7;
    QSpinBox* spinExpand;
    QLabel* TextLabel6;

protected:
    QGridLayout* grid;
    QGridLayout* grid_2;
};

#endif // PARSEROPTIONS_H
