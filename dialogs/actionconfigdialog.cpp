/***************************************************************************
                             actionconfigdialog.cpp
                             ---------------------
    copyright            : (C) 2003 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/

//qt includes
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qdom.h>
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qmultilineedit.h>
#include <qradiobutton.h>
#include <qtabwidget.h>
#include <qregexp.h>

//kde includes
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kicondialog.h>
#include <kiconloader.h>
#include <kkeybutton.h>
#include <klistview.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kpushbutton.h>
#include <kstandarddirs.h>
#include <kdeversion.h>
#include <kinputdialog.h>
#include <ktoolbar.h>

//app includes
#include "resource.h"
#include "filecombo.h"

#include "actionconfigdialog.h"
#include "tagaction.h"
#include "toolbartabwidget.h"

ActionConfigDialog::ActionConfigDialog(const QDict<ToolbarEntry> &toolbarList, KMainWindow* parent, const char* name, bool modal, WFlags fl, const QString& defaultAction )
    :ActionConfigDialogS( parent, name, modal, fl )
{
  m_mainWindow = parent;
  m_toolbarList = toolbarList;
  buttonOk->setIconSet(SmallIconSet("button_ok"));
  buttonCancel->setIconSet(SmallIconSet("button_cancel"));
  buttonApply->setIconSet(SmallIconSet("apply"));
  //buttonHelp->setIconSet(SmallIconSet("help"));

  currentAction = 0L;
  m_toolbarItem = 0L;

  connect(actionTreeView, SIGNAL(contextMenu(KListView *,QListViewItem *,const QPoint &)),
                          SLOT(slotContextMenu(KListView *,QListViewItem *,const QPoint &)));
  connect(actionTreeView, SIGNAL(selectionChanged(QListViewItem *)),
                          SLOT(slotSelectionChanged(QListViewItem *)));
  connect(shortcutKeyButton, SIGNAL(capturedShortcut(const KShortcut &)),
                             SLOT(slotShortcutCaptured(const KShortcut &)));
  connect(scriptPath, SIGNAL(activated(const QString&)),
                      SLOT(slotTextChanged(const QString&)));
  connect(this, SIGNAL(addToolbar()), m_mainWindow, SLOT(slotAddToolbar()));
  connect(this, SIGNAL(removeToolbar(const QString&)), m_mainWindow, SLOT(slotRemoveToolbar(const QString&)));
  connect(this, SIGNAL(deleteUserAction(KAction*)), m_mainWindow, SLOT(slotDeleteAction(KAction*)));
  connect(this, SIGNAL(configureToolbars(const QString&)), m_mainWindow, SLOT(slotConfigureToolbars(const QString&)));
//fill up the tree view with the toolbars and actions
  actionTreeView->setSorting(-1);
  allActionsItem = new KListViewItem(actionTreeView, i18n("All"));
  actionTreeView->insertItem(allActionsItem);
  QListViewItem *item, *oldItem = allActionsItem;
  KAction *action;
  QString toolbarName;
  QString toolbarId;
  ToolbarTabWidget *tb = ToolbarTabWidget::ref();
  QRegExp r("\\&(?!\\&)");
  KActionCollection *ac = m_mainWindow->actionCollection();
  for (int i = 0; i < tb->count(); i++)
  {
    toolbarName = tb->label(i);
    toolbarName.replace(r, "");
    item = new KListViewItem(actionTreeView, oldItem, i18n(toolbarName.utf8()));
    actionTreeView->insertItem(item);

    toolbarId = tb->id(i);
    QListViewItem *oldActionItem = 0L;
    ToolbarEntry *p_toolbar = m_toolbarList[toolbarId];
    if (p_toolbar)
    {
      QDomNode node = p_toolbar->guiClient->domDocument().firstChild().firstChild().firstChild();
      while (!node.isNull())
      {
        if (node.nodeName() == "Action")
        {
          action = ac->action(node.toElement().attribute("name"));
          if (action)
          {
            oldActionItem = new KListViewItem(item, oldActionItem, action->text().replace(r,""), action->shortcut().toString(), action->name());
            oldActionItem->setPixmap(0, SmallIcon(action->icon()) );
          }
        }
        node = node.nextSibling();
      }
      if (tb->label(tb->currentPageIndex()).replace(r, "") == toolbarName)
      {
         item->setOpen(true);
         if (item->firstChild())
         {
           if (defaultAction.isEmpty())
           {
             actionTreeView->setCurrentItem(item->firstChild());
           } else
           {
             QListViewItem *actionItem = item->firstChild();
             while (actionItem && actionItem->depth() > 0)
             {
                if (actionItem->text(0) == defaultAction || actionItem->text(0) == defaultAction + "...")
                {
                  actionTreeView->setCurrentItem(actionItem);
                  break;
                }
                actionItem = actionItem->nextSibling();
             }
           }
         }
      }
    }
    oldItem = item;
  }
  uint acCount = ac->count();
  for (uint i = 0; i < acCount; i++)
  {
    action = ac->action(i);
    item = new KListViewItem(allActionsItem, action->text().replace(r, ""),  action->shortcut().toString(), action->name());
    item->setPixmap(0, SmallIcon(action->icon()) );
  }
  allActionsItem->sortChildItems(0, true);

  treeMenu = new KPopupMenu(actionTreeView);
  treeMenu->insertItem(i18n("&Add New Toolbar"), this, SLOT(slotAddToolbar()));
  treeMenu->insertItem(i18n("&Remove Toolbar"), this, SLOT(slotRemoveToolbar()));
  treeMenu->insertItem(i18n("&Edit Toolbar"), this, SLOT(slotEditToolbar()));

  globalShortcuts = KGlobal::config()->entryMap( "Global Shortcuts" );
}

ActionConfigDialog::~ActionConfigDialog()
{
}

void ActionConfigDialog::slotAddToolbar()
{
  emit addToolbar();
  QString toolbarName;
  QListViewItem *item;
  ToolbarTabWidget *tb = ToolbarTabWidget::ref();
  for (int i = 0; i < tb->count(); i++)
  {
    toolbarName = tb->label(i);
    if (!actionTreeView->findItem(toolbarName, 0))
    {
      item = actionTreeView->lastItem();
      if (item->parent())
        item = item->parent();
      new KListViewItem(actionTreeView, item, i18n(toolbarName.utf8()));
      break;
    }
  }
}

void ActionConfigDialog::slotRemoveToolbar()
{
  QListViewItem *item = actionTreeView->currentItem();
  QString s = item->text(0);
  if (item->parent())
  {
    item = item->parent();
    s = item->text(0);
  }
  if (s != i18n("All"))
  {
      if ( KMessageBox::warningContinueCancel(this, i18n("Do you really want to remove the \"%1\" toolbar?").arg(s),QString::null,KStdGuiItem::del()) == KMessageBox::Continue )
    {
      m_toolbarItem = item;
      connect(m_mainWindow, SIGNAL(toolbarRemoved(const QString&)), SLOT(slotToolbarRemoved(const QString&)));
      emit removeToolbar(s.lower());
    }
  }
}

void ActionConfigDialog::slotToolbarRemoved(const QString &/*name*/)
{
  actionTreeView->setCurrentItem(allActionsItem);
  delete m_toolbarItem;
  m_toolbarItem = 0L;
  disconnect(m_mainWindow, SIGNAL(toolbarRemoved(const QString&)), this, SLOT(slotToolbarRemoved(const QString&)));
}

