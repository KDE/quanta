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
//app includes

#include "actionconfigdialog.h"
#include "filecombo.h"
#include "useraction.h"
#include "toolbartabwidget.h"

//qt includes
#include <QButtonGroup>
#include <QCheckBox>
#include <qdom.h>
#include <QFileInfo>
#include <QLineEdit>
#include <q3listbox.h>
#include <QTextEdit>
#include <QRadioButton>
#include <QTabWidget>
#include <QRegExp>

//kde includes
#include <kapplication.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kicon.h>
#include <kicondialog.h>
#include <kiconloader.h>
#include <kkeysequencewidget.h>
#include <k3listview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kmenu.h>
#include <kpushbutton.h>
#include <krandom.h>
#include <kstandarddirs.h>
#include <kdeversion.h>
#include <kinputdialog.h>
#include <ktoolbar.h>
#include <kxmlguifactory.h>
#include <kactioncollection.h>
#include <kparts/mainwindow.h>

//kdevelop includes
#include <core.h>
#include <iplugin.h>
#include <iuicontroller.h>

ActionConfigDialog::ActionConfigDialog(const QHash<QString, ToolbarEntry*> &toolbarList, KDevelop::IPlugin* parent, bool modal, Qt::WFlags fl, const QString& defaultAction )
  :QDialog( KDevelop::Core::self()->uiController()->activeMainWindow(), fl )
{
  setModal(modal);
  setupUi(this);
  m_plugin = parent;
  m_toolbarList = toolbarList;
  buttonOk->setIcon(KIcon("dialog-ok"));
  buttonCancel->setIcon(KIcon("dialog-cancel"));
  buttonApply->setIcon(KIcon("dialog-apply"));
  //buttonHelp->setIcon(KIcon("help-contents"));

  currentAction = 0L;
  m_toolbarItem = 0L;

  connect(actionTreeView, SIGNAL(contextMenu(K3ListView *,Q3ListViewItem *,const QPoint &)),
                          SLOT(slotContextMenu(K3ListView *,Q3ListViewItem *,const QPoint &)));
  connect(actionTreeView, SIGNAL(selectionChanged(Q3ListViewItem *)),
                          SLOT(slotSelectionChanged(Q3ListViewItem *)));
  connect(shortcutKeyButton, SIGNAL(capturedShortcut(const KShortcut &)),
                             SLOT(slotShortcutCaptured(const KShortcut &)));
  connect(scriptPath, SIGNAL(activated(const QString&)),
                      SLOT(slotTextChanged(const QString&)));
  connect(this, SIGNAL(addToolbar()), m_plugin, SLOT(slotAddToolbar()));
  connect(this, SIGNAL(removeToolbar(const QString&)), m_plugin, SLOT(slotRemoveToolbar(const QString&)));
  connect(this, SIGNAL(deleteUserAction(KAction*)), m_plugin, SLOT(slotDeleteAction(KAction*)));
  connect(this, SIGNAL(configureToolbars(const QString&)), m_plugin, SLOT(slotConfigureToolbars(const QString&)));
//fill up the tree view with the toolbars and actions
  actionTreeView->setSorting(-1);
  allActionsItem = new K3ListViewItem(actionTreeView, i18n("All"));
  actionTreeView->insertItem(allActionsItem);
  Q3ListViewItem *item, *oldItem = allActionsItem;
  QAction *action;
  QString toolbarName;
  QString toolbarId;
  ToolbarTabWidget *tb = ToolbarTabWidget::ref();
  QRegExp r("\\&(?!\\&)");
  KActionCollection *ac = KDevelop::Core::self()->uiController()->activeMainWindow()->actionCollection();
  for (int i = 0; i < tb->count(); i++)
  {
    toolbarName = tb->tabText(i);
    toolbarName.replace(r, "");
    item = new K3ListViewItem(actionTreeView, oldItem, i18n(toolbarName.toUtf8()));
    actionTreeView->insertItem(item);

    toolbarId = tb->id(i);
    Q3ListViewItem *oldActionItem = 0L;
    ToolbarEntry *p_toolbar = m_toolbarList.value(toolbarId);
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
            oldActionItem = new K3ListViewItem(item, oldActionItem, action->text().replace(r,""), action->shortcut().toString(), action->objectName());
            oldActionItem->setPixmap(0, action->icon().pixmap(QSize(16,16)));
          }
        }
        node = node.nextSibling();
      }
      if (tb->tabText(tb->currentIndex()).replace(r, "") == toolbarName)
      {
         item->setOpen(true);
         if (item->firstChild())
         {
           if (defaultAction.isEmpty())
           {
             actionTreeView->setCurrentItem(item->firstChild());
           } else
           {
             Q3ListViewItem *actionItem = item->firstChild();
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
  uint acCount = ac->actions().count();
  for (uint i = 0; i < acCount; i++)
  {
    action = ac->actions().value(i);
    if (dynamic_cast<UserAction*>(action))
    {
      item = new K3ListViewItem(allActionsItem, action->text().replace(r, ""),  action->shortcut().toString(), action->objectName());
      item->setPixmap(0, action->icon().pixmap(QSize(16,16)));
    }
  }
  allActionsItem->sortChildItems(0, true);

  treeMenu = new KMenu(actionTreeView);
  treeMenu->addAction(i18n("&Add New Toolbar"), this, SLOT(slotAddToolbar()));
  treeMenu->addAction(i18n("&Remove Toolbar"), this, SLOT(slotRemoveToolbar()));
  treeMenu->addAction(i18n("&Edit Toolbar"), this, SLOT(slotEditToolbar()));

  globalShortcuts = KGlobal::config()->entryMap( "Global Shortcuts" );
}

ActionConfigDialog::~ActionConfigDialog()
{
}

void ActionConfigDialog::slotAddToolbar()
{
  emit addToolbar();
  QString toolbarName;
  Q3ListViewItem *item;
  ToolbarTabWidget *tb = ToolbarTabWidget::ref();
  for (int i = 0; i < tb->count(); i++)
  {
    toolbarName = tb->tabText(i);
    if (!actionTreeView->findItem(toolbarName, 0))
    {
      item = actionTreeView->lastItem();
      if (item->parent())
        item = item->parent();
      new K3ListViewItem(actionTreeView, item, i18n(toolbarName.toUtf8()));
      break;
    }
  }
}

void ActionConfigDialog::slotRemoveToolbar()
{
  Q3ListViewItem *item = actionTreeView->currentItem();
  QString s = item->text(0);
  if (item->parent())
  {
    item = item->parent();
    s = item->text(0);
  }
  if (s != i18n("All"))
  {
      if ( KMessageBox::warningContinueCancel(this, i18n("Do you really want to remove the \"%1\" toolbar?", s),QString::null,KStandardGuiItem::del()) == KMessageBox::Continue )
    {
      m_toolbarItem = item;
      connect(m_plugin, SIGNAL(toolbarRemoved(const QString&)), SLOT(slotToolbarRemoved(const QString&)));
      emit removeToolbar(s.toLower());
    }
  }
}

void ActionConfigDialog::slotToolbarRemoved(const QString &/*name*/)
{
  actionTreeView->setCurrentItem(allActionsItem);
  delete m_toolbarItem;
  m_toolbarItem = 0L;
  disconnect(m_plugin, SIGNAL(toolbarRemoved(const QString&)), this, SLOT(slotToolbarRemoved(const QString&)));
}

void ActionConfigDialog::slotEditToolbar()
{
  QString toolbarName;
  QString toolbarId;
  Q3ListViewItem *oldItem;
  Q3ListViewItem *item = actionTreeView->currentItem();
  if (item->parent())
     item = item->parent();
  toolbarName = item->text(0);
  if ( toolbarName != i18n("All"))
  {
    emit configureToolbars(toolbarName +" <quanta>");

    //update the tree view
    QAction *action;
    KActionCollection *ac = KDevelop::Core::self()->uiController()->activeMainWindow()->actionCollection();
    ToolbarTabWidget *tb = ToolbarTabWidget::ref();
    for (int i = 0; i < tb->count(); i++)
    {
      toolbarName = tb->tabText(i);
      toolbarId = tb->id(i);
      ToolbarEntry *p_toolbar = m_toolbarList.value(toolbarId);
      if (p_toolbar)
      {
        oldItem = actionTreeView->findItem(toolbarName, 0);
        item = new K3ListViewItem(actionTreeView, oldItem, toolbarName);
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
              oldItem = new K3ListViewItem(item, oldItem, action->text().replace(QRegExp("\\&(?!\\&)"),""), action->shortcut().toString(), action->objectName());
              oldItem->setPixmap(0,action->icon().pixmap(QSize(16,16)));
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

void ActionConfigDialog::slotContextMenu(K3ListView *,Q3ListViewItem *,const QPoint &point)
{
  treeMenu->popup(point);
}

void ActionConfigDialog::slotSelectionChanged(Q3ListViewItem *item)
{
  if (currentAction && currentAction->inherits("UserAction"))
  {
    if ( buttonApply->isEnabled() &&
         KMessageBox::questionYesNo(this, i18n("Do you want to save the changes made to this action?")) == KMessageBox::Yes )
    {
      saveCurrentAction();
    }
    buttonApply->setEnabled(false);
    currentAction = 0L;
  }
  if (item && item->depth() > 0)
  {
    UserAction *action = 0L;
    KActionCollection *ac = KDevelop::Core::self()->uiController()->activeMainWindow()->actionCollection();
    uint acCount = ac->actions().count();
//find the corresponding action
    for (uint i = 0; i < acCount; i++)
    {
      QAction *a = ac->actions().value(i);
      QString actionName = a->objectName();
      if (actionName == item->text(2) && a->inherits("UserAction"))
      {
        action = static_cast<UserAction*>(a);
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
        shortcutKeyButton->clearKeySequence();
      } else
      {
        customShortcut->setChecked(true);
        shortcutKeyButton->setKeySequence(shortcutText);
      }

//find the container toolbars of this action and add them to the container listbox
      toolbarListBox->clear();
      int current = 0;
      int count = 0;
      ToolbarTabWidget *tb = ToolbarTabWidget::ref();
      for (int i = 0; i < tb->count(); i++)
      {
        QString toolbarName = tb->tabText(i);
        QString toolbarId = tb->id(i);
        ToolbarEntry *p_toolbar = m_toolbarList.value(toolbarId);
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
      inputBox->setCurrentIndex(0);
      outputBox->setCurrentIndex(5);
      errorBox->setCurrentIndex(5);
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
        typeCombo->setCurrentIndex(0);
      }
      else if (type == "script")
      {
        QDomElement elScript = el.namedItem("script").toElement();
        scriptPath->setText( elScript.text() );

        QString s;
        s = elScript.attribute("input","none");
        if (s == "current")
        {
          inputBox->setCurrentIndex(1);
        } else
        if (s == "selected")
        {
          inputBox->setCurrentIndex(2);
        } else
        {
          inputBox->setCurrentIndex(0);
        }

        s = elScript.attribute("output","none");
        if (s == "cursor")
        {
          outputBox->setCurrentIndex(1);
        } else
        if (s == "selection")
        {
          outputBox->setCurrentIndex(2);
        } else
        if (s == "replace")
        {
          outputBox->setCurrentIndex(3);
        } else
        if (s == "new")
        {
          outputBox->setCurrentIndex(4);
        } else
        if (s == "message")
        {
          outputBox->setCurrentIndex(5);
        } else
        {
          outputBox->setCurrentIndex(0);
        }

        s = elScript.attribute("error","none");
        if (s == "cursor")
        {
          errorBox->setCurrentIndex(1);
        } else
        if (s == "selection")
        {
          errorBox->setCurrentIndex(2);
        } else
        if (s == "replace")
        {
          errorBox->setCurrentIndex(3);
        } else
        if (s == "new")
        {
          errorBox->setCurrentIndex(4);
        } else
        if (s == "message")
        {
          errorBox->setCurrentIndex(5);
        } else
        {
          errorBox->setCurrentIndex(0);
        }
        typeCombo->setCurrentIndex(1);
      }
      else if (type == "text")
      {
        textEdit->setText( el.namedItem("text").toElement().text() );
        typeCombo->setCurrentIndex(2);
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
  static_cast<UserAction *>(currentAction)->setModified(true);
  QString s;
  QDomElement el = static_cast<UserAction *>(currentAction)->data();
  s = actionIcon->icon();
  el.setAttribute("icon", s);
  currentAction->setIcon(KIcon(s));
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
  }
  el.setAttribute("shortcut", s);
  currentAction->setShortcut(selectedShortcut);

//update the tree view
  Q3ListViewItem *listItem;
  Q3ListViewItemIterator it(actionTreeView);
  while (it.current())
  {
    listItem = it.current();
    if (listItem->depth() > 0 && listItem->text(2) == currentAction->objectName())
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
  int type = typeCombo->currentIndex();
  switch (type)
  {
    case 1:{
        el.setAttribute("type","script");
        item = document.createElement("script");
        switch (inputBox->currentIndex())
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
        switch (outputBox->currentIndex())
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
        switch (errorBox->currentIndex())
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
    QString toolbarName = tb->tabText(i);
    QString toolbarId = tb->id(i);
    ToolbarEntry *p_toolbar = m_toolbarList.value(toolbarId);
    bool isOnToolbar = false;
    if (p_toolbar)
    {
      QDomNode node = p_toolbar->guiClient->domDocument().firstChild().firstChild().firstChild();
      bool placeOnToolbar = toolbarListBox->findItem(toolbarName, Q3ListBox::ExactMatch);
      while (!node.isNull())
      {
        if (node.nodeName() == "Action" &&
            node.toElement().attribute("name") == el.attribute("name"))
        {
          //if it's present in the toolbar, but not in the container list,
          //remove it also from the toolbar
          if (!placeOnToolbar)
          {
            tb->page(i)->removeAction(currentAction);
            p_toolbar->menu->removeAction(currentAction);
            node.parentNode().removeChild(node);
            Q3ListViewItemIterator iter(actionTreeView);
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
        tb->page(i)->addAction(currentAction);
        p_toolbar->menu->addAction(currentAction);
        item = p_toolbar->guiClient->domDocument().createElement("Action");
        item.setAttribute("name",el.attribute("name"));
        p_toolbar->guiClient->domDocument().firstChild().firstChild().appendChild(item);
      //put it also in the treeview
        listItem = actionTreeView->findItem(toolbarName, 0);
        if (listItem)
        {
          Q3ListViewItem *after = listItem->firstChild();
          while ( after && after->nextSibling() && after->nextSibling()->depth()!=0 )
          {
            if (after->text(2) == currentAction->objectName())
            {
                placeOnToolbar = false;
                break;
            }
            after = after->nextSibling();
          }
          if (placeOnToolbar)
          {
              listItem = new K3ListViewItem(listItem, after, lineText->text(), currentAction->shortcut().toString(), currentAction->objectName());
              listItem->setPixmap(0, SmallIcon(actionIcon->icon()));
          }
        }
      }
      KXMLGUIFactory::saveConfigFile(p_toolbar->guiClient->domDocument(),
        p_toolbar->guiClient->xmlFile(), p_toolbar->guiClient->componentData());
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
  int pos = shortcutText.indexOf(';');
  if (pos != -1)
  {
    shortcutText2 = shortcutText.mid(pos + 1);
    shortcutText = shortcutText.left(pos);
  }
  QString global;
//check for conflicting global shortcuts
  QMap<QString, QString>::Iterator it;
  for ( it = globalShortcuts.begin(); it != globalShortcuts.end(); ++it )
  {
    if (it.value().contains(shortcutText))
    {
      global = it.key();
      break;
    }
    if (!shortcutText2.isEmpty() && it.value().contains(shortcutText2))
    {
      shortcutText = shortcutText2;
      global = it.key();
      break;
    }
  }

  if (global.isEmpty())
  {
    QList<KXMLGUIClient*> clients = KDevelop::Core::self()->uiController()->activeMainWindow()->guiFactory()->clients();
    KXMLGUIClient *current = 0L;
    QListIterator<KXMLGUIClient*> it(clients);
    while (it.hasNext())
    {
        current = it.next();
        KActionCollection *ac = current->actionCollection();
        for (int i = 0; i < ac->actions().count(); i++)
        {
          QAction *action = ac->actions().value(i);
          if (action != currentAction && action->shortcut().toString().contains(shortcutText))
          {
            global = action->text();
            break;
          }
          if (!shortcutText2.isEmpty() && action != currentAction &&  action->shortcut().toString().contains(shortcutText))
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
    shortcutKeyButton->setKeySequence(shortcutText);
    buttonApply->setEnabled(true);
    selectedShortcut = shortcut;
  } else
  {
    global.replace('&',"");
    QString s =  i18n("The '%1' key combination has already been allocated "
                "to the \"%2\" action.\n"
                "Please choose a unique key combination.",
                shortcutText, global);
    KMessageBox::sorry( this, s, i18n("Conflicting Shortcuts"));
  }
}

void ActionConfigDialog::accept()
{
  if (buttonApply->isEnabled())
      saveCurrentAction();
  QDialog::accept();
}

void ActionConfigDialog::reject()
{
  if (buttonApply->isEnabled() && KMessageBox::questionYesNo(this, i18n("Do you want to save the changes made to this action?")) == KMessageBox::Yes  )
      saveCurrentAction();
  QDialog::reject();
}

void ActionConfigDialog::slotNewAction()
{
  QDomDocument doc;
  QDomElement el = doc.createElement("action");
  el.setAttribute( "name", "user_" + KRandom::randomString(10) );
  el.setAttribute( "icon", "ball" );

  currentAction = new UserAction(&el, m_plugin);
  //add the actions to every toolbar xmlguiclient
  QHashIterator<QString, ToolbarEntry*> it(m_toolbarList);
  int i=0;
  while (it.hasNext())
  {
    it.next();
    it.value()->guiClient->actionCollection()->addAction(QString("name_action%1").arg(i),currentAction);
    i++;
  }

  selectedShortcut = KShortcut();
  static_cast<UserAction*>(currentAction)->setModified(true);
  Q3ListViewItem *currentItem = actionTreeView->currentItem();
  Q3ListViewItem *item = new K3ListViewItem(allActionsItem);
  QString actionText = QString("Action_%1").arg(KDevelop::Core::self()->uiController()->activeMainWindow()->actionCollection()->actions().count());
  currentAction->setText(actionText);
  item->setText(2, currentAction->objectName());
  item->setText(0, actionText);
  item->setPixmap(0, SmallIcon("ball"));
  allActionsItem->sortChildItems(0, true);
  if (currentItem->parent() && currentItem->parent() == allActionsItem)
  {
     actionTreeView->setCurrentItem(item);
  } else
  {
    Q3ListViewItem *parentItem = currentItem->parent();
    if (!parentItem)
      parentItem = currentItem;

    item = new K3ListViewItem(parentItem, currentItem);
    item->setText(0, actionText);
    item->setText(2, currentAction->objectName());
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
    if ( KMessageBox::warningContinueCancel(this, i18n("<qt>Removing the action removes all the references to it.\nAre you sure you want to remove the <b>%1</b> action?</qt>", currentAction->text()),QString::null,KStandardGuiItem::del()) == KMessageBox::Continue )
  {
    QString actionName = currentAction->objectName();
    emit deleteUserAction(currentAction);
    currentAction = 0L;
    //update the tree view
    Q3ListViewItem *listItem;
    Q3ListViewItemIterator it(actionTreeView);
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
    if (!toolbarListBox->findItem(tb->tabText(i), Q3ListBox::ExactMatch))
      lst << tb->tabText(i);
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

void ActionConfigDialog::createScriptAction(const QString& name, const QString& script)
{
  actionTreeView->setCurrentItem(allActionsItem);
  slotNewAction();
  typeCombo->setCurrentIndex(1);
  scriptPath->setText(script);
  lineText->setText(name);
}

#include "actionconfigdialog.moc"
