/***************************************************************************
                          csseditor.cpp  -  description
                             -------------------
    begin                : gio ago 29 2002
    copyright            : (C) 2002 by Andrea Bergia, Andras Mantia
    email                : andreabergia@yahoo.it, amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// QT includes
#include <qstringlist.h>

// KDE includes
#include <klocale.h>

// QT & KDE widget includes
#include <kpushbutton.h>
#include <qlistbox.h>
#include <kmessagebox.h>
#include <kstdguiitem.h>

// App includes
#include "csseditor.h"
#include "cssselectoreditor.h"

CSSEditor::CSSEditor (QString code, QWidget* parent, const char* name)
	: CSSEditorS (parent, name)
{
	// UI things
	connect (buttonSave, SIGNAL (clicked()), SLOT (accept()));
	connect (buttonAdd, SIGNAL (clicked()), SLOT (slotAddSelector()));
	connect (buttonRemove, SIGNAL (clicked()), SLOT (slotRemoveSelector()));
	connect (buttonEdit, SIGNAL (clicked()), SLOT (slotEditSelector()));
	connect (buttonCopy, SIGNAL (clicked()), SLOT (slotCopySelector()));

	// Ok, done. Now, let's parse all the selectors
  selectors = QStringList::split ("}", code);
  QStringList::Iterator it = selectors.begin();
  do
  {
		if ((*it).simplifyWhiteSpace() == "")
			it = selectors.remove (it);
		else
			*it += "}";
    if (it != selectors.end()) it++;
	} while (it != selectors.end());

	slotShowSelectors();
}

CSSEditor::~CSSEditor()
{
}

/** Show the window to add a selector */
void CSSEditor::slotAddSelector()
{
	CSSSelectorEditor* dlg = new CSSSelectorEditor ("", false, this,
		i18n ("Insert a new selector"));
	if (dlg->exec()) {
		selectors += dlg->code();
		slotShowSelectors();
	}
  delete dlg;
}

/** Show the window to edit an existing selector */
void CSSEditor::slotEditSelector()
{
	int i = listRules->currentItem();
	if (i == -1) {
		KMessageBox::sorry (this,
		i18n ("Sorry, but you must select an item before trying to edit it."));
		return;
	}
	CSSSelectorEditor* dlg = new CSSSelectorEditor (selectors[i], false, this,
		i18n ("Edit an existing CSS selector"));
	if (dlg->exec()) {
		selectors[i] = dlg->code();
		slotShowSelectors();
	}
  delete dlg;
}

/** Copy a selector into anothers */
void CSSEditor::slotCopySelector()
{
	int i = listRules->currentItem();
	if (i == -1)
  {
		KMessageBox::sorry (this,
		i18n ("Sorry, but you must select an item before trying to copy it."));
		return;
	}
	selectors += selectors[i];
	slotShowSelectors();
}

/** Ask to remove an existing selector */
void CSSEditor::slotRemoveSelector()
{
	int i = listRules->currentItem();
	if (i == -1)
  {
		KMessageBox::sorry (this,
		i18n ("Sorry, but you must select an item before trying to edit it."));
		return;
	}
	if (KMessageBox::warningYesNo (this,
		i18n("Are you sure do you want to completly remove the current selector? "
		"There's no way to restore it."), i18n ("Remove selector warning"),
		KStdGuiItem::yes(), KStdGuiItem::no(), "css-remove-selector-warning")
		== KMessageBox::Yes) {
    	QStringList::Iterator it = selectors.begin();
		for (int j = 0; j < listRules->currentItem(); j++)
			it++;
		selectors.remove(it);
		slotShowSelectors();
	}
}

/** Place all the selector contained in the string list into the list box */
void CSSEditor::slotShowSelectors()
{
	listRules->clear();
	QStringList::Iterator it = selectors.begin();
	for (; it != selectors.end(); it++)
  {
		QString name = (*it).mid(0, (*it).find("{"));
		listRules->insertItem(name.simplifyWhiteSpace());
	}
}

/** Return the complete CSS code */
QString CSSEditor::code()
{
	QString text = "";
	for (unsigned i = 0; i < selectors.size(); i++)
		text += selectors[i] + "\n";
	return text;
}