void ActionConfigDialog::slotEditToolbar()
{
  QString toolbarName;
  QString toolbarId;
  QListViewItem *oldItem;
  QListViewItem *item = actionTreeView->currentItem();
  if (item->parent())
     item = item->parent();
  toolbarName = item->text(0);
  if ( toolbarName != i18n("All"))
  {
    emit configureToolbars(toolbarName +" <quanta>");

    //update the tree view
    KAction *action;
    KActionCollection *ac = m_mainWindow->actionCollection();
    ToolbarTabWidget *tb = ToolbarTabWidget::ref();
    for (int i = 0; i < tb->count(); i++)
    {
      toolbarName = tb->label(i);
      toolbarId = tb->id(i);
      ToolbarEntry *p_toolbar = m_toolbarList[toolbarId];
      if (p_toolbar)
      {
        oldItem = actionTreeView->findItem(toolbarName, 0);
        item = new KListViewItem(actionTreeView, oldItem, toolbarName);
        item->setOpen(oldItem->isOpen());
        delete oldItem;
        QDomNode node = p_toolbar->guiClient->domDocument().firstChild().firstChild().firstChild();
        while (!node.isNull())
        {
          if (node.nodeName() == "Action")
          {
            action = ac->action(node.toElement().attribute("name"));
            if (action)
            {
              oldItem = new KListViewItem(item, oldItem, action->text().replace(QRegExp("\\&(?!\\&)"),""), action->shortcut().toString(), action->name());
              oldItem->setPixmap(0, SmallIcon(action->icon()));
            }
          }
          node = node.nextSibling();
        }
      }
    }
    actionTreeView->setCurrentItem(allActionsItem);
    actionTreeView->setSelected(allActionsItem, true);
  }
}

