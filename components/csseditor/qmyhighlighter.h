/***************************************************************************
                          bashhighlighter.h  -  description
                             -------------------
    begin                : dom mar 16 2003
    copyright            : (C) 2003 by Emiliano Gulmini
    email                : emi_barbarossa@yahoo.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QMYHIGHLIGHTER_H
#define QMYHIGHLIGHTER_H

#include <qsyntaxhighlighter.h>

/**
  *@author Emiliano Gulmini
  */

class QMyHighlighter : public QSyntaxHighlighter  {
public: 
	QMyHighlighter(QTextEdit* Qtxt);

	~QMyHighlighter();
  int highlightParagraph ( const QString & text, int endStateOfLastPara ); 

};

#endif
