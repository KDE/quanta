/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "structuretreewidget.h"
#include "structtreeitem.h"
#include "parseresult.h"
#include "areastruct.h"
#include "settings.h"
#include "node.h"
#include "editorsource.h"
#include "quantacoreif.h"

#include <q3header.h>
#include <qpoint.h>
#include <QClipboard>
#include <QApplication>

#include <kurl.h>
// #include <klibloader.h>
// #include <kparts/part.h>
#include <klocale.h>
#include <kmenu.h>
#include <kaction.h>
#include <kstdaction.h>
// #include <kdebug.h>

//kdevelop includes
#include <interfaces/kdevcore.h>
#include <interfaces/kdevplugin.h>


StructureTreeWidget::StructureTreeWidget(KDevPlugin *plugin, QWidget *parent)
  : K3ListView(parent), m_popupMenu(0), m_dirty(false), m_populated(0), m_parseResult(0), m_plugin(plugin)
{
  setTreeStepSize(15);
  setRootIsDecorated(true);
  header()->hide();
  setSorting(-1, false);
  setAcceptDrops(false); // TODO no drag&drop yet
  setDropVisualizer(true);
  setDragEnabled(false); // TODO no drag&drop yet
  setSelectionModeExt(Single);
  addColumn(i18n("Name"), -1);
  addColumn("");
  setFullWidth(true);
  setFocusPolicy(Qt::ClickFocus);

  connect(this, SIGNAL(expanded(Q3ListViewItem *)), SLOT(slotExpanded(Q3ListViewItem *)));

  connect(this, SIGNAL(contextMenu(K3ListView *, Q3ListViewItem *, const QPoint &)), SLOT(slotContextMenu(K3ListView *, Q3ListViewItem *, const QPoint &)));

  connect(this, SIGNAL(doubleClicked(Q3ListViewItem *)), SLOT(slotDoubleClicked(Q3ListViewItem *)));

  connect(this, SIGNAL(mouseButtonPressed(int, Q3ListViewItem*, const QPoint&, int)),SLOT(slotMouseClicked(int, Q3ListViewItem*, const QPoint&, int)));

  m_qcore = plugin->extension<QuantaCoreIf>("KDevelop/Quanta");
}


StructureTreeWidget::~StructureTreeWidget()
{
}


void StructureTreeWidget::slotBuild(const ParseResult *parseResult)
{
  m_populated = false;
  if (isVisible())
    return build(parseResult);

  m_dirty = true;
  m_parseResult = parseResult;

//   kDebug(24000) << "StructureTreeWidget-----------slotBuild dirty " << parseResult << endl;
}


void StructureTreeWidget::build(const ParseResult *parseResult)
{
  clear();
  setColumnWidth(0, 10); // reset the size, will grow with new content
  if (parseResult && parseResult->baseNode)
  {
    StructTreeItem * lastItem = new StructTreeItem(this, parseResult->baseNode);
    Node *node = parseResult->baseNode->next;
    while (node)
    {
      lastItem = new StructTreeItem(this, node, lastItem);
      node = node->next;
    }
  }
  slotUnblockGUI();
  m_dirty = false;
}


void StructureTreeWidget::slotExpanded(Q3ListViewItem * item)
{
  StructTreeItem * structItem = dynamic_cast<StructTreeItem *>(item);
  if (structItem)
    structItem->populate();
}


void StructureTreeWidget::slotBlockGUI()
{
 // kDebug(24000) << "slotBlockGUI" << endl;
  setEnabled(false);
};


void StructureTreeWidget::slotUnblockGUI()
{
//  kDebug(24000) << "slotUnblockGUI" << endl;
  setEnabled(true);
};


void StructureTreeWidget::slotNewSettings()
{
  if (m_dirty)
    build(m_parseResult);

  Q3ListViewItem * item = firstChild();
  while (item)
  {
    static_cast<StructTreeItem *>(item)->settingsChanged();
    item = item->nextSibling();
  }
};