void ActionConfigDialog::slotContextMenu(KListView *,QListViewItem *,const QPoint &point)
{
  treeMenu->popup(point);
}

void ActionConfigDialog::slotSelectionChanged(QListViewItem *item)
{
  if (currentAction && currentAction->inherits("TagAction"))
  {
    if ( buttonApply->isEnabled() &&
         KMessageBox::questionYesNo(this, i18n("Do you want to save the changes made to this action?"), QString::null, KStdGuiItem::save(), KStdGuiItem::discard()) == KMessageBox::Yes )
    {
      saveCurrentAction();
    }
    buttonApply->setEnabled(false);
    currentAction = 0L;
  }
  if (item && item->depth() > 0)
  {
    TagAction *action = 0L;
    KActionCollection *ac = m_mainWindow->actionCollection();
    uint acCount = ac->count();
//find the corresponding action
    for (uint i = 0; i < acCount; i++)
    {
      KAction *a = ac->action(i);
      QString actionName = a->name();
      if (a && actionName == item->text(2) && a->inherits("TagAction"))
      {
        action = static_cast<TagAction*>(a);
        actionProperties->setEnabled(true);
        deleteAction->setEnabled(true);
        break;
      } else
      {
        actionProperties->setEnabled(false);
        deleteAction->setEnabled(false);
      }
    }

    //if we found the action, load the action details
    if (action)
    {
      currentAction = action;
      QDomElement el = action->data();
      if ( el.hasAttribute("icon") )
      {
        QString s = el.attribute("icon");
        if (!QFileInfo(s).exists())
          s = QFileInfo(s).fileName();
        actionIcon->setIcon(s);
      }
      QString actionText = el.attribute("text");
      actionText.replace(QRegExp("\\&(?!\\&)"),"");
      lineText->setText(actionText);
      lineToolTip->setText( el.attribute("tooltip") );
      selectedShortcut = action->shortcut();
      QString shortcutText = action->shortcut().toString();
      if (shortcutText.isEmpty())
      {
        noShortcut->setChecked(true);
        shortcutKeyButton->setText(i18n("None"));
      } else
      {
        customShortcut->setChecked(true);
        shortcutKeyButton->setShortcut(action->shortcut(), false);
        shortcutKeyButton->setText(shortcutText);
      }

//find the container toolbars of this action and add them to the container listbox
      toolbarListBox->clear();
      int current = 0;
      int count = 0;
      ToolbarTabWidget *tb = ToolbarTabWidget::ref();
      for (int i = 0; i < tb->count(); i++)
      {
        QString toolbarName = tb->label(i);
        QString toolbarId = tb->id(i);
        ToolbarEntry *p_toolbar = m_toolbarList[toolbarId];
        if (p_toolbar)
        {
          QDomNode node = p_toolbar->guiClient->domDocument().firstChild().firstChild().firstChild();
          while (!node.isNull())
          {
            if (node.nodeName() == "Action" &&
                node.toElement().attribute("name") == el.attribute("name"))
            {
              toolbarListBox->insertItem(toolbarName);
              if (item->parent()->text(0) == toolbarName)
                current = count;
              count++;
            }
            node = node.nextSibling();
          }
        }
      }
      toolbarListBox->setCurrentItem(current);
      toolbarListBox->setSelected(current, true);
      toolbarListBox->sort();

//set the detailed settings pages to their defaults
      lineTag->clear();
      lineClosingTag->clear();
      useClosingTag->setChecked(false);
      useActionDialog->setChecked(false);
      scriptPath->setText("");
      inputBox->setCurrentItem(0);
      outputBox->setCurrentItem(5);
      errorBox->setCurrentItem(5);
      textEdit->clear();
//set the detailed settings according to the type of the action
      QString type = el.attribute("type", "tag");
      if (type == "tag")
      {
        QDomElement eltag = el.namedItem("tag").toElement();
        lineTag->setText( eltag.text() );
        useActionDialog->setChecked(eltag.attribute("useDialog","false") == "true");
        eltag = el.namedItem("xtag").toElement();
        lineClosingTag->setText( eltag.text() );
        useClosingTag->setChecked( eltag.attribute("use","false") == "true" );
        typeCombo->setCurrentItem(0);
      }
      else if (type == "script")
      {
        QDomElement elScript = el.namedItem("script").toElement();
        scriptPath->setText( elScript.text() );

        QString s;
        s = elScript.attribute("input","none");
        if (s == "current")
        {
          inputBox->setCurrentItem(1);
        } else
        if (s == "selected")
        {
          inputBox->setCurrentItem(2);
        } else
        {
          inputBox->setCurrentItem(0);
        }

        s = elScript.attribute("output","none");
        if (s == "cursor")
        {
          outputBox->setCurrentItem(1);
        } else
        if (s == "selection")
        {
          outputBox->setCurrentItem(2);
        } else
        if (s == "replace")
        {
          outputBox->setCurrentItem(3);
        } else
        if (s == "new")
        {
          outputBox->setCurrentItem(4);
        } else
        if (s == "message")
        {
          outputBox->setCurrentItem(5);
        } else
        {
          outputBox->setCurrentItem(0);
        }

        s = elScript.attribute("error","none");
        if (s == "cursor")
        {
          errorBox->setCurrentItem(1);
        } else
        if (s == "selection")
        {
          errorBox->setCurrentItem(2);
        } else
        if (s == "replace")
        {
          errorBox->setCurrentItem(3);
        } else
        if (s == "new")
        {
          errorBox->setCurrentItem(4);
        } else
        if (s == "message")
        {
          errorBox->setCurrentItem(5);
        } else
        {
          errorBox->setCurrentItem(0);
        }
        typeCombo->setCurrentItem(1);
      }
      else if (type == "text")
      {
        textEdit->setText( el.namedItem("text").toElement().text() );
        typeCombo->setCurrentItem(2);
      }
      buttonApply->setEnabled(false);
    }
  } else
  {
    actionProperties->setEnabled(false);
    deleteAction->setEnabled(false);
  }
}

