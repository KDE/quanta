/***************************************************************************
                          framewizard.h  -  description
                             -------------------
    begin                : mer giu  4 14:14:07 CEST 2003
    copyright            : (C) |YEAR| by Gu2003Luciano
    email                : gulmini.luciano@student.unife.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FRAMEWIZARD_H
#define FRAMEWIZARD_H


#include <qwidget.h>
#include <framewizards.h>
#include "visualframeeditor.h"
class QStringList;

/** FrameWizard is the base class of the project */
class FrameWizard : public FrameWizardS
{
  Q_OBJECT
  private:
    bool saved; // if saved = false the the file containing the frameset structure
                // has been not saved on so you cannot edit the frame
          // This is for me: se non si salva il file no si riesce a conoscere il
          // percorso relativo dei file da mettere nell'attributo src
    QString framesetFileCurrentPath;

  public:
    /** construtor */
    FrameWizard( QWidget* parent=0, const char *name=0);
    /** destructor */
    ~FrameWizard();
  public slots:
    void catchSelectedArea( QString id );
    void split();
    void draw();
    void showFrameEditorDlg();
    void reset();
    void remove();
    void loadExistingFramesetStructure(QStringList list){ vfe->loadExistingStructure(list,framesetFileCurrentPath);}
    void setFramesetFileCurrentPath(const QString& p){ framesetFileCurrentPath = p; }
    QString getFramesetFileCurrentPath() { return framesetFileCurrentPath; }
    QString generateFramesetStructure(){ return vfe->framesetStructure(framesetFileCurrentPath); }
    int showRCeditorDlg( QString );
    void setSaved( bool b){ saved=b; }

};

#endif
