/***************************************************************************
                          dtepeditdlg.h  -  description
                             -------------------
    begin                : Tue Feb 1 11:07:24 2005
    copyright            : (C) 2005 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#ifndef DTEPEDITDLG_H
#define DTEPEDITDLG_H

//qt includes
#include <QList>

//own includes
#include "ui_dtepeditdlgs.h"

/**
@author Andras Mantia
*/

class KConfig;
class Ui_DTEPStructureEditDlgS;

struct StructGroup {
  QString name;   ///<the name of the group
  QString noName; ///<the text when there are no elements in the group
  QString icon;   ///<the icon of the group
  QString tag;
  bool hasFileName;
  QString fileNameRx;
  bool appendToTags;
  QString parentGroup;
  QString definitionRx;
  bool definitionRxMinimal;
  QString usageRx;
  QString typeRx;
  QString tagType;
  bool parseFile;
  QString completeAfterRx;
  QString removeRx;
  bool simpleGroup;
  bool variableGroup;
  bool functionGroup;
  bool classGroup;
  bool objectGroup;
};

class DTEPEditDlg : public QWidget, public Ui::DTEPEditDlgS
{
  Q_OBJECT
  public:
    explicit DTEPEditDlg(const QString& descriptionFile, QWidget *parent = 0);
    ~DTEPEditDlg();
    void saveResult();
    void slotFamilyChanged(int family);
    void slotEditStructGroup();
    void slotAddStructGroup();
    void slotDeleteStructGroup();
    
  private:  
    void init();
    void readGeneral();
    void readPages();
    void readParserRules();
    void readStructures();
    StructGroup readFromStructDlg(Ui_DTEPStructureEditDlgS *structDlg);
    
    void writeGeneral(KConfig *config);
    void writePages(KConfig *config);
    void writeParserRules(KConfig *config);
    void writeStructures(KConfig *config);
    
  QString m_descriptionFile;
  KConfig *m_config;
  int m_family;
  QWidget *m_pagesWidget;
  QList<StructGroup> m_structGroups;
};

#endif
