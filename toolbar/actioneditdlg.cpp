/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

 /***************************************************************************
    copyright            : (C) 2001 by Dmitry Poplavsky
                           (C) 2002 by Andras Mantia <amantia@freemail.hu>
    email                : dima@kde.org
 ***************************************************************************/


#include <qlistbox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qmultilineedit.h>
#include <qtabwidget.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qptrlist.h>
#include <qpopupmenu.h>

#include <klocale.h>
#include <kmessagebox.h>
#include <kaction.h>
#include <kiconloader.h>
#include <kicondialog.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kxmlguiclient.h>
#include <kmenubar.h>

#include "actioneditdlg.h"
#include "tagaction.h"
#include "../tagdialogs/filecombo.h"
#include "../quanta.h"
#include "../quantadoc.h"
#include "../quantaview.h"

class ActionListItem : public QListBoxPixmap {
private:
  KAction *m_action;
public:
  ActionListItem( KAction *a )
    :QListBoxPixmap( UserIcon( a->icon()), a->plainText() ),
     m_action(a) {;}
  ~ActionListItem() {}

  KAction *action() { return m_action; }

};

void replaceDomItem( QDomElement &el, QString name, QString value )
  {
    QDomDocument document = el.ownerDocument();
    QDomElement item = el.namedItem(name).toElement();
    if ( !item.isNull() )
      el.removeChild(item);
    item = document.createElement(name);
    el.appendChild(item);
    item.appendChild( document.createTextNode( value ) );
  }


ActionEditDlg::ActionEditDlg( QuantaApp *a, QWidget* parent, const char* name, bool modal, WFlags fl )
    :ActionEditDlgS( parent, name, modal, fl ),
     action(0),
     app(a)
{
   scriptPath->setBasePath(app->getDoc()->basePath());
   scriptPath->setReturnAbsolutePath(true);
   actionIcon->setIconType(KIcon::User,KIcon::Any,true);

   QValueList<KAction*> actions = app->actionCollection()->actions();

   QValueList<KAction*>::Iterator it;
   for( it = actions.begin(); it != actions.end(); ++it ) {
      KAction *action = *it;
      if ( action->inherits("TagAction"))
          actionsList->insertItem( new ActionListItem(action) );
   }
   actionsList->sort();

  QTabWidget *tb = a->getView()->getToolbarTab();
  for (int i = 0; i < tb->count(); i++)
  {
    toolbarCombo->insertItem(tb->label(i));
  }
}

ActionEditDlg::~ActionEditDlg()
{
}



void ActionEditDlg::deleteAction()
{
   if ( !action ) return;
   if ( KMessageBox::questionYesNo(this, i18n("Removing the action removes all the references to it.\nAre you sure you want to remove the current action ?")) == KMessageBox::Yes )
   {
//remove all references to this action
     app->actions()->firstChild().removeChild( action->data() );
     QString actionText = action->name();

     QPtrList<KXMLGUIClient> guiClients = app->factory()->clients();
     KXMLGUIClient *guiClient = 0;
     QDomNodeList nodeList;
     for (uint i = 0; i < guiClients.count(); i++)
     {
      guiClient = guiClients.at(i);
      app->factory()->removeClient(guiClient);
      nodeList = guiClient->domDocument().elementsByTagName("Action");
      for (uint j = 0; j < nodeList.count(); j++)
      {
        //we found a toolbar that contains the action
        if (nodeList.item(j).toElement().attribute("name") == actionText)
        {
          nodeList.item(j).parentNode().removeChild(nodeList.item(j));
          guiClient->actionCollection()->remove(action);
          KXMLGUIFactory::saveConfigFile(guiClient->domDocument(), guiClient->localXMLFile());
        }
      }
        guiClient ->setXMLGUIBuildDocument( QDomDocument() );
        guiClient->reloadXML();
        app->guiFactory()->addClient(guiClient);
     }
//       app->actionCollection()->remove(action);
       action = 0;
       actionsList->removeItem( actionsList->currentItem() );
   }

}