void ActionConfigDialog::saveCurrentAction()
{
  static_cast<TagAction *>(currentAction)->setModified(true);
  QString s;
  QDomElement el = static_cast<TagAction *>(currentAction)->data();
  s = actionIcon->icon();
  el.setAttribute("icon", s);
  currentAction->setIcon(s);
  QString oldText = el.attribute("text");
  s = lineText->text();
  s.replace('&', "&&");
  el.setAttribute("text", s);
  currentAction->setText(s);
  s = lineToolTip->text();
  el.setAttribute("tooltip", s);
  currentAction->setToolTip(s);
  s = "";
  if (customShortcut->isChecked())
  {
    s = selectedShortcut.toString();
    currentAction->setShortcut(selectedShortcut);
  } else
  {
    currentAction->setShortcut(KShortcut());
  }
  el.setAttribute("shortcut", s);


//update the tree view
  QListViewItem *listItem;
  QListViewItemIterator it(actionTreeView);
  while (it.current())
  {
    listItem = it.current();
    if (listItem->depth() > 0 && listItem->text(2) == currentAction->name())
    {
      listItem->setPixmap(0, SmallIcon(actionIcon->icon()));
      listItem->setText(0, lineText->text());
      listItem->setText(1, currentAction->shortcut().toString());
    }
    ++it;
  }

//remove all the detailed settings
  QDomElement item = el.namedItem("tag").toElement();
  if ( !item.isNull() )
      el.removeChild(item);
  item = el.namedItem("tag").toElement();
  if ( !item.isNull() )
      el.removeChild(item);
  item = el.namedItem("xtag").toElement();
  if ( !item.isNull() )
      el.removeChild(item);
  item = el.namedItem("script").toElement();
  if ( !item.isNull() )
      el.removeChild(item);
  item = el.namedItem("text").toElement();
  if ( !item.isNull() )
      el.removeChild(item);
//add the new detailed settings
  QDomDocument document = QDomDocument();
  int type = typeCombo->currentItem();
  switch (type)
  {
    case 1:{
        el.setAttribute("type","script");
        item = document.createElement("script");
        switch (inputBox->currentItem())
        {
          case 1:{ item.setAttribute("input", "current");
                   break;
                 }
          case 2:{ item.setAttribute("input", "selected");
                   break;
                 }
          default:{ item.setAttribute("input", "none");
                    break;
                  }
        }
        switch (outputBox->currentItem())
        {
          case 1:{ item.setAttribute("output", "cursor");
                   break;
                 }
          case 2:{ item.setAttribute("output", "selection");
                   break;
                 }
          case 3:{ item.setAttribute("output", "replace");
                   break;
                 }
          case 4:{ item.setAttribute("output", "new");
                   break;
                 }
          case 5:{ item.setAttribute("output", "message");
                   break;
                 }
          default:{ item.setAttribute("output", "none");
                    break;
                  }
        }
        switch (errorBox->currentItem())
        {
          case 1:{ item.setAttribute("error", "cursor");
                   break;
                 }
          case 2:{ item.setAttribute("error", "selection");
                   break;
                 }
          case 3:{ item.setAttribute("error", "replace");
                   break;
                 }
          case 4:{ item.setAttribute("error", "new");
                   break;
                 }
          case 5:{ item.setAttribute("error", "message");
                   break;
                 }
          default:{ item.setAttribute("error", "none");
                    break;
                  }
        }

        el.appendChild(item);
        item.appendChild(document.createTextNode(scriptPath->text()));
        break;
      }
    case 2:{
        el.setAttribute("type","text");
        item = document.createElement("text");
        el.appendChild(item);
        item.appendChild(document.createTextNode(textEdit->text()));
        break;
      }
    default:{
        el.setAttribute("type","tag");
        item = document.createElement("tag");
        item.setAttribute("useDialog", useActionDialog->isChecked() ? "true" : "false");
        el.appendChild(item);
        item.appendChild(document.createTextNode(lineTag->text()));
        item = document.createElement("xtag");
        item.setAttribute("use", useClosingTag->isChecked() ? "true" : "false");
        el.appendChild(item);
        item.appendChild(document.createTextNode(lineClosingTag->text()));
        break;
      }
  }
  ToolbarTabWidget *tb = ToolbarTabWidget::ref();
  for (int i = 0; i < tb->count(); i++)
  {
    QString toolbarName = tb->label(i);
    QString toolbarId = tb->id(i);
    ToolbarEntry *p_toolbar = m_toolbarList[toolbarId];
    bool isOnToolbar = false;
    if (p_toolbar)
    {
      QDomNode node = p_toolbar->guiClient->domDocument().firstChild().firstChild().firstChild();
      bool placeOnToolbar = toolbarListBox->findItem(toolbarName, Qt::ExactMatch);
      while (!node.isNull())
      {
        if (node.nodeName() == "Action" &&
            node.toElement().attribute("name") == el.attribute("name"))
        {
          //if it's present in the toolbar, but not in the container list,
          //remove it also from the toolbar
          if (!placeOnToolbar)
          {
            currentAction->unplug(tb->page(i));
            currentAction->unplug(p_toolbar->menu);
            node.parentNode().removeChild(node);
            QListViewItemIterator iter(actionTreeView);
            while (iter.current())
            {
              listItem = iter.current();
              if (listItem->depth() > 0 && listItem->parent()->text(0) == toolbarName
                  && listItem->text(2) == el.attribute("name"))
              {
                delete listItem;
                break;
              }
              ++iter;
            }
          }
          isOnToolbar = true;
          break;
        }
        node = node.nextSibling();
      }
      //it's not on the toolbar, but it should be
      if (!isOnToolbar && placeOnToolbar)
      {
        currentAction->plug(tb->page(i));
        currentAction->plug(p_toolbar->menu);
        item = p_toolbar->guiClient->domDocument().createElement("Action");
        item.setAttribute("name",el.attribute("name"));
        p_toolbar->guiClient->domDocument().firstChild().firstChild().appendChild(item);
      //put it also in the treeview
        listItem = actionTreeView->findItem(toolbarName, 0);
        if (listItem)
        {
          QListViewItem *after = listItem->firstChild();
          while ( after && (!after->nextSibling() || (after->nextSibling() && after->nextSibling()->depth()!=0 ) ))
          {
            if (after->text(2) == currentAction->name())
            {
                placeOnToolbar = false;
                break;
            }
            after = after->nextSibling();
          }
          if (placeOnToolbar)
          {
              listItem = new KListViewItem(listItem, after, lineText->text(), currentAction->shortcut().toString(), currentAction->name());
              listItem->setPixmap(0, SmallIcon(actionIcon->icon()));
          }
        }
      }
      KXMLGUIFactory::saveConfigFile(p_toolbar->guiClient->domDocument(),
        p_toolbar->guiClient->xmlFile(), p_toolbar->guiClient->instance());
    }
    QWidget *toolBar = tb->page(i);
    if (toolBar->minimumSizeHint().height() > 20)
    {
      toolBar->adjustSize();
      toolBar->setGeometry(0,0, tb->width(), toolBar->height());
    } else
    {
      toolBar->setGeometry(0,0, tb->width(), tb->height() - tb->tabHeight());
    }

  }
}

