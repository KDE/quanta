/**********************************************************************
            --- KDevelop (KDlgEdit)  generated file ---                

            Last generated: Fri Apr 7 23:17:41 2000

            DO NOT EDIT!!!  This file will be automatically
            regenerated by KDevelop.  All changes will be lost.

**********************************************************************/
#include <kapp.h>
#include <klocale.h>
#include <knumvalidator.h>
#include <qspinbox.h>
#include "pictureview.h"
#include "tagimgdlg.h"

void  TagImgDlg::initDialog(){
  this->resize(400,320);
  this->setMinimumSize(0,0);
  labelImgSource= new QLabel(this,"labelImgSource");
  labelImgSource->setGeometry(10,10,120,30);
  labelImgSource->setMinimumSize(0,0);
  labelImgSource->setText(i18n("Image Source"));

  lineImgSource= new KLineEdit(this,"lineImgSource");
  lineImgSource->setGeometry(130,10,220,25);
  lineImgSource->setMinimumSize(0,0);

  buttonImgSource= new QPushButton(this,"NoName");
  buttonImgSource->setGeometry(355,10,35,25);
  buttonImgSource->setMinimumSize(0,0);
  buttonImgSource->setText(i18n("..."));

  QLabel_4= new QLabel(this,"NoName");
  QLabel_4->setGeometry(10,40,120,30);
  QLabel_4->setMinimumSize(0,0);
  QLabel_4->setText(i18n("Width"));

  lineWidth= new KLineEdit(this,"NoName");
  lineWidth->setGeometry(130,40,60,25);
  lineWidth->setMinimumSize(0,0);
  lineWidth->setValidator( new KFloatValidator( lineWidth ) );

  QLabel_5= new QLabel(this,"NoName");
  QLabel_5->setGeometry(200,40,90,30);
  QLabel_5->setMinimumSize(0,0);
  QLabel_5->setText(i18n("Height"));

  lineHeight= new KLineEdit(this,"NoName");
  lineHeight->setGeometry(290,40,60,25);
  lineHeight->setMinimumSize(0,0);
  lineHeight->setValidator( new KFloatValidator( lineHeight ) );
  
  buttonRecalcImgSize= new QPushButton(this,"NoName");
  buttonRecalcImgSize->setGeometry(355,40,35,25);
  buttonRecalcImgSize->setMinimumSize(0,0);
  buttonRecalcImgSize->setText(i18n("..."));

  QLabel_6= new QLabel(this,"NoName");
  QLabel_6->setGeometry(10,70,120,30);
  QLabel_6->setMinimumSize(0,0);
  QLabel_6->setText(i18n("HSpace"));

  lineHSpace= new KLineEdit(this,"NoName");
  lineHSpace->setGeometry(130,70,60,25);
  lineHSpace->setMinimumSize(0,0);

  QLabel_7= new QLabel(this,"NoName");
  QLabel_7->setGeometry(200,70,90,30);
  QLabel_7->setMinimumSize(0,0);
  QLabel_7->setText(i18n("VSpace"));

  lineVSpace= new KLineEdit(this,"NoName");
  lineVSpace->setGeometry(290,70,60,25);
  lineVSpace->setMinimumSize(0,0);

  QLabel_8= new QLabel(this,"NoName");
  QLabel_8->setGeometry(10,100,120,30);
  QLabel_8->setMinimumSize(0,0);
  QLabel_8->setText(i18n("Alternate Text"));

  lineAltText= new KLineEdit(this,"NoName");
  lineAltText->setGeometry(130,100,260,25);
  lineAltText->setMinimumSize(0,0);

  QLabel_9= new QLabel(this,"NoName");
  QLabel_9->setGeometry(10,130,120,30);
  QLabel_9->setMinimumSize(0,0);
  QLabel_9->setText(i18n("Border"));

  spinBorder= new QSpinBox(this,"NoName");
  spinBorder->setGeometry(130,130,60,25);
  spinBorder->setMinimumSize(0,0);
  spinBorder->setRange(0,20);

  QLabel_10= new QLabel(this,"NoName");
  QLabel_10->setGeometry(220,130,70,30);
  QLabel_10->setMinimumSize(0,0);
  QLabel_10->setText(i18n("Align"));

  comboAlign= new QComboBox(this,"NoName");
  comboAlign->setGeometry(290,130,100,25);
  comboAlign->setMinimumSize(0,0);

  widgetImg= new PictureView(this);
  widgetImg->setGeometry(10,170,380,140);
  widgetImg->setMinimumSize(0,0);

}
