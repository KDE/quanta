//
// C++ Interface: dtepeditdlg
//
// Description: 
//
//
// Author: Andras Mantia <amantia@kde.org>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DTEPEDITDLG_H
#define DTEPEDITDLG_H

#include "dtepeditdlgs.h"

/**
@author Andras Mantia
*/

class KConfig;

class DTEPEditDlg : public DTEPEditDlgS
{
  Q_OBJECT
  public:
    DTEPEditDlg(const QString& descriptionFile, QWidget *parent = 0, const char *name = 0);
    ~DTEPEditDlg();
    void saveResult();
    void slotFamilyChanged(int family);
    
  private:  
    void init();
    void readGeneral();
    void readPages();
    void writeGeneral(KConfig *config);
    void writePages(KConfig *config);
    
  QString m_descriptionFile;
  KConfig *m_config;
  int m_family;
  QWidget *m_pagesWidget;
};

#endif
