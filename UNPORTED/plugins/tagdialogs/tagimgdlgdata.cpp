/**********************************************************************
            --- KDevelop (KDlgEdit)  generated file ---

            Last generated: Fri Apr 7 23:17:41 2000

            DO NOT EDIT!!!  This file will be automatically
            regenerated by KDevelop.  All changes will be lost.

**********************************************************************/
#include <klocale.h>
#include <kurl.h>
#include <knumvalidator.h>

#include <QLayout>
#include <qspinbox.h>

#include "pictureview.h"
#include "tagimgdlg.h"

void  TagImgDlg::initDialog(){
  QGridLayout *grid = new QGridLayout( this );
  grid->setSpacing( 13 );
  grid->setMargin( 11 );
  labelImgSource= new QLabel(this);
  labelImgSource->setText(i18n("Image source:"));
  grid->addWidget(labelImgSource, 0, 0, 0, 0);

  lineImgSource= new KLineEdit(this);
  grid->addWidget(lineImgSource, 0, 0, 1, 3);

  buttonImgSource= new QPushButton(this);
  buttonImgSource->setText(i18n("..."));
  grid->addWidget(buttonImgSource, 0, 0, 4, 4);

  QLabel_4= new QLabel(this);
  QLabel_4->setText(i18n("Width:"));
  grid->addWidget(QLabel_4, 1, 1, 0, 0);

  lineWidth= new KLineEdit(this);
  lineWidth->setValidator( new KFloatValidator( lineWidth ) );
  grid->addWidget(lineWidth, 1, 1, 1, 1);

  QLabel_5= new QLabel(this);
  QLabel_5->setText(i18n("Height:"));
  grid->addWidget(QLabel_5, 1, 1, 2, 2);

  lineHeight= new KLineEdit(this);
  lineHeight->setValidator( new KFloatValidator( lineHeight ) );
  grid->addWidget(lineHeight, 1, 1, 3, 3);

  buttonRecalcImgSize= new QPushButton(this);
  buttonRecalcImgSize->setText(i18n("..."));
  grid->addWidget(buttonRecalcImgSize, 1, 1, 4, 4);

  QLabel_6= new QLabel(this);
  QLabel_6->setText(i18n("HSpace:"));
  grid->addWidget(QLabel_6, 2, 2, 0, 0);

  lineHSpace= new KLineEdit(this);
  grid->addWidget(lineHSpace, 2, 2, 1, 1);

  QLabel_7= new QLabel(this);
  QLabel_7->setText(i18n("VSpace:"));
  grid->addWidget(QLabel_7, 2, 2, 2, 2);

  lineVSpace= new KLineEdit(this);
  grid->addWidget(lineVSpace, 2, 2, 3, 3);

  QLabel_8= new QLabel(this);
  QLabel_8->setText(i18n("Alternate text:"));
  grid->addWidget(QLabel_8, 3, 3, 0, 0);

  lineAltText= new KLineEdit(this);
  grid->addWidget(lineAltText, 3, 3, 1, 4);

  QLabel_9= new QLabel(this);
  QLabel_9->setText(i18n("Border:"));
  grid->addWidget(QLabel_9, 4, 4, 0, 0);

  spinBorder= new QSpinBox(this);
  spinBorder->setRange(0,20);
  grid->addWidget(spinBorder, 4, 4, 1, 1);

  QLabel_10= new QLabel(this);
  QLabel_10->setText(i18n("Align:"));
  grid->addWidget(QLabel_10, 4, 4, 2, 2);

  comboAlign= new QComboBox(this);
  grid->addWidget(comboAlign, 4, 4, 3, 4);

  widgetImg= new PictureView(this);
  grid->addWidget(widgetImg, 5, 10, 0, 4);

}
