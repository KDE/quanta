/***************************************************************************
                          spellchecker.h  -  description
                             -------------------
    begin                : Mon Oct 21 2002
    copyright            : (C) 2002 by Andras Mantia
    email                : amantia@freemail.hu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H

#include <qobject.h>

#include <ktexteditor/document.h>

class KSpell;
class KConfig;

/**This class handles the spellchecking. It should work with all KTextEditor's, not only Kate.
   This is not a real plugin. The code is based on SpellPlugin for Kate.
  *@author Andras Mantia
  */

class SpellChecker : public QObject  {
   Q_OBJECT
public: 
  SpellChecker(QObject *parent=0, const char *name=0);
  ~SpellChecker();
  /** Read the KSpell configuration from the Quanta configuration file. */
  void readConfig(KConfig *config);
  /** Write the KSpell configuration into the Quanta configuration file. */
  void writeConfig(KConfig *config);
  /** No descriptions */
  void spellCheck(KTextEditor::Document *document);

public slots:
  void ready();
  void misspelling( const QString&, const QStringList&, unsigned int );
  void corrected  ( const QString&, const QString&, unsigned int);
  void spellResult( const QString& );
  void spellCleanDone();

private:
  void locatePosition( uint pos, uint& line, uint& col );

  KTextEditor::Document *m_currentDoc;
  KSpell *m_kspell;
  int m_mispellCount;
  int m_replaceCount;
    
};

#endif