void ActionEditDlg::newAction()
{
   qDebug("new action");
   QDomElement el = app->actions()->createElement("action");
   el.setAttribute( "name", "user_"+KApplication::randomString(10) );
   el.setAttribute( "icon", "ball" );
   app->actions()->firstChild().appendChild(el);

   TagAction *a = new TagAction(&el, app->view, app->actionCollection() );
   ActionListItem *it = new ActionListItem(a);

   actionsList->insertItem( it );
   actionsList->setSelected( it, true );
   actionsList->setCurrentItem( it );
   actionsList->centerCurrentItem( );

   app->actionCollection()->insert( a );

}


TagAction * ActionEditDlg::selectedAction()
{
    ActionListItem *item = dynamic_cast<ActionListItem*>( actionsList->item(actionsList->currentItem()) );

    if ( !item )
        return 0;

    return dynamic_cast<TagAction*>( item->action() );
}

void ActionEditDlg::actionSelected()
{
    if ( action )  {
        saveAction(action);
    }

    action = selectedAction();

    if ( action ) {
        loadAction(action);
    }

}


void ActionEditDlg::loadAction( TagAction *a )
{
    if (!a)
        return;
    QDomElement el = a->data();

    if ( el.hasAttribute("icon") )
        actionIcon->setIcon( el.attribute("icon") );

    lineText->setText( el.attribute("text") );
    lineToolTip->setText( el.attribute("tooltip") );
    lineStatusText->setText( el.attribute("statustext") );

    QDomElement elxtag = el.namedItem("xtag").toElement();
    QDomElement eltag = el.namedItem("tag").toElement();

    tagClose->setText( elxtag.text() );
    tag->setText( eltag.text() );

    useTagClose->setChecked( elxtag.attribute("use","false") == "true" );
//FIXME/TODO add insert in 1 line option
//        insertInLine->setChecked( elxtag.attribute("inLine","true") == "true" );
    useActionDialog->setChecked( eltag.attribute("useDialog","false") == "true" );

    QDomElement elScript = el.namedItem("script").toElement();
    scriptPath->setText( elScript.text() );

    QString sInput  = elScript.attribute("input","none");
    QString sOutput = elScript.attribute("output","none");
    QString sError  = elScript.attribute("error","none");

    inputCurrent->setChecked( sInput == "current" );
    inputSelected->setChecked( sInput == "selected" );

    outputCursor_2->setChecked( sOutput == "cursor" );
    outputMessage->setChecked( sOutput == "message" );
    outputNew    ->setChecked( sOutput == "new" );
    outputReplace->setChecked( sOutput == "replace" );

    errorCursor ->setChecked( sError == "cursor" );
    errorMessage->setChecked( sError == "message" );
    errorNew    ->setChecked( sError == "new" );
    errorReplace->setChecked( sError == "replace" );

    text->setText( el.namedItem("text").toElement().text() );

    QString type = el.attribute("type","tag");

    if ( type == "tag" )
      actionTab->setCurrentPage(0);
    if ( type == "script" )
      actionTab->setCurrentPage(1);
    if ( type == "text" )
      actionTab->setCurrentPage(2);

    placeOnToolbar->setChecked(false);

}