void ActionConfigDialog::slotShortcutCaptured(const KShortcut &shortcut)
{
  QString shortcutText = shortcut.toString();
  QString shortcutText2;
  int pos = shortcutText.find(';');
  if (pos != -1)
  {
    shortcutText2 = shortcutText.mid(pos + 1);
    shortcutText = shortcutText.left(pos);
  }
  QString s = shortcutText;
  s.replace('+', "\\+");
  QRegExp shortcutRx("\\(" + s + "\\)|" + s + "$|" + s + ";");
  s = shortcutText2;
  s.replace('+', "\\+");
  QRegExp shortcutRx2("\\(" + s + "\\)|" + s + "$|" + s + ";");
  QString global;
//check for conflicting global shortcuts
  QMap<QString, QString>::Iterator it;
  for ( it = globalShortcuts.begin(); it != globalShortcuts.end(); ++it )
  {
    if (it.data().contains(shortcutRx))
    {
      global = it.key();
      break;
    }
    if (!shortcutText2.isEmpty() && it.data().contains(shortcutRx2))
    {
      shortcutText = shortcutText2;
      global = it.key();
      break;
    }
  }

  if (global.isEmpty())
  {
    QPtrList<KXMLGUIClient> clients = m_mainWindow->guiFactory()->clients();
    for( QPtrListIterator<KXMLGUIClient> it( clients ); it.current(); ++it )
    {
        KActionCollection *ac = (*it)->actionCollection();
        for (uint i = 0; i < ac->count(); i++)
        {
          KAction *action = ac->action(i);
          if (action != currentAction && action->shortcut().toString().contains(shortcutRx))
          {
            global = action->text();
            break;
          }
          if (!shortcutText2.isEmpty() && action != currentAction &&  action->shortcut().toString().contains(shortcutRx))
          {
            shortcutText = shortcutText2;
            global = action->text();
            break;
          }
        }
         if (!global.isEmpty())
           break;
      }
  }

  if (global.isEmpty())
  {
    shortcutKeyButton->setText(shortcutText);
    buttonApply->setEnabled(true);
    selectedShortcut = shortcut;
  } else
  {
    global.replace('&',"");
    QString s =  i18n("The '%1' key combination has already been allocated "
                "to the \"%2\" action.\n"
                "Please choose a unique key combination.").
                arg(shortcutText).arg(global);
    KMessageBox::sorry( this, s, i18n("Conflicting Shortcuts"));
  }
}

