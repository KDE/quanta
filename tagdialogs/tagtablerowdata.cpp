/**********************************************************************
            --- KDevelop (KDlgEdit)  generated file ---                

            Last generated: Sun Apr 2 02:14:49 2000

            DO NOT EDIT!!!  This file will be automatically
            regenerated by KDevelop.  All changes will be lost.

**********************************************************************/
#include <kapp.h>
#include <klocale.h>
#include "tagtablerow.h"

void  TagTableRow::initDialog(){
  this->resize(400,140);
  this->setMinimumSize(0,0);
  QLabel_1= new QLabel(this,"NoName");
  QLabel_1->setGeometry(10,10,120,30);
  QLabel_1->setMinimumSize(0,0);
  QLabel_1->setText(i18n("Align"));

  comboAlign= new QComboBox(true,this,"NoName");
  comboAlign->setGeometry(130,10,260,25);
  comboAlign->setMinimumSize(0,0);

  QLabel_2= new QLabel(this,"NoName");
  QLabel_2->setGeometry(10,40,120,30);
  QLabel_2->setMinimumSize(0,0);
  QLabel_2->setText(i18n("VAlign"));

  comboVAlign= new QComboBox(true,this,"NoName");
  comboVAlign->setGeometry(130,40,260,25);
  comboVAlign->setMinimumSize(0,0);

  QLabel_3= new QLabel(this,"NoName");
  QLabel_3->setGeometry(10,100,120,30);
  QLabel_3->setMinimumSize(0,0);
  QLabel_3->setText(i18n("BG Color"));

  comboColor= new QComboBox(true,this,"NoName");
  comboColor->setGeometry(130,100,170,25);
  comboColor->setMinimumSize(0,0);

  colorButton= new KColorButton(this,"NoName");
  colorButton->setGeometry(310,100,80,25);
  colorButton->setMinimumSize(0,0);
  colorButton->setText(i18n("Button"));

}
