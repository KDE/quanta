/***************************************************************************
                          cssselectoreditor.h  -  description
                             -------------------
    begin                : dom ago 25 2002
    copyright            : (C) 2002 by Andras Mantia, Andrea Bergia
    email                : amantia@freemail.hu, andreabergia@yahoo.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "selectoreditors.h"

/** @author Andrea Bergia */
class CSSSelectorEditor : public CSSEditSelectorS  {
   Q_OBJECT
public:
	CSSSelectorEditor(QString code, bool inlin,
		QWidget *parent=0, const char *name=0);
	~CSSSelectorEditor();

	/** Get the CSS code based on the widget's values */
	QString code();

protected:
	/** From the code, insert the values on the widgets */
	void widgetFromCode(QString);

	/** Split a string in the form top [right]? [bottom]? [left]? into the
	four variables */
	void splitValueTRBL(QString value, QString&, QString&, QString&, QString&);

private:
	bool code_inline;
};
