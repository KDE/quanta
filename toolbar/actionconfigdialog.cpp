//
//
// C++ Implementation: cpp
//
// Description:
//
//
// Author: Andras Mantia <amantia@freemail.hu>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//

//qt includes
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qdom.h>
#include <qfileinfo.h>
#include <qinputdialog.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qmultilineedit.h>
#include <qradiobutton.h>
#include <qtabwidget.h>

//kde includes
#include <kicondialog.h>
#include <kiconloader.h>
#include <kkeybutton.h>
#include <klistview.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kpushbutton.h>

//app includes
#include "../resource.h"
#include "../quanta.h"
#include "../quantaview.h"
#include "../tagdialogs/filecombo.h"

#include "actionconfigdialog.h"
#include "tagaction.h"

ActionConfigDialog::ActionConfigDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    :ActionConfigDialogS( parent, name, modal, fl )
{
  currentAction = 0L;

  connect(actionTreeView, SIGNAL(contextMenu(KListView *,QListViewItem *,const QPoint &)),
                          SLOT(slotContextMenu(KListView *,QListViewItem *,const QPoint &)));
  connect(actionTreeView, SIGNAL(selectionChanged(QListViewItem *)),
                          SLOT(slotSelectionChanged(QListViewItem *)));
  connect(shortcutKeyButton, SIGNAL(capturedShortcut(const KShortcut &)),
                             SLOT(slotShortcutCaptured(const KShortcut &)));
  connect(scriptPath, SIGNAL(activated(const QString&)), SLOT(slotTextChanged(const QString&)));
//fill up the tree view with the toolbars and actions
  actionTreeView->setSorting(-1);
  allActionsItem = new KListViewItem(actionTreeView, i18n("All"));
  actionTreeView->insertItem(allActionsItem);
  QListViewItem *item, *oldItem = 0L;
  KAction *action;
  QString toolbarName;
  QTabWidget *tb = quantaApp->getView()->toolbarTab;
  for (int i = 0; i < tb->count(); i++)
  {
    toolbarName = tb->label(i);
    item = new KListViewItem(actionTreeView, oldItem, i18n(toolbarName));
    actionTreeView->insertItem(item);

    QListViewItem *oldActionItem = 0L;
    ToolbarEntry *p_toolbar = quantaApp->toolbarList[toolbarName.lower()];
    if (p_toolbar)
    {
      QDomNode node = p_toolbar->guiClient->domDocument().firstChild().firstChild().firstChild();
      while (!node.isNull())
      {
        if (node.nodeName() == "Action")
        {
          action = quantaApp->actionCollection()->action(node.toElement().attribute("name"));
          if (action)
          {
            oldActionItem = new KListViewItem(item, oldActionItem, action->text(), action->shortcut().toString(), "Amdrs");
            oldActionItem->setPixmap(0, BarIcon(action->icon()) );
          }
        }
        node = node.nextSibling();
      }
      if (tb->label(tb->currentPageIndex()) == toolbarName)
      {
         item->setOpen(true);
         if (item->firstChild())
         {
           actionTreeView->setCurrentItem(item->firstChild());
         }
      }
    }
    oldItem = item;
  }
  for (uint i = 0; i < quantaApp->actionCollection()->count(); i++)
  {
    action = quantaApp->actionCollection()->action(i);
    item = new KListViewItem(allActionsItem, action->text().replace('&',""), action->shortcut().toString());
    item->setPixmap(0, BarIcon(action->icon()) );
  }
  allActionsItem->sortChildItems(0, true);

  treeMenu = new KPopupMenu(actionTreeView);
  treeMenu->insertItem(i18n("&Add new toolbar"), this, SLOT(slotAddToolbar()));
  treeMenu->insertItem(i18n("&Remove toolbar"), this, SLOT(slotRemoveToolbar()));
  treeMenu->insertItem(i18n("&Edit toolbar"), this, SLOT(slotEditToolbar()));
}

ActionConfigDialog::~ActionConfigDialog()
{
}

void ActionConfigDialog::slotAddToolbar()
{
  quantaApp->slotAddToolbar();
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
    if ( KMessageBox::questionYesNo(this, i18n("Do you really want to remove the \"%1\" toolbar?").arg(s)) == KMessageBox::Yes )
    {
      quantaApp->removeToolbar(s.lower());
      actionTreeView->setCurrentItem(allActionsItem);
      delete item;
    }
  }
}

