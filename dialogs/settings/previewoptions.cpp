/***************************************************************************
                          previewoptions.cpp  -  description
                             -------------------
    begin                : Fri Aug 4 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002, 2004 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "previewoptions.h"
#include "previewoptions.moc"
// qt includes
#include <qradiobutton.h>
#include <qcheckbox.h>

// kde includes

PreviewOptions::PreviewOptions(QWidget *parent, const char *name)
  : PreviewOptionsS(parent,name)
{
}

PreviewOptions::~PreviewOptions(){
}

void PreviewOptions::setWindowLayout(const QString& layout )
{
  if ( layout == "Default") resetLayout->setChecked(true);
  else resetLayout->setChecked(false);
}

void PreviewOptions::setPosition(const QString& position )
{
  radioEditor->setChecked(true);

  if ( position == "Editor") radioEditor->setChecked(true);
  if ( position == "Toolview"  ) radioToolview->setChecked(true);
}

QString PreviewOptions::position()
{
  QString position = "Bottom";

  if ( radioEditor->isChecked() )
      position = "Editor";
  if ( radioToolview->isChecked() )
      position = "Toolview";
  return position;
}

QString PreviewOptions::layout()
{
  QString layout = "Custom";
  if ( resetLayout->isChecked() )
      layout = "Default";
  return layout;
}

QString PreviewOptions::closeButtons()
{
   QString closeButtonState = "ShowAlways";
   if (radioDelayedCloseButtons->isChecked())
       closeButtonState = "ShowDelayed";
   if (radioNoCloseButtons->isChecked())
       closeButtonState = "Disabled";
   return closeButtonState;
}

void PreviewOptions::setCloseButtons(const QString &state)
{
   if (state == "ShowAlways")  radioShowCloseButtons->setChecked(true);
   if (state == "ShowDelayed")  radioDelayedCloseButtons->setChecked(true);
   if (state == "Disabled")  radioNoCloseButtons->setChecked(true);
}

