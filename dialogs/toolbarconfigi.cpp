
#include "toolbarconfigi.h"
#include "toolbars.h"
#include "actioneditdialog.h"

#include <qlistview.h>
#include <qheader.h>
#include <qpushbutton.h>
#include <qdom.h>

#include <kmessagebox.h>
#include <klocale.h>
#include <klineeditdlg.h>
#include <kiconloader.h>

ToolBarConfigI::ToolBarConfigI( ToolBars *t, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : ToolBarConfig( parent, name, modal, fl )
{
  this->t = t;
  listAction ->header()->hide();
  listToolbar->header()->hide();
  listAction ->setSorting(-1);
  listToolbar->setSorting(-1);

  currentAction = currentToolbar = 0;


  updateToolbarsList();
}


ToolBarConfigI::~ToolBarConfigI()
{
}

void ToolBarConfigI::actionDown()
{
  QDomNode curNode =  t->toolbar( currentToolbar ).childNodes().item( currentAction );
  t->toolbar( currentToolbar ).insertAfter( curNode, curNode.nextSibling() );
  currentAction++;
  updateActionsList();
  actionSelected();
}

void ToolBarConfigI::actionUp()
{
  QDomNode curNode =  t->toolbar( currentToolbar ).childNodes().item( currentAction );
  t->toolbar( currentToolbar ).insertBefore( curNode, curNode.previousSibling() );
  currentAction--;
  updateActionsList();
  actionSelected();
}

void ToolBarConfigI::actionEdit()
{
  QDomElement el = t->toolbar( currentToolbar ).childNodes().item( currentAction ).toElement();
  ActionEditDialog * dlg = new ActionEditDialog(0, "Edit item", true);
  //update dlg from dom structure
  *dlg << el;

  if ( dlg->exec() ) {
    // update dom structure from dlg
    *dlg >> el;
    updateActionsList();
  }

  delete dlg;
}

void ToolBarConfigI::actionNew()
{
  QDomElement el = t->d.createElement("action");

  ActionEditDialog * dlg = new ActionEditDialog(0, "Edit item", true);

  if ( dlg->exec() ) {
    // update dom structure from dlg
    *dlg >> el;
    // find free id :
    int id = ( currentToolbar + 1 ) * 1000;
    while ( ! t->action(id).isNull() ) id++;
    el.setAttribute( "id", id );

    t->toolbar( currentToolbar ).appendChild(el);
    currentAction = t->toolbar( currentToolbar ).childNodes().count()-1;
    updateActionsList();
  }

  delete dlg;
}

void ToolBarConfigI::actionRemove()
{
  QDomElement curNode =  t->toolbar( currentToolbar ).childNodes().item( currentAction ).toElement();

  QString text = i18n("Are you sure, you want to remove\nitem ")+curNode.attribute("name","")+" ?";

  if ( KMessageBox::questionYesNo(this,text) == KMessageBox::Yes )
  {
      curNode.parentNode().removeChild(curNode);
      currentAction = 0;
      updateActionsList();
      actionSelected();
  }

}

void ToolBarConfigI::updateToolbarsList()
{
    listToolbar->clear();
    QListViewItem *item, *currentItem = 0;

    for ( int i = t->count()-1; i>=0; i-- ) {
      item = new QListViewItem( listToolbar, t->toolbarName(i));
      if ( i == currentToolbar )
        currentItem = item;
    }
    if ( currentItem )
      listToolbar->setCurrentItem( currentItem );

    updateActionsList();
}

void ToolBarConfigI::toolbarUp()
{
  QDomNode curNode =  t->toolbar( currentToolbar );
  t->toolbar( currentToolbar ).parentNode().insertBefore( curNode, curNode.previousSibling() );
  currentToolbar--;
  updateToolbarsList();
  toolbarSelected();
}


void ToolBarConfigI::toolbarDown()
{
  QDomNode curNode =  t->toolbar( currentToolbar );
  t->toolbar( currentToolbar ).parentNode().insertAfter( curNode, curNode.nextSibling() );
  currentToolbar++;
  updateToolbarsList();
  toolbarSelected();
}

void ToolBarConfigI::toolbarEdit()
{
  bool ok = false;
  QDomElement curNode =  t->toolbar( currentToolbar );

  QString res = KLineEditDlg::getText("\nChange toolbar caption :\n", curNode.attribute("name",""), &ok, this);

  if ( ok ) {
    curNode.setAttribute("name",res);
    updateToolbarsList();
  }

}

void ToolBarConfigI::toolbarNew()
{
  bool ok = false;

  QString res = KLineEditDlg::getText("\nToolbar caption :\n", "new toolbar", &ok, this);

  if ( ok ) {
    QDomElement curNode = t->d.createElement("tabbar");
    curNode.setAttribute("name",res);

    t->toolbar( currentToolbar ).parentNode().appendChild( curNode );

    updateToolbarsList();
  }
}

void ToolBarConfigI::toolbarRemove()
{
  QDomElement curNode =  t->toolbar( currentToolbar );

  QString text = i18n("Are you sure, you want to remove\ntoolbar ")+curNode.attribute("name","")+" ?";

  if ( KMessageBox::questionYesNo(this,text) == KMessageBox::Yes )
  {
      curNode.parentNode().removeChild(curNode);
      currentToolbar = 0;
      updateToolbarsList();
      actionSelected();
      toolbarSelected();
  }

}


void ToolBarConfigI::updateActionsList()
{
    listAction->clear();
    QListViewItem *item, *currentItem = 0;

    QDomNodeList list = t->toolbar( currentToolbar ).childNodes();

    for ( int i=list.count()-1; i>=0; i-- ) {

      QDomElement action = list.item(i).toElement();
      int id = action.attribute("id","-1").toInt();

      item = new QListViewItem( listAction, action.attribute("name",""));
      if ( action.attribute("type","separator") == "separator" )
        item->setPixmap(0, UserIcon("tag_hr") );
      else
        item->setPixmap(0,t->actionIcon(id));
      if ( i == currentAction )
        currentItem = item;
    }
    if ( currentItem )
      listAction->setCurrentItem( currentItem );
}

void ToolBarConfigI::actionSelected()
{
  currentAction = currentActionPos();

  buttonActionUp->setEnabled( currentAction );

  QListViewItem *cur = listAction->currentItem();
  if ( cur )
    buttonActionDown->setEnabled( cur->nextSibling() );
  else
    buttonActionDown->setEnabled( false );
}

void ToolBarConfigI::toolbarSelected()
{
   currentToolbar = currentToolbarPos();

   buttonToolbarUp->setEnabled( currentToolbar );

   QListViewItem *cur = listToolbar->currentItem();
   if ( cur )
     buttonToolbarDown->setEnabled( cur->nextSibling() );
   else
     buttonToolbarDown->setEnabled( false );

   currentAction = 0;
   updateActionsList();
}

int ToolBarConfigI::currentActionPos()
{
  QListViewItem *item = listAction->currentItem();
  int pos = 0;
  for ( QListViewItem *current = listAction->firstChild(); current; current = current->nextSibling(), pos++ )
    if ( item == current )
      return pos;
  return 0;
}

int ToolBarConfigI::currentToolbarPos()
{
  QListViewItem *item = listToolbar->currentItem();
  int pos = 0;
  for ( QListViewItem *current = listToolbar->firstChild(); current; current = current->nextSibling(), pos++ )
    if ( item == current )
      return pos;
  return 0;
}
