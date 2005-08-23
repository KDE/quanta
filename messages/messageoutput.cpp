/***************************************************************************
                          messageoutput.cpp  -  description
                             -------------------
    begin                : Thu Feb 24 2000
    copyright            : (C) 2000 by Yacovlev Alexander & Dmitry Poplavski <pdima@mail.univ.kiev.ua>
                           (C) 2003-2005 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// KDE includes
#include <kapplication.h>
#include <kdebug.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kiconloader.h>

// Qt includes
#include <qclipboard.h>
#include <qfile.h>
#include <qtextstream.h>

#include "messageoutput.h"
#include "messageitem.h"

MessageOutput::MessageOutput(QWidget *parent, const char *name )
  : QListBox(parent,name)
{
  m_maxItems = 2000;

  QPalette pal = palette();
  pal.setColor(QColorGroup::HighlightedText, pal.color(QPalette::Normal, QColorGroup::Text));
  pal.setColor(QColorGroup::Highlight,       pal.color(QPalette::Normal, QColorGroup::Mid));
  setPalette(pal);
  setFocusPolicy( NoFocus );

  m_popupMenu = new KPopupMenu(this);
  connect(this, SIGNAL(contextMenuRequested(QListBoxItem*, const QPoint&)),
     this, SLOT(showMenu(QListBoxItem*, const QPoint&)));
  m_popupMenu->insertItem( SmallIconSet("editcopy"), i18n("&Copy"), this, SLOT(copyContent()) ) ;
  m_popupMenu->insertItem( SmallIconSet("filesaveas"), i18n("&Save As..."), this, SLOT(saveContent()) ) ;
  m_popupMenu->insertSeparator();
  m_popupMenu->insertItem( SmallIconSet("editclear"), i18n("Clear"), this, SLOT(clear()) ) ;

  connect( this, SIGNAL(clicked(QListBoxItem*)), SLOT(clickItem(QListBoxItem*)) );
}

MessageOutput::~MessageOutput()
{
}

MessageItem *MessageOutput::insertItem(const QString& s)
{
  checkMaxItems();
  MessageItem *it = new MessageItem(this, s);
  setBottomItem(count()>0?count()-1:0);
  return it;
}

void MessageOutput::addToLastItem(const QString& s)
{
  int ind = count() - 1;
  if ( ind != -1 ) {
    MessageItem *it = dynamic_cast<MessageItem*>( item(ind) );
    if ( it )
      it->addText( s );
    else
      changeItem( text( ind )+ s, ind );
  }
}

void MessageOutput::showMessage(int line, int col, const QString &fileName, const QString& s, bool append)
{
  MessageItem *it = 0L;
  QString message = s;
  int endPos;
  if ( !count() || (!append && !text(count()-1).stripWhiteSpace().isEmpty()) )
    it = insertItem("");
  while ( ( endPos = message.find('\n') ) != -1 ) {
    if (it)
    {
      it->setLine(line);
      it->setColumn(col);
      it->setFileName(fileName);
    }
    addToLastItem( message.left(endPos) );
    it = insertItem("");
    message.remove(0,endPos+1);
  }
  if (!message.isEmpty())
  {
    if (it)
    {
      it->setLine(line);
      it->setColumn(col);
      it->setFileName(fileName);
    }
    addToLastItem( message);
  }
  setBottomItem(count()>0?count()-1:0);
}

void MessageOutput::showMessage(const QString& s, bool append)
{
  showMessage(-1, -1, "", s, append);
}


void MessageOutput::checkMaxItems()
{
  if ( count() >= m_maxItems )
    removeItem( index(firstItem()) );
}

void MessageOutput::clickItem( QListBoxItem * l_item )
{
   MessageItem *item = dynamic_cast<MessageItem*>(l_item);
   if ( item )  {
//     kdDebug(24000) << "Column: " << item->column() << endl;
     if ( item->line() != -1  )
       emit clicked( item->fileName(), item->line() - 1, item->column() - 1);
   }
}

void MessageOutput::showMenu( QListBoxItem*, const QPoint& l_point )
{
  m_popupMenu->exec(l_point);
}

QString MessageOutput::content()
{
  QString p_content;
  for (uint i=0; i<count(); i++)
    p_content.append(text(i) + "\n");
  return p_content;
}

void MessageOutput::copyContent()
{
  kapp->clipboard()->setText(content(), QClipboard::Clipboard);
}

void MessageOutput::saveContent()
{
  KURL url=KFileDialog::getSaveURL(QDir::currentDirPath(),
    i18n("*.log|Log Files (*.log)\n*|All Files"), this, i18n("Save Log File"));
  if (url.isEmpty())
    return;

  QFileInfo fileinfo(url.path());
  if (fileinfo.exists() && KMessageBox::warningContinueCancel(0,
      i18n("<qt>File<br><b>%1</b><br>already exists. Overwrite it?</qt>")
      .arg(url.path()), QString::null, i18n("Overwrite")) == KMessageBox::Cancel)
    return;

  QFile file(url.path());
  if (!file.open(IO_WriteOnly)) {
     KMessageBox::error(0, i18n("<qt>Cannot save log file<br><b>%1</b></qt>")
         .arg(url.url()));
     return;
   }

   QTextStream textfile(&file);
   textfile << content();
   file.close();

}


#include "messageoutput.moc"
