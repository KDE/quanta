/***************************************************************************
 *   Copyright (C) 2005 by Jens Herden                                     *
 *   jens@kdewebdev.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "groupswidget.h"
#include "groupstreeitem.h"
#include "parseresult.h"
#include "areastruct.h"
#include "settings.h"
#include "node.h"
#include "dtdstruct.h"
#include "groupstorage.h"
#include "extfileinfo.h"
#include "quantacoreif.h"
#include "editorsource.h"

#include <q3header.h>
// #include <Q3ValueList>
#include <q3listview.h>

#include <kurl.h>
#include <klibloader.h>
#include <kparts/part.h>
#include <klocale.h>
#include <kmenu.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kstandardaction.h>

//kdevelop includes
#include <kdevcore.h>
#include <kdevplugin.h>
#include <kdevplugincontroller.h>
#include <kdevdocumentcontroller.h>


GroupsWidget::GroupsWidget(Koncrete::Plugin *plugin, QWidget *parent)
  : K3ListView(parent), m_popupMenu(0), m_dtdMenu(0), m_dirty(false), m_parseResult(0), m_plugin(plugin), m_partController(Koncrete::Core::documentController())
{
  setTreeStepSize(15);
  setRootIsDecorated(false);
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

  m_qcore = Koncrete::PluginController::self()->extension<QuantaCoreIf>("KDevelop/Quanta");

}


GroupsWidget::~GroupsWidget()
{
}


void GroupsWidget::slotBuild(const ParseResult *parseResult)
{
//   kDebug(24000) << "GroupsWidget::slotBuild " << parseResult->baseURL << endl;
  if (!isVisible())
  {
    m_dirty = true;
    m_parseResult = parseResult;
    return;
  }
  // remove the old content
  if (! parseResult)
    clear();

  slotUnblockGUI();
}


void GroupsWidget::slotExpanded(Q3ListViewItem * item)
{
  GroupsTreeItem * gItem = dynamic_cast<GroupsTreeItem *>(item);
  if (gItem && gItem->isExpandable())
    gItem->populate();
}


void GroupsWidget::slotBlockGUI()
{
 // kDebug(24000) << "slotBlockGUI" << endl;
  setEnabled(false);
}


void GroupsWidget::slotUnblockGUI()
{
//  kDebug(24000) << "slotUnblockGUI" << endl;
  setEnabled(true);
  m_dirty = false;
}


void GroupsWidget::slotNewSettings()
{
  if (m_dirty)
    build(m_parseResult);

  Q3ListViewItem * item = firstChild();
  while (item)
  {
    static_cast<GroupsTreeItem *>(item)->settingsChanged();
    item = item->nextSibling();
  }
}


void GroupsWidget::slotContextMenu(K3ListView * view, Q3ListViewItem * item, const QPoint & point)
{
  Q_UNUSED(view)

/* FIXME this function needs to be reimplemented
     if (dynamic_cast<StructTreeTag*>(item))
      {
        popupMenu->setItemVisible(openFileMenuId, static_cast<StructTreeTag*>(item)->hasOpenFileMenu);
      }*/
  // lazy init of the menus
  if (! item)
    return;

  if (! m_dtdMenu)
  {
    m_dtdMenu = new KMenu(this);
    m_dtdMenu->setTitle(i18n("Show Groups For"));
    m_dtdMenu->addAction(i18n("All Present DTEP"));
    m_dtdMenu->addSeparator();
  }
  if (! m_popupMenu)
  {
    m_popupMenu = new KMenu(this);
    m_popupMenu->addMenu(m_dtdMenu);
    m_popupMenu->addAction(i18n("Select Tag Area"), this, SLOT(slotSelectTag()));
    m_popupMenu->addSeparator();
    m_cutAction = KStandardAction::cut(this, SLOT(slotCut()), 0);
    m_popupMenu->addAction(m_cutAction);
    m_copyAction = KStandardAction::copy(this, SLOT(slotCopy()), 0);
    m_popupMenu->addAction(m_copyAction);
    m_pasteAction = KStandardAction::paste(this, SLOT(slotPaste()), 0);
    m_popupMenu->addAction(m_pasteAction);
    m_popupMenu->addSeparator();
    openFileMenuId = m_popupMenu->insertItem(i18n("Open File"), this, SLOT(slotOpenFile())); //FIXME
  }

  KUrl url = static_cast<GroupsTreeItem *>(item)->getURL();
  m_popupMenu->setItemVisible(openFileMenuId, !url.isEmpty());

  m_popupMenu->popup(point);
}


void GroupsWidget::slotDoubleClicked(Q3ListViewItem *item)
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

void GroupsWidget::slotMouseClicked(int button, Q3ListViewItem *item, const QPoint& point, int c)
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


void GroupsWidget::gotoTag(Q3ListViewItem *item)
{
  GroupsTreeItem *it = dynamic_cast<GroupsTreeItem *>(item);
  if (it)
  {
    setCursorPosition(it->tagArea(true).start);
  }
}