void StructureTreeWidget::slotContextMenu(K3ListView * view, Q3ListViewItem * item, const QPoint & point)
{
  Q_UNUSED(view)

  // lazy init of the menus
  if (! item)
    return;

  if (! m_popupMenu)
  {
    m_popupMenu = new KMenu(this);
    m_popupMenu->addAction(i18n("Select Tag Area"), this, SLOT(slotSelectTag()));
    m_popupMenu->addAction(i18n("Go to End of Tag"), this ,SLOT(slotGotoClosingTag()));
    m_popupMenu->addSeparator();
    m_cutAction = KStdAction::cut(this, SLOT(slotCut()), 0);
    m_popupMenu->addAction(m_cutAction);
    m_copyAction = KStdAction::copy(this, SLOT(slotCopy()), 0);
    m_popupMenu->addAction(m_copyAction);
    m_pasteAction = KStdAction::paste(this, SLOT(slotPaste()), 0);
    m_popupMenu->addAction(m_pasteAction);
    m_popupMenu->addSeparator();
    m_popupMenu->addAction(i18n("Open Subtrees"), this, SLOT(slotOpenSubTree()));
    m_popupMenu->addAction(i18n("Close Subtrees"),this, SLOT(slotCloseSubTree()));
    m_popupMenu->addSeparator();
    m_followCursorId = m_popupMenu->insertItem(i18n("Follow Cursor"), this, SLOT(slotChangeFollowCursor())); //FIXME
  }

  EditorSource * es = m_qcore->activeEditorSource();
  if (! es)
  {
    m_cutAction->setEnabled(false);
    m_copyAction->setEnabled(false);
    m_pasteAction->setEnabled(false);
  } else
  {
    m_cutAction->setEnabled(es->cutAction());
    m_copyAction->setEnabled(es->copyAction());
    if (es->pasteAction())
    {
      QString text = QApplication::clipboard()->text(QClipboard::Clipboard);
      m_pasteAction->setEnabled(! text.isEmpty());
    }
    else
      m_pasteAction->setEnabled(false);
  }

  m_popupMenu->setItemChecked(m_followCursorId, Settings::self()->followCursor());
  m_popupMenu->popup(point);
}


void StructureTreeWidget::slotCloseSubTree()
{
  StructTreeItem *item = dynamic_cast<StructTreeItem *>(currentItem());
  if (item)
  {
    item->setSubTreeOpen(false);
  }
}


void StructureTreeWidget::slotOpenSubTree()
{
  StructTreeItem *item = dynamic_cast<StructTreeItem *>(currentItem());
  if (item)
  {
    item->setSubTreeOpen(true);
  }
}


void StructureTreeWidget::slotDoubleClicked(Q3ListViewItem *item)
{
/*  config->setGroup("Parser options");

  if ( config->readEntry("Double click") != i18n("nothing") )
  {
    slotSelectTag();
  } else*/
  {
    item->setOpen(!item->isOpen());
  }
}

void StructureTreeWidget::slotMouseClicked(int button, Q3ListViewItem *item, const QPoint& point, int c)
{
  if (! item)
  {
/*    if (button == Qt::RightButton)
      emptyAreaMenu->popup(point);*/
    return;
  }

//     config->setGroup("Parser options");
//
//     QString handleMBM = config->readEntry("MBM", i18n("Select Tag Area"));
//     QString handleLBM = config->readEntry("LBM", i18n("Find tag"));
//     QString handleDoubleClick = config->readEntry("Double click", i18n("Select Tag Area"));

  setSelected(item, true);

  switch (button)
  {
    case Qt::LeftButton:
    {
/*      if (handleLBM == i18n("Find Tag && Open Tree"))
           setOpen(item, !isOpen(item));
      */

//       bool const ctrlPressed = KApplication::keyboardMouseState() & Qt::ControlButton;

/*      if(ctrlPressed)
          setContiguousSelectedItems();*/

/*      if(ViewManager::ref()->activeView()->hadLastFocus() == QuantaView::VPLFocus)
          slotMouseClickedVPL(button, item, point, col);
      else
          slotGotoTag(item);*/
      gotoTag(item);
      return;
    }

    case Qt::MidButton:
    {
/*      if (handleMBM == i18n("nothing"))
           return;

      if (handleMBM == i18n("Find Tag && Open Tree"))
      {
        setOpen(item, !isOpen(item));
        setSelected(item, true);
        slotGotoTag(item);
      }

      if (handleMBM == i18n("Select Tag Area"))
           slotSelectTag();

      if (handleMBM == i18n("Go to End of Tag"))
          slotGotoClosingTag();
*/
      slotSelectTag();
    }
    default:
      return;
  }
}


