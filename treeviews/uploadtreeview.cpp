/***************************************************************************
                          uploadtreeview.cpp  -  description
                             -------------------
    begin                : Sun Aug 25 2002
    copyright            : (C) 2002 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

#include <qregexp.h>

#include <kcursor.h>
#include <klocale.h>
#include <kurl.h>
#include <kfileitem.h>

#include "uploadtreeview.h"
#include "quantacommon.h"

UploadTreeView::UploadTreeView( QWidget *parent, const char *name ) :
  KListView(parent, name)
{
  setRootIsDecorated( true );
  setSorting( 0 );
  setMultiSelection(true);

  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  addColumn( i18n("Name") );
  addColumn( i18n("Upload") );
  addColumn( i18n("Size") );
  addColumn( i18n("Date") );

  setFocusPolicy(QWidget::ClickFocus);

  connect( this, SIGNAL(selectionChanged()), SLOT(slotSelectFile()));
  connect( this, SIGNAL(selectionChanged(QListViewItem *)),
           this, SLOT(slotSelectFile(QListViewItem *)));

  connect(this, SIGNAL(doubleClicked(QListViewItem *, const QPoint &, int )), SLOT(slotDoubleClicked(QListViewItem *, const QPoint &, int )));
}

UploadTreeView::~UploadTreeView()
{
}

int UploadTreeView::checkboxTree( QListViewItem *it )
{
  parentWidget()->setCursor(KCursor::workingCursor());

  QListViewItem *itIter = it ? it->firstChild() : firstChild();

  // bitFlag structure: (0/1)all children exist (0/1)no children exist.
  // We don't need some children as a bit flag, because that's implied if the bits are "00".

  int bitFlags = 3;
  int retVal = 1;

  if ( itIter != 0 )
  {
    for( ; itIter != 0; itIter = itIter->nextSibling() )
    {
      if ( dynamic_cast<UploadTreeFolder *>(itIter) )
      {
        int hadCheckFlags = checkboxTree( itIter );
        bitFlags &= hadCheckFlags;
        UploadTreeFolder *itF = static_cast<UploadTreeFolder *>(itIter);

        if (hadCheckFlags == 2) {
          // All children exist.
          itF->setWhichPixmap( "check" );
          itF->setSelected( true );
        }
        else if (hadCheckFlags == 1) {
          // No children exist.
          itF->setWhichPixmap( "check_clear" );
          itF->setSelected( false );
        }
        else {
          // Some children exist.
          itF->setWhichPixmap( "check_grey" );
          itF->setSelected( true );
        }

      }
      else if ( dynamic_cast<UploadTreeFile *>(itIter) )
      {
        UploadTreeFile *itF = static_cast<UploadTreeFile *>(itIter);
        if ( itF->isSelected() )
        {
          itF->setWhichPixmap("check");
          // Turn off "no children"
          if ( bitFlags % 2 == 1 ) bitFlags -= 1;
        }
        else
        {
          itF->setWhichPixmap("check_clear");
          // Turn off "all children".
          if (bitFlags >> 1 == 1) bitFlags -= 2;
        }

      }
    }
    retVal = bitFlags;
  } else
  {
    if ( dynamic_cast<UploadTreeFolder *>(it) )
    {
        UploadTreeFolder *itF = static_cast<UploadTreeFolder *>(it);
        if ( itF->isSelected() )
        {
          itF->setWhichPixmap("check");
          retVal = 2;
        }
        else
        {
          itF->setWhichPixmap("check_clear");
          retVal = 1;
        }

    }
  }

  parentWidget()->setCursor(KCursor::arrowCursor());

  return retVal;
}

void UploadTreeView::selectAllUnderNode( QListViewItem* it, bool select )
{
  QListViewItem *itIter = 0;
  if (it == 0) itIter = firstChild();
  else itIter = it->firstChild();

  for( ; itIter != 0; itIter = itIter->nextSibling() )
  {
    itIter->setSelected(select);
    selectAllUnderNode(itIter, select);
    if (itIter->isSelected() != select)
        itIter->setSelected(select);
  }
}

void UploadTreeView::slotSelectFile( QListViewItem *it )
{
  UploadTreeFolder *itF = dynamic_cast<UploadTreeFolder *>(it);
  // This need a bit of special behavior for clicking on directories.
  if ( itF )
  {
    itF->setSelected(it->isSelected());
    selectAllUnderNode( it, it->isSelected() );
  }

//set the correct checkbox for this item, if it was a folder
  int hadCheckFlags = checkboxTree(it);
  if ( itF )
  {
    if (hadCheckFlags == 2) {
      // All children exist.
      itF->setWhichPixmap( "check" );
      itF->setSelected( true );
    }
    else if (hadCheckFlags == 1) {
      // No children exist.
      itF->setWhichPixmap( "check_clear" );
      itF->setSelected( false );
    }
    else {
      // Some children exist.
      itF->setWhichPixmap( "check_grey" );
      itF->setSelected( true );
    }

    itF = itF->parentFolder;
  }
  else
  {
    UploadTreeFile *itFile = static_cast<UploadTreeFile*>(it);
    if (it->isSelected())
    {
      itFile->setWhichPixmap("check");
      itFile->setSelected(true);
    } else
    {
      itFile->setWhichPixmap("check_clear");
      itFile->setSelected(false);
    }
    itF = itFile->parentFolder;
  }

  //iterate through the item's parents and set the correct checkboxes for them
  while (itF)
  {
    bool hasSelected = false;
    bool allSelected = true;
    //check if the item has any children's selected
    QListViewItemIterator iter(itF->firstChild());
    while ( iter.current() && iter.current() != itF->nextSibling())
    {
       if ( iter.current()->isSelected() )
       {
         hasSelected = true;
       } else
       {
         allSelected = false;
       }
       ++iter;
    }
    if (hasSelected)
    {
      if (allSelected)
      {
        itF->setWhichPixmap( "check" );
      } else
      {
        itF->setWhichPixmap( "check_grey" );
      }
      itF->setSelected( true );
    } else
    {
      itF->setWhichPixmap( "check_clear" );
      itF->setSelected( false );
    }
    itF = itF->parentFolder;
  }
}

void UploadTreeView::slotSelectFile( )
{
  slotSelectFile(currentItem());
}

//TODO: This should search based on url's rather than on text(0)
UploadTreeFolder* UploadTreeView::findFolder( UploadTreeFolder *it, const QString& folderName )
{
  QListViewItem *itIter = 0;
  if (it == 0) itIter = firstChild();
  else itIter = it->firstChild();

  for( ; itIter != 0; itIter = itIter->nextSibling() )
  {
    if ( itIter->text(0) == folderName )
    {
      return (UploadTreeFolder *)itIter;
    }
  }
  return 0;
}

UploadTreeFolder* UploadTreeView::printTree( UploadTreeFolder *it = 0,const QString& indent = QString::null )
{
  QListViewItem *itIter = 0;
  if (it == 0) itIter = firstChild();
  else itIter = it->firstChild();

  for( ; itIter != 0; itIter = itIter->nextSibling() )
  {
//    cout << indent << itIter->text(0) << endl;
    if ( dynamic_cast<UploadTreeFolder *>(itIter) )
      printTree( (UploadTreeFolder *)itIter, indent + "  " );
  }
  return 0;
}

// :NOTE: AFAIK, safe to use only if you are sure the item searched for
// is already in here.  It might be safe otherwise, but use at your own
// peril.
QListViewItem* UploadTreeView::findItem(const QString& path )
{
  QString item = path;
  UploadTreeFolder *it = 0;
  int i;
  while ( ( i = item.find('/') ) >= 0 )
  {
    it = findFolder( it, item.left(i) );
    if ( it == 0 )
         return 0;
    item.remove(0,i+1);
  }

  QListViewItem *itIter = 0;
  if ( it == 0 ) itIter = firstChild();
  else itIter = it->firstChild();

  for( ; itIter != 0; itIter = itIter->nextSibling() )
  {
    if ( itIter->text(0) == item )
    {
      return itIter;
    }
  }
  return 0;
}

UploadTreeFile* UploadTreeView::addItem(const KURL &a_url, const KFileItem& a_fileItem)
{
  QString item = a_url.path(); //TODO: do with real KURL's
  QString fname = item;
  int i;
  uint col = 0;
  UploadTreeFolder *it = 0;
  KURL u;
  while ( ( i = item.find('/', col) ) >= 0 )
  {
    if ( i!=0 )
    {
      UploadTreeFolder *itTemp = findFolder(it, item.mid(col, i - col));
      if ( itTemp == 0 )
      {
        u = a_url;
        QuantaCommon::setUrl(u,item.left(i)+"/");
        if ( it == 0 )
        {
          it = new UploadTreeFolder(u, this, "");
        }
        else {
          it = new UploadTreeFolder(u, it, "");
        }
      }
      else
      {
        it = itTemp;
      }
    }
    col = i + 1;
  }
  UploadTreeFile *file = 0;
  if ( col < item.length() )
  {
    if (it == 0)
    {
    file = new UploadTreeFile(this, a_url, a_fileItem);
    }
    else
    {
      file = new UploadTreeFile(it, a_url, a_fileItem);
    }
  }
  return file;
}

void UploadTreeView::expandAll( QListViewItem *it )
{
  QListViewItem *itIter = it;
  if (it == 0) itIter = firstChild();
  else itIter = it->firstChild();

  for( ; itIter != 0; itIter = itIter->nextSibling() )
  {
    if ( dynamic_cast<UploadTreeFolder *>(itIter) )
    {
      itIter->setOpen( true );
      expandAll( itIter );
    }
  }
}

void UploadTreeView::collapseAll( QListViewItem *it )
{
  QListViewItem *itIter = it;
  if (it == 0) itIter = firstChild();
  else itIter = it->firstChild();

  for( ; itIter != 0; itIter = itIter->nextSibling() )
  {
    if ( dynamic_cast<UploadTreeFolder *>(itIter) )
    {
      itIter->setOpen( false );
      expandAll( itIter );
    }
  }
}

void UploadTreeView::invertAll( QListViewItem *it )
{
  QListViewItem *itIter = it;
  if (it == 0) itIter = firstChild();
  else itIter = it->firstChild();

  for( ; itIter != 0; itIter = itIter->nextSibling() )
  {
    if ( dynamic_cast<UploadTreeFile *>(itIter) ||
       ( dynamic_cast<UploadTreeFolder *>(itIter) &&
         !itIter->firstChild()) )
    {
      itIter->setSelected( !itIter->isSelected() );
    }
    else
    {
      invertAll( itIter );
    }
  }
}

void UploadTreeView::selectAll( bool select )
{
  QListViewItemIterator it(this);

  for ( ; it.current(); ++it )
  {
    if ( dynamic_cast<UploadTreeFile *>(it.current()) )
    {
      it.current()->setSelected( select);
    } else
    if ( dynamic_cast<UploadTreeFolder *>(it.current()) &&
         !it.current()->firstChild() )
    {
      it.current()->setSelected( select);
    }
  }
}


/** No descriptions */
QListViewItem* UploadTreeView::itemByUrl(const KURL& a_url)
{
  QListViewItemIterator it(this);
  UploadTreeFile *fileItem;
  UploadTreeFolder *folderItem;
  KURL url;
  for ( ; it.current(); ++it )
  {
    if ( (fileItem = dynamic_cast<UploadTreeFile *>(it.current()) ) !=0)
    {
      url = fileItem->m_url;
    } else
    {
      folderItem = static_cast<UploadTreeFolder*>(it.current());
      url = folderItem->m_url;
    }
    if (url == a_url)
    {
      return it.current();
    }
  }

  return 0L;
}

void UploadTreeView::slotDoubleClicked(QListViewItem *item, const QPoint &, int )
{
  item->setOpen(!item->isOpen());
}

#include "uploadtreeview.moc"
