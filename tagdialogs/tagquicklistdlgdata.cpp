/**********************************************************************
            --- KDevelop (KDlgEdit)  generated file ---

            Last generated: Tue Dec 7 16:51:35 1999

            DO NOT EDIT!!!  This file will be automatically
            regenerated by KDevelop.  All changes will be lost.

**********************************************************************/
#include <kapplication.h>
#include <klocale.h>
#include <qbuttongroup.h>
#include "tagquicklistdlg.h"

void  TagQuickListDlg::initDialog()
{
	this->resize(230,150);

  QLabel_1= new QLabel(this,"NoName");
  QLabel_1->setGeometry(10,10,100,30);
  QLabel_1->setMinimumSize(0,0);
  QLabel_1->setText(i18n("Rows:"));

  spinBoxRows= new QSpinBox(this,"NoName");
  spinBoxRows->setGeometry(120,10,100,25);
  spinBoxRows->setMinimumSize(0,0);
  spinBoxRows->setRange(1,99);

  buttonOk= new QPushButton(this,"NoName");
  buttonOk->setGeometry(10,110,100,30);
  buttonOk->setMinimumSize(0,0);
  buttonOk->setText(i18n("&OK"));

  QLabel_2= new QLabel(this,"NoName");
  QLabel_2->setGeometry(10,40,100,30);
  QLabel_2->setMinimumSize(0,0);
  QLabel_2->setText(i18n("Style:"));

  buttonCancel= new QPushButton(this,"NoName");
  buttonCancel->setGeometry(120,110,100,30);
  buttonCancel->setMinimumSize(0,0);
  buttonCancel->setText(i18n("&Cancel"));

  groupBox= new QButtonGroup(this,"NoName");
  groupBox->setGeometry(120,40,100,60);
  groupBox->setMinimumSize(0,0);

  radioOrdered= new QRadioButton(groupBox,"NoName");
  radioOrdered->setGeometry(5,5,90,25);
  radioOrdered->setMinimumSize(0,0);
  radioOrdered->setText(i18n("Ordered"));
  radioOrdered->setChecked(true);

  radioUnordered= new QRadioButton(groupBox,"NoName");
  radioUnordered->setGeometry(5,30,90,25);
  radioUnordered->setMinimumSize(0,0);
  radioUnordered->setText(i18n("Unordered"));
  radioUnordered->setChecked(false);
}
