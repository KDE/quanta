/**********************************************************************
            --- KDevelop (KDlgEdit)  generated file ---                

            DO NOT EDIT!!!  This file will be automatically
            regenerated by KDevelop.  All changes will be lost.

**********************************************************************/
#include <kapp.h>
#include <klocale.h>
#include "projectnewgeneral.h"

void  ProjectNewGeneral::initDialog(){
  this->resize(450,370);
  this->setMinimumSize(0,0);
  QLabel_1= new QLabel(this,"NoName");
  QLabel_1->setGeometry(10,30,140,30);
  QLabel_1->setMinimumSize(0,0);
  QLabel_1->setText(i18n("Project Name :"));

  linePrjName= new QLineEdit(this,"NoName");
  linePrjName->setGeometry(150,30,240,25);
  linePrjName->setMinimumSize(0,0);

  QLabel_2= new QLabel(this,"NoName");
  QLabel_2->setGeometry(10,60,140,30);
  QLabel_2->setMinimumSize(0,0);
  QLabel_2->setText(i18n("Project Directory :"));

  linePrjDir= new QLineEdit(this,"NoName");
  linePrjDir->setGeometry(150,60,240,25);
  linePrjDir->setMinimumSize(0,0);

  buttonDir= new QPushButton(this,"NoName");
  buttonDir->setGeometry(400,60,30,25);
  buttonDir->setMinimumSize(0,0);
  buttonDir->setText(i18n("..."));

  QLabel_3= new QLabel(this,"NoName");
  QLabel_3->setGeometry(10,90,130,30);
  QLabel_3->setMinimumSize(0,0);
  QLabel_3->setText(i18n("Project File :"));

  linePrjFile= new QLineEdit(this,"NoName");
  linePrjFile->setGeometry(150,90,240,25);
  linePrjFile->setMinimumSize(0,0);

  QLabel_4= new QLabel(this,"NoName");
  QLabel_4->setGeometry(10,120,130,30);
  QLabel_4->setMinimumSize(0,0);
  QLabel_4->setText(i18n("Base URL:"));

  lineBaseURL= new QLineEdit(this,"NoName");
  lineBaseURL->setGeometry(150,120,240,25);
  lineBaseURL->setMinimumSize(0,0);

  QLabel_5= new QLabel(this,"NoName");
  QLabel_5->setGeometry(10,150,130,30);
  QLabel_5->setMinimumSize(0,0);
  QLabel_5->setText(i18n("Author :"));

  lineAuthor= new QLineEdit(this,"NoName");
  lineAuthor->setGeometry(150,150,240,25);
  lineAuthor->setMinimumSize(0,0);

  QLabel_6= new QLabel(this,"NoName");
  QLabel_6->setGeometry(10,180,130,30);
  QLabel_6->setMinimumSize(0,0);
  QLabel_6->setText(i18n("E-Mail :"));

  lineEmail= new QLineEdit(this,"NoName");
  lineEmail->setGeometry(150,180,240,25);
  lineEmail->setMinimumSize(0,0);

  QLabel_7= new QLabel(this,"NoName");
  QLabel_7->setGeometry(10,220,140,30);
  QLabel_7->setMinimumSize(0,0);
  QLabel_7->setText(i18n("Images Directory:"));

  QLabel_8= new QLabel(this,"NoName");
  QLabel_8->setGeometry(10,250,140,30);
  QLabel_8->setMinimumSize(0,0);
  QLabel_8->setText(i18n("cgi-bin Directory:"));

  QLabel_9= new QLabel(this,"NoName");
  QLabel_9->setGeometry(10,280,140,30);
  QLabel_9->setMinimumSize(0,0);
  QLabel_9->setText(i18n("Project Scripts Dir:"));

  lineImagesDir= new QLineEdit(this,"NoName");
  lineImagesDir->setGeometry(150,220,240,25);
  lineImagesDir->setMinimumSize(0,0);

  buttonImagesDir= new QPushButton(this,"NoName");
  buttonImagesDir->setGeometry(400,220,30,25);
  buttonImagesDir->setMinimumSize(0,0);
  buttonImagesDir->setText(i18n("..."));

  lineCgiDir= new QLineEdit(this,"NoName");
  lineCgiDir->setGeometry(150,250,240,25);
  lineCgiDir->setMinimumSize(0,0);

  buttonCgiDir= new QPushButton(this,"NoName");
  buttonCgiDir->setGeometry(400,250,30,25);
  buttonCgiDir->setMinimumSize(0,0);
  buttonCgiDir->setText(i18n("..."));

  lineScriptsDir= new QLineEdit(this,"NoName");
  lineScriptsDir->setGeometry(150,280,240,25);
  lineScriptsDir->setMinimumSize(0,0);

  buttonScriptsDir= new QPushButton(this,"NoName");
  buttonScriptsDir->setGeometry(400,280,30,25);
  buttonScriptsDir->setMinimumSize(0,0);
  buttonScriptsDir->setText(i18n("..."));

}