void ActionConfigDialog::slotEditToolbar()
{
  quantaApp->slotOptionsConfigureToolbars();
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
         KMessageBox::questionYesNo(this, i18n("Do you want to save the changes made for this action?")) == KMessageBox::Yes )
    {
      saveCurrentAction();
    }
    buttonApply->setEnabled(false);
    currentAction = 0L;
  }
  if (item && item->depth() > 0)
  {
    TagAction *action = 0L;
//find the corresponding action
    for (uint i = 0; i < quantaApp->actionCollection()->count(); i++)
    {
      KAction *a = quantaApp->actionCollection()->action(i);
      if (a && a->text() == item->text(0) && a->inherits("TagAction"))
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
          actionIcon->setIcon( el.attribute("icon") );

      lineText->setText( el.attribute("text") );
      lineToolTip->setText( el.attribute("tooltip") );
      QString shortcutText = action->shortcut().toString();
      if (shortcutText.isEmpty())
      {
        noShortcut->setChecked(true);
        shortcutKeyButton->setText(i18n("None"));
      } else
      {
        customShortcut->setChecked(true);
        shortcutKeyButton->setShortcut(KShortcut(shortcutText));
        shortcutKeyButton->setText(shortcutText);
      }

//find the container toolbars of this action and add them to the container listbox
      toolbarListBox->clear();
      int current = 0;
      int count = 0;
      QTabWidget *tb = quantaApp->getView()->toolbarTab;
      for (int i = 0; i < tb->count(); i++)
      {
        QString toolbarName = tb->label(i);
        ToolbarEntry *p_toolbar = quantaApp->toolbarList[toolbarName.lower()];
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
  QString s;
  QDomElement el = static_cast<TagAction *>(currentAction)->data();
  s = actionIcon->icon();
  el.setAttribute("icon", QFileInfo(s).fileName());
  currentAction->setIcon(s);
  QString oldText = el.attribute("text");
  s = lineText->text();
  el.setAttribute("text", s);
  currentAction->setText(s);
  s = lineToolTip->text();
  el.setAttribute("tooltip", s);
  currentAction->setToolTip(s);
  s = "";
  if (customShortcut->isChecked())
  {
    s = shortcutKeyButton->text();
  }
  el.setAttribute("shortcut", s);
  currentAction->setShortcut(KShortcut(s));

//update the tree view
  QListViewItem *listItem;
  QListViewItemIterator it(actionTreeView);
  while (it.current())
  {
    listItem = it.current();
    if (listItem->depth() > 0 && listItem->text(0) == oldText)
    {
      listItem->setPixmap(0, BarIcon(actionIcon->icon()));
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
  QDomDocument document = el.ownerDocument();
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
}

void ActionConfigDialog::slotShortcutCaptured(const KShortcut &shortcut)
{
  shortcutKeyButton->setText(shortcut.toString());
}

void ActionConfigDialog::accept()
{
  saveCurrentAction();
  ActionConfigDialogS::accept();
}

void ActionConfigDialog::slotNewAction()
{
  QDomElement el = quantaApp->actions()->createElement("action");
  el.setAttribute( "name", "user_"+KApplication::randomString(10) );
  el.setAttribute( "icon", "ball" );
  quantaApp->actions()->firstChild().appendChild(el);

  new TagAction(&el, quantaApp->actionCollection());
  QListViewItem *currentItem = actionTreeView->currentItem();
  QListViewItem *item = new KListViewItem(allActionsItem);
  item->setPixmap(0, BarIcon("ball"));
  allActionsItem->sortChildItems(0, true);
  if (!currentItem->parent() || currentItem->parent() == allActionsItem)
  {
     actionTreeView->setCurrentItem(item);
  } else
  {
    item = new KListViewItem(currentItem->parent(), currentItem);
    item->setPixmap(0, BarIcon("ball"));
    actionTreeView->setCurrentItem(item);
    toolbarListBox->insertItem(currentItem->parent()->text(0));
    toolbarListBox->setCurrentItem(0);
    toolbarListBox->setSelected(0, true);
  }
  buttonApply->setEnabled(true);
}

void ActionConfigDialog::slotDeleteAction()
{
  if ( KMessageBox::questionYesNo(this, i18n("Removing the action removes all the references to it.\nAre you sure you want to remove the current action ?")) == KMessageBox::Yes )
  {
//remove all references to this action
    QDomElement el = static_cast<TagAction*>(currentAction)->data();
    QString text = el.attribute("text");
    quantaApp->actions()->firstChild().removeChild( el );
    QString actionName = currentAction->name();

    QPtrList<KXMLGUIClient> guiClients = quantaApp->guiFactory()->clients();
    KXMLGUIClient *guiClient = 0;
    QDomNodeList nodeList;
    for (uint i = 1; i < guiClients.count(); i++)
    {
      guiClient = guiClients.at(i);
      quantaApp->guiFactory()->removeClient(guiClient);
      nodeList = guiClient->domDocument().elementsByTagName("Action");
      for (uint j = 0; j < nodeList.count(); j++)
      {
        //we found a toolbar that contains the action
        if (nodeList.item(j).toElement().attribute("name") == actionName)
        {
          nodeList.item(j).parentNode().removeChild(nodeList.item(j));
          KXMLGUIFactory::saveConfigFile(guiClient->domDocument(), guiClient->localXMLFile());
        }
      }
        guiClient ->setXMLGUIBuildDocument( QDomDocument() );
        guiClient->reloadXML();
        quantaApp->guiFactory()->addClient(guiClient);
    }
      delete currentAction;
      currentAction = 0L;
    //update the tree view
      QListViewItem *listItem;
      QListViewItemIterator it(actionTreeView);
      while (it.current())
      {
        listItem = it.current();
        if (listItem->depth() > 0 && listItem->text(0) == text)
        {
          ++it;
          delete listItem;
        } else
        {
          ++it;
        }
      }
  }
}

void ActionConfigDialog::slotApply()
{
  saveCurrentAction();
  buttonApply->setEnabled(false);
}

void ActionConfigDialog::slotAddContainerToolbar()
{
  QTabWidget *tb = quantaApp->getView()->toolbarTab;
  int i;

  QStringList lst;
  for (i = 0; i < tb->count(); i++)
  {
    if (!toolbarListBox->findItem(tb->label(i), Qt::ExactMatch))
        lst << tb->label(i);
  }

  if (lst.count() > 0)
  {
    bool ok = FALSE;
    QString res = QInputDialog::getItem(
                    i18n( "Add Action To Toolbar" ),
                    i18n( "Please select a toolbar:" ), lst, 0, FALSE, &ok, this );

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

#include "actionconfigdialog.moc"