void StructureTreeWidget::gotoTag(Q3ListViewItem *item)
{
  StructTreeItem *it = dynamic_cast<StructTreeItem *>(item);
  if (it)
  {
    setCursorPosition(it->tagArea(true).start);
  }
}


void StructureTreeWidget::slotSelectTag()
{
  StructTreeItem *item = dynamic_cast<StructTreeItem *>(currentItem());
  if (item)
  {
    AreaStruct area(item->tagArea());
    setCursorPosition(area.start);
    selectArea(area);
  }
}


void StructureTreeWidget::slotGotoClosingTag()
{
  StructTreeItem *item = dynamic_cast<StructTreeItem *>(currentItem());
  if (item)
  {
    AreaStruct area(item->tagArea(true, true));
    setCursorPosition(area.end);
  }
}

void StructureTreeWidget::slotNewCursorPosition(const QPoint & qp)
{
  if (!isEnabled() || !isVisible() || !Settings::self()->followCursor())
    return;

//   kDebug(24000) << "slotNewCursorPosition " << Settings::self()->followCursor() << endl;
  StructTreeItem * openItem;
  StructTreeItem * item = static_cast<StructTreeItem *>(firstChild());
  while (item)
  {
    openItem = item->makeNodeVisible(qp);
    if (openItem)
    {
      ensureItemVisible(openItem);
      return;
    }
    item = static_cast<StructTreeItem *>(item->nextSibling());
  }
}


void StructureTreeWidget::slotChangeFollowCursor()
{
  Settings::self()->setFollowCursor(! Settings::self()->followCursor());
  Settings::self()->writeConfig();
}


void StructureTreeWidget::showEvent ( QShowEvent *ev )
{
  K3ListView::showEvent(ev);
  if (m_dirty)
    build(m_parseResult);
}


void StructureTreeWidget::populate()
{
  if (m_populated)
    return;

  if (m_dirty)
    build(m_parseResult);

  Q3ListViewItem * item = firstChild();
  while (item)
  {
    static_cast<StructTreeItem *>(item)->populateAll();
    item = item->nextSibling();
  }
  m_populated = true;
}


void StructureTreeWidget::slotPaste()
{
  StructTreeItem *item = dynamic_cast<StructTreeItem *>(currentItem());
  if (! item)
    return;

  EditorSource * es = m_qcore->activeEditorSource();
  if (! es || ! es->pasteAction())
    return;

  setCursorPosition(item->tagArea().start);
  es->pasteAction()->trigger();
}


void StructureTreeWidget::slotCopy()
{
  StructTreeItem *item = dynamic_cast<StructTreeItem *>(currentItem());
  if (! item)
    return;

  EditorSource * es = m_qcore->activeEditorSource();
  if (! es || ! es->copyAction())
    return;

  AreaStruct area(item->tagArea());
  selectArea(area);
  es->copyAction()->trigger();
}


void StructureTreeWidget::slotCut()
{
  StructTreeItem *item = dynamic_cast<StructTreeItem *>(currentItem());
  if (! item)
    return;

  EditorSource * es = m_qcore->activeEditorSource();
  if (! es || ! es->cutAction())
    return;

  AreaStruct area(item->tagArea());
  selectArea(area);
  es->cutAction()->trigger();
}


void StructureTreeWidget::setCursorPosition(const QPoint & qp)
{
  EditorSource * es = m_qcore->activeEditorSource();
  if (es)
    es->setCursorPosition(qp.x(), qp.y());
}


void StructureTreeWidget::selectArea(const AreaStruct &area)
{
  EditorSource * es = m_qcore->activeEditorSource();
  if (es)
    es->selectArea(area);
}


#include "structuretreewidget.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