void ActionConfigDialog::accept()
{
  if (buttonApply->isEnabled())
      saveCurrentAction();
  ActionConfigDialogS::accept();
}

void ActionConfigDialog::reject()
{
  if (buttonApply->isEnabled() && KMessageBox::questionYesNo(this, i18n("Do you want to save the changes made to this action?"), QString::null, KStdGuiItem::save(), KStdGuiItem::discard()) == KMessageBox::Yes  )
      saveCurrentAction();
  ActionConfigDialogS::reject();
}

void ActionConfigDialog::slotNewAction()
{
  QDomDocument doc;
  QDomElement el = doc.createElement("action");
  el.setAttribute( "name", "user_"+KApplication::randomString(10) );
  el.setAttribute( "icon", "ball" );

  currentAction = new TagAction(&el, m_mainWindow);

  //add the actions to every toolbar xmlguiclient
  QDictIterator<ToolbarEntry> it(m_toolbarList);
  while (it.current())
  {
    it.current()->guiClient->actionCollection()->insert(currentAction);
    ++it;
  }

  selectedShortcut = KShortcut();
  static_cast<TagAction*>(currentAction)->setModified(true);
  QListViewItem *currentItem = actionTreeView->currentItem();
  QListViewItem *item = new KListViewItem(allActionsItem);
  QString actionText = QString("Action_%1").arg(m_mainWindow->actionCollection()->count());
  currentAction->setText(actionText);
  item->setText(2, currentAction->name());
  item->setText(0, actionText);
  item->setPixmap(0, SmallIcon("ball"));
  allActionsItem->sortChildItems(0, true);
  if (currentItem->parent() && currentItem->parent() == allActionsItem)
  {
     actionTreeView->setCurrentItem(item);
  } else
  {
    QListViewItem *parentItem = currentItem->parent();
    if (!parentItem)
      parentItem = currentItem;

    item = new KListViewItem(parentItem, currentItem);
    item->setText(0, actionText);
    item->setText(2, currentAction->name());
    item->setPixmap(0, SmallIcon("ball"));
    actionTreeView->setCurrentItem(item);
    if (parentItem != allActionsItem)
    {
      toolbarListBox->insertItem(parentItem->text(0));
      toolbarListBox->setCurrentItem(0);
      toolbarListBox->setSelected(0, true);
    }
  }
  actionTreeView->ensureItemVisible(item);
  buttonApply->setEnabled(true);
}