void GroupsWidget::slotSelectTag()
{
  GroupsTreeItem *item = dynamic_cast<GroupsTreeItem *>(currentItem());
  if (item)
  {
    AreaStruct area(item->tagArea());
    setCursorPosition(area.start);
    selectArea(area);
  }
}


void GroupsWidget::slotGroupsParsed(const ParseResult *parseResult)
{
  if (isVisible())
    return build(parseResult);

  m_dirty = true;
  m_parseResult = parseResult;
  return;
}


void GroupsWidget::build(const ParseResult *parseResult)
{
  // remove the old content
  clear();

  if (parseResult)
  {
    setColumnWidth(0, 10); // reset the size, will grow with new content
    const DTDStruct *parsingDTD;
    uint count = parseResult->dtepList.count();
    for (uint index = 0; index < count; index++)
    {
      parsingDTD = parseResult->dtepList[index];
      if (parsingDTD->family == DTDStruct::Script)
      {
        createScriptBranch(parsingDTD, parseResult->groupStorage);
      } else
      {
        createXMLBranch(parsingDTD, parseResult->groupStorage);
      }
    }
  }
  slotUnblockGUI();
}


void GroupsWidget::createXMLBranch(const DTDStruct *dtd, const GroupStorage *groupStorage)
{
  QMap<QString, XMLStructGroup>::ConstIterator end(dtd->xmlStructTreeGroups.end());
  for (QMap<QString, XMLStructGroup>::ConstIterator it = dtd->xmlStructTreeGroups.begin(); it != end; ++it)
  {
    GroupsTreeItem *item = new GroupsTreeItem(this, groupStorage->elementMapForGroup((*it).name));
    if (!(*it).icon.isEmpty())
      item->setPixmap(0, SmallIcon((*it).icon));

    if (groupStorage->elementCount((*it).name) > 0)
    {
      item->setText(0, i18n((*it).name.toUtf8()) + " [" + dtd->nickName + ']');
      item->setExpandable(true);
    } else
    {
      item->setText(0, i18n((*it).noName.toUtf8()) + " [" + dtd->nickName + ']');
    }
  }
}



void GroupsWidget::createScriptBranch(const DTDStruct *dtd, const GroupStorage *groupStorage)
{
  uint gCount = dtd->structTreeGroups.count();
  for (uint i = 0; i < gCount; i++)
  {
    const StructTreeGroup *group = &dtd->structTreeGroups[i];
    GroupsTreeItem *item = new GroupsTreeItem(this, groupStorage->elementMapForGroup(group->name));
    if (!group->icon.isEmpty())
      item->setPixmap(0, SmallIcon(group->icon));

    item->setText(0, i18n(group->name.toUtf8()) + " [" + dtd->nickName + ']');
    if(groupStorage->elementCount(group->name) > 0)
    {
      item->setExpandable(true);
    } else
    {
      item->setEnabled(false);
    }
  }
}


void GroupsWidget::showEvent ( QShowEvent *ev )
{
  K3ListView::showEvent(ev);
  if (m_dirty)
    build(m_parseResult);
}


void GroupsWidget::slotOpenFile()
{
  GroupsTreeItem *item = dynamic_cast<GroupsTreeItem *>(currentItem());
  if (!item)
    return;

  KUrl url = item->getURL();
  if (url.isEmpty())
    return;

  if (KUrl::isRelativeUrl(url.path()))
  {
    url = KUrl(m_parseResult->baseURL, url.path());
  }
  if (ExtFileInfo::exists(url))
  {
    m_partController->editDocument(url);
  }
}


void GroupsWidget::slotPaste()
{
  GroupsTreeItem *item = dynamic_cast<GroupsTreeItem *>(currentItem());
  if (!item)
    return;

  EditorSource * es = m_qcore->activeEditorSource();
  if (!es || !es->pasteAction())
    return;

  setCursorPosition(item->tagArea().start);
  es->pasteAction()->trigger();
}


void GroupsWidget::slotCopy()
{
  GroupsTreeItem *item = dynamic_cast<GroupsTreeItem *>(currentItem());
  if (! item)
    return;

  EditorSource * es = m_qcore->activeEditorSource();
  if (! es || ! es->copyAction())
    return;

  AreaStruct area(item->tagArea());
  selectArea(area);
  es->copyAction()->trigger();
}


void GroupsWidget::slotCut()
{
  GroupsTreeItem *item = dynamic_cast<GroupsTreeItem *>(currentItem());
  if (! item)
    return;

  EditorSource * es = m_qcore->activeEditorSource();
  if (! es || ! es->cutAction())
    return;

  AreaStruct area(item->tagArea());
  selectArea(area);
  es->cutAction()->trigger();
}


void GroupsWidget::setCursorPosition(const QPoint & qp)
{
  EditorSource * es = m_qcore->activeEditorSource();
  if (es)
    es->setCursorPosition(qp.x(), qp.y());
}


void GroupsWidget::selectArea(const AreaStruct &area)
{
  EditorSource * es = m_qcore->activeEditorSource();
  if (es)
    es->selectArea(area);
}

#include "groupswidget.moc"

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
