/***************************************************************************
                          actioneditdialog.h  -  description
                             -------------------
    begin                : Wed Oct 11 2000
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
/****************************************************************************
** Form interface generated from reading ui file 'actionedit.ui'
**
** Created: Wed Oct 11 17:48:33 2000
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef ACTIONEDITDIALOG_H
#define ACTIONEDITDIALOG_H

#include <qdialog.h>
#include <qdom.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class FileCombo;
class QButtonGroup;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMultiLineEdit;
class QPushButton;
class QRadioButton;
class QTabWidget;
class QWidget;
class KIconButton;

class ActionEditDialog : public QDialog
{ 
    Q_OBJECT

public:
    ActionEditDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ActionEditDialog();

    QGroupBox* GroupBox7;
    KIconButton* actionIcon;
    QCheckBox* checkActionIconText;
    QLineEdit* actionButtonText;
    QTabWidget* actionTab;
    QComboBox* standardActions;
    QCheckBox* checkSeparator;
    QLabel* TextLabel5;
    QCheckBox* useTagClose;
    QCheckBox* insertInLine;
    QLineEdit* tag;
    QLineEdit* tagClose;
    QCheckBox* useActionDialog;
    QTabWidget* tabScript;
    QButtonGroup* ButtonGroup34;
    QRadioButton* inputNone;
    QRadioButton* inputSelected;
    QRadioButton* inputCurrent;
    QButtonGroup* ButtonGroup35;
    QRadioButton* outputNone;
    QRadioButton* outputCursor;
    QRadioButton* outputReplace;
    QRadioButton* outputNew;
    QRadioButton* outputMessage;
    QButtonGroup* ButtonGroup35_2;
    QRadioButton* errorNone;
    QRadioButton* errorCursor;
    QRadioButton* errorReplace;
    QRadioButton* errorNew;
    QRadioButton* errorMessage;
    QRadioButton* errorMerge;
    FileCombo* script;
    QMultiLineEdit* text;
    QLineEdit* actionName;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

    void operator << ( QDomElement& );
    void operator >> ( QDomElement& );

protected:
    QHBoxLayout* hbox;
    QGridLayout* grid;
    QGridLayout* grid_2;
    QGridLayout* grid_3;
    QGridLayout* grid_4;
    QGridLayout* grid_5;
    QGridLayout* grid_6;
    QGridLayout* grid_7;
    QGridLayout* grid_8;
    QGridLayout* grid_9;
    QGridLayout* grid_10;
    QGridLayout* grid_11;
    QGridLayout* grid_12;
};

#endif // ACTIONEDITDIALOG_H
