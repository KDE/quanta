/***************************************************************************
                          previewoptions.cpp  -  description
                             -------------------
    begin                : Fri Aug 4 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@penguinpowered.com,sequitur@easystreet.com>
                           (C) 2002 Andras Mantia <amantia@kde.org>
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
#include <qlabel.h>
#include <qradiobutton.h>

// kde includes
#include <kiconloader.h>

PreviewOptions::PreviewOptions(QWidget *parent, const char *name)
  : PreviewOptionsS(parent,name)
{
  connect(radioBottom, SIGNAL(toggled(bool)),
          this,  SLOT(slotTogglePreview()));
  connect(radioRight, SIGNAL(toggled(bool)),
          this,  SLOT(slotTogglePreview()));
  connect(radioDisabled, SIGNAL(toggled(bool)),
          this,  SLOT(slotTogglePreview()));
  connect(radioFullworkspace, SIGNAL(toggled(bool)),
          this,  SLOT(slotTogglePreview()));
  connect(radioDefault, SIGNAL(toggled(bool)),
          this,  SLOT(slotToggleWindowLayout()));
  connect(radioTabbed, SIGNAL(toggled(bool)),
          this,  SLOT(slotToggleWindowLayout()));
  connect(radioCustom, SIGNAL(toggled(bool)),
          this,  SLOT(slotToggleWindowLayout()));
}

PreviewOptions::~PreviewOptions(){
}

void PreviewOptions::setWindowLayout(const QString& layout )
{
  if ( layout == "Default") radioDefault->setChecked(true);
  if ( layout == "Tabbed" ) radioTabbed->setChecked(true);
  if ( layout == "Custom") radioCustom->setChecked(true);
}

void PreviewOptions::setPosition(const QString& position )
{
  radioBottom->setChecked(true);

  if ( position == "Disabled") radioDisabled->setChecked(true);
  if ( position == "Bottom"  ) radioBottom->setChecked(true);
  if ( position == "Right"   ) radioRight->setChecked(true);
  if ( position == "FWSpace" ) radioFullworkspace->setChecked(true);
}

QString PreviewOptions::position()
{
  QString position = "Bottom";

  if ( radioBottom->isChecked() )
      position = "Bottom";
  if ( radioRight->isChecked() )
      position = "Right";
  if ( radioFullworkspace->isChecked() )
      position = "FWSpace";
  if ( radioDisabled->isChecked() )
      position = "Disabled";

  return position;
}

QString PreviewOptions::layout()
{
  QString layout = "Custom";
  if ( radioDefault->isChecked() )
      layout = "Default";
  if ( radioTabbed->isChecked() )
      layout = "Tabbed";

  return layout;
}

void PreviewOptions::slotTogglePreview()
{
  if ( radioBottom->isChecked() )
      pixmap->setPixmap( UserIcon("preview1") );
  if ( radioRight->isChecked() )
      pixmap->setPixmap( UserIcon("preview2") );
  if ( radioFullworkspace->isChecked() )
      pixmap->setPixmap( UserIcon("preview3") );
  if ( radioDisabled->isChecked() )
      pixmap->setPixmap( UserIcon("preview4") );
}

void PreviewOptions::slotToggleWindowLayout()
{
  if ( radioDefault->isChecked() )
      windowPixmap->setPixmap( UserIcon("layout1") );
  if ( radioTabbed->isChecked() )
      windowPixmap->setPixmap( UserIcon("layout2") );
  if ( radioCustom->isChecked() )
      windowPixmap->setPixmap( UserIcon("customlayout") );
}