void ActionConfigDialog::slotDeleteAction()
{
    if ( KMessageBox::warningContinueCancel(this, i18n("<qt>Removing the action removes all the references to it.\nAre you sure you want to remove the <b>%1</b> action?</qt>").arg(currentAction->text()),QString::null,KStdGuiItem::del()) == KMessageBox::Continue )
  {
    QString actionName = currentAction->name();
    emit deleteUserAction(currentAction);
    currentAction = 0L;
    //update the tree view
    QListViewItem *listItem;
    QListViewItemIterator it(actionTreeView);
    while (it.current())
    {
      listItem = it.current();
      if (listItem->depth() > 0 && listItem->text(2) == actionName)
      {
        ++it;
        delete listItem;
      } else
      {
        ++it;
      }
    }
    slotSelectionChanged(actionTreeView->currentItem());
  }
}

void ActionConfigDialog::slotApply()
{
  saveCurrentAction();
  buttonApply->setEnabled(false);
}

void ActionConfigDialog::slotAddContainerToolbar()
{
  ToolbarTabWidget *tb = ToolbarTabWidget::ref();
  int i;

  QStringList lst;
  for (i = 0; i < tb->count(); i++)
  {
    if (!toolbarListBox->findItem(tb->label(i), Qt::ExactMatch))
        lst << tb->label(i);
  }

  if (lst.count() > 0)
  {
    bool ok = false;
    QString res = KInputDialog::getItem(
                    i18n( "Add Action to Toolbar" ),
                    i18n( "Please select a toolbar:" ), lst, 0, false, &ok, this );

    if (ok)
    {
      toolbarListBox->insertItem(res);
      toolbarListBox->sort();
      buttonApply->setEnabled(true);
    }
  }
}

void ActionConfigDialog::slotRemoveContainerToolbar()
{
  int item = toolbarListBox->currentItem();
  toolbarListBox->removeItem(item);
  item = (item > 0) ? item - 1 : 0;
  toolbarListBox->setCurrentItem(item);
  toolbarListBox->setSelected(item, true);
  buttonApply->setEnabled(true);
}

void ActionConfigDialog::slotToggled(bool)
{
  buttonApply->setEnabled(true);
}

void ActionConfigDialog::slotTextChanged()
{
  buttonApply->setEnabled(true);
}

void ActionConfigDialog::slotTextChanged(const QString&)
{
  buttonApply->setEnabled(true);
}

void ActionConfigDialog::createScriptAction(const QString& a_name, const QString& a_script)
{
  actionTreeView->setCurrentItem(allActionsItem);
  slotNewAction();
  typeCombo->setCurrentItem(1);
  scriptPath->setText(a_script);
  lineText->setText(a_name);
}

#include "actionconfigdialog.moc"
