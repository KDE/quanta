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

#include <framewizards.h>
#include "visualframeeditor.h"
class QStringList;

/** FrameWizard is the base class of the project */
class FrameWizard : public FrameWizardS
{
  Q_OBJECT
  private:
    bool m_hasSelected,
            m_saved; // if saved = false the the file containing the frameset structure
                            // has been not saved and so you cannot edit the frame
                            // This is for me: se non si salva il file no si riesce a conoscere il
                            // percorso relativo dei file da mettere nell'attributo src
    QString m_currSA;

  public:
    FrameWizard( QWidget* parent=0, const char *name=0);
    ~FrameWizard();

  private slots:
    void showFrameEditorDlg();
    void reset();
    void remove();
    void catchSelectedArea(const QString &id );
    void split();
    void draw();
    int showRCeditorDlg(const QString &s);

  public :
    void loadExistingFramesetStructure(const QStringList &list){ vfe->loadExistingStructure(list);}
    QString generateFramesetStructure(){ return vfe->framesetStructure(); }
    void setSaved( bool b){ m_saved=b; }
    void setMarkupLanguage(const QString& s){ vfe->setMarkupLanguage(s);}

  signals:
    void launchDraw();

};

#endif