void ActionEditDlg::saveAction( TagAction *a )
{
    if (!a)
        return;
    QDomElement el = a->data();
    el.setAttribute("icon", actionIcon->icon() );
    el.setAttribute("text", lineText->text() );
    el.setAttribute("tooltip", lineToolTip->text() );
    el.setAttribute("statustext", lineStatusText->text() );
    QString stab = actionTab->tabLabel( actionTab->currentPage() );

    a->setText( lineText->text() );
    a->setIcon( actionIcon->icon() );

    QDomDocument document = el.ownerDocument();


    if ( stab == i18n("Tag") )
    {
      el.setAttribute("type","tag");
    }
    replaceDomItem(el,"tag",tag->text() );
    replaceDomItem(el,"xtag",tagClose->text() );
    el.namedItem("xtag").toElement().setAttribute("use", useTagClose->isChecked() ? "true" : "false" );
    //el.namedItem("xtag").toElement().setAttribute("inLine", insertInLine->isChecked() ? "true" : "false" );
    el.namedItem("tag").toElement().setAttribute("useDialog", useActionDialog->isChecked() ? "true" : "false" );

    if ( stab == i18n("Text") )
    {
      el.setAttribute("type","text");
    }
    replaceDomItem(el,"text",text->text() );

    if ( stab == i18n("Script") )
    {
      el.setAttribute("type","script");
    }
    replaceDomItem(el,"script", scriptPath->text() );
    QDomElement script = el.namedItem("script").toElement();

    if ( inputNone       ->isChecked() ) script.setAttribute("input","none");
    if ( inputCurrent    ->isChecked() ) script.setAttribute("input","current");
    if ( inputSelected   ->isChecked() ) script.setAttribute("input","selected");

    if ( outputNone      ->isChecked() ) script.setAttribute("output","none");
    if ( outputCursor_2  ->isChecked() ) script.setAttribute("output","cursor");
    if ( outputMessage   ->isChecked() ) script.setAttribute("output","message");
    if ( outputNew       ->isChecked() ) script.setAttribute("output","new");
    if ( outputReplace   ->isChecked() ) script.setAttribute("output","replace");

    if ( errorNone      ->isChecked() ) script.setAttribute("error","none");
    if ( errorCursor    ->isChecked() ) script.setAttribute("error","cursor");
    if ( errorMessage   ->isChecked() ) script.setAttribute("error","message");
    if ( errorNew       ->isChecked() ) script.setAttribute("error","new");
    if ( errorReplace   ->isChecked() ) script.setAttribute("error","replace");

  if (placeOnToolbar->isChecked())
  {
    QPtrList<KXMLGUIClient> guiClients = app->factory()->clients();
    QDomNodeList nodeList;
    QDomNode foundNode;
    QString tabName = toolbarCombo->currentText().lower();
    QPopupMenu *menu = app->toolbarMenu(tabName);
    a->plug(menu);

    KXMLGUIClient *guiClient = 0;
    uint i =0;
    do {
      nodeList = guiClients.at(i)->domDocument().elementsByTagName("ToolBar");
      for (uint j = 0; j < nodeList.count(); j++)
      {
        //we found the right toolbar
        if (nodeList.item(j).toElement().attribute("name") == tabName)
        {
          guiClient = guiClients.at(i);
          foundNode = nodeList.item(j);
        }
      }
     i++;
    } while ( (!guiClient) && (i < guiClients.count()) );

    // modify the client's XML
    if (guiClient)
    {
      QDomElement e = guiClient->domDocument().createElement("Action");
      e.setAttribute("name",el.attribute("name"));
      foundNode.appendChild(e);
      kdDebug() << guiClient->domDocument().toString() << endl;
      //if it is a user toolbar, reload them
      guiClient->actionCollection()->insert(a);
      KXMLGUIFactory::saveConfigFile(guiClient->domDocument(), guiClient->localXMLFile());
      //reload all the clients
      for (i = 0; i < guiClients.count(); i++)
      {
        guiClient = guiClients.at(i);
        app->guiFactory()->removeClient(guiClient);
        guiClient ->setXMLGUIBuildDocument( QDomDocument() );
        guiClient->reloadXML();
        app->guiFactory()->addClient(guiClient);
      }
    }

    app->menuBar()->removeItem(app->tagsMenuId());
    int id = app->menuBar()->insertItem(i18n("&Tags"),app->tagsMenu(),-1,5);
    app->setTagsMenuId(id);
  }

}

#include "actioneditdlg.moc"
