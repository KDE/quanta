/***************************************************************************
                          spellchecker.h  -  description
                             -------------------
    begin                : Mon Oct 21 2002
    copyright            : (C) 2002 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
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

  /**
   *  since this class is a singleton you must use this function to access it
   *
   *  the parameter are only used at the first call to create the class
   *
   */
  static SpellChecker* const ref(QWidget *parent = 0L, const char *name = 0L)
  {
    static SpellChecker *m_ref;
    if (!m_ref) m_ref = new SpellChecker (parent, name);
    return m_ref;
  }

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

  /** The constructor is privat because we use singleton patter.
   *  If you need the class use SpellChecker::ref() for
   *  construction and reference
   */
  SpellChecker(QObject *parent=0, const char *name=0);
  void locatePosition( uint pos, uint& line, uint& col );

  KTextEditor::Document *m_currentDoc;
  KSpell *m_kspell;
  int m_mispellCount;
  int m_replaceCount;

};

#endif
