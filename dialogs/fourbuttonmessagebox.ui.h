
/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
//(C) 2003 Andras Mantia <amantia@kde.org>

void FourButtonMessageBox::cancelButton_clicked()
{
   m_status = -1;
   reject();
}


void FourButtonMessageBox::user1Button_clicked()
{
  m_status = 1;
  accept();
}


void FourButtonMessageBox::user2Button_clicked()
{
  m_status = 2;
  accept();
}


void FourButtonMessageBox::user3Button_clicked()
{
  m_status = 3;
  accept();
}


int FourButtonMessageBox::status()
{
  return m_status;
}
