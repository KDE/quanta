/***************************************************************************
                          spellchecker.cpp  -  description
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

//qt includes
#include <qstringlist.h>

//kde includes
#include <kapplication.h>
#include <kconfig.h>
#include <kspell.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kdeversion.h>

#include <ktexteditor/editinterface.h>
#ifdef BUILD_KAFKAPART
#include <ktexteditor/editinterfaceext.h>
#endif
#include <ktexteditor/selectioninterface.h>

//app includes
#include "spellchecker.h"
#include "quantacommon.h"
#include "resource.h"

SpellChecker::SpellChecker(QObject *parent, const char *name ) : QObject(parent,name)
{
}

SpellChecker::~SpellChecker(){
}

/** Write the KSpell configuration into the Quanta configuration file. */
void SpellChecker::writeConfig(KConfig *config)
{
  config->setGroup("Spelling");
  config->writeEntry("Dictionary", qConfig.spellConfig->dictionary());
  config->writeEntry("NoRootAffix", qConfig.spellConfig->noRootAffix());
  config->writeEntry("RunTogether", qConfig.spellConfig->runTogether());
  config->writeEntry("DictFromList", qConfig.spellConfig->dictFromList());
  config->writeEntry("Encoding", qConfig.spellConfig->encoding());
  config->writeEntry("IgnoreList", qConfig.spellConfig->ignoreList());
  config->writeEntry("ReplaceAllList", qConfig.spellConfig->replaceAllList());
  config->writeEntry("Client", qConfig.spellConfig->client());
}

/** Read the KSpell configuration from the Quanta configuration file. */
void SpellChecker::readConfig(KConfig *config)
{
  KSpellConfig *defaultConfig = new KSpellConfig();

  config->setGroup("Spelling");
  QString dictionary = config->readEntry("Dictionary", defaultConfig->dictionary());
  qConfig.spellConfig->setDictionary(dictionary.isNull() ? "" : dictionary);
  bool noRootAffix = config->readBoolEntry("NoRootAffix", defaultConfig->noRootAffix());
  qConfig.spellConfig->setNoRootAffix(noRootAffix);
  bool runTogether = config->readBoolEntry("RunTogether", defaultConfig->runTogether());
  qConfig.spellConfig->setRunTogether(runTogether);
  bool dictFromList = config->readBoolEntry("DictFromList", defaultConfig->dictFromList());
  qConfig.spellConfig->setDictFromList(dictFromList);
  int encoding = config->readNumEntry("Encoding", defaultConfig->encoding());
  qConfig.spellConfig->setEncoding(encoding);
  QStringList ignoreList = config->readListEntry("IgnoreList");
  if (ignoreList.isEmpty()) ignoreList = defaultConfig->ignoreList();
  qConfig.spellConfig->setIgnoreList(ignoreList);
  QStringList replaceAllList = config->readListEntry("ReplaceAllList");
  if (replaceAllList.isEmpty()) replaceAllList = defaultConfig->replaceAllList();
  qConfig.spellConfig->setReplaceAllList(replaceAllList);
  int client = config->readNumEntry("Client", defaultConfig->client());
  qConfig.spellConfig->setClient(client);

  delete defaultConfig;
}

/** No descriptions */
void SpellChecker::spellCheck(KTextEditor::Document *document)
{
  if (document->isReadWrite())
  {
    m_currentDoc = document;
    m_currentDoc->setModified(true);
#if KDE_IS_VERSION(3,1,90)
    m_kspell = new KSpell( 0, i18n("Spellcheck"),
                            this, SLOT(ready()), qConfig.spellConfig, true, false, KSpell::HTML);
#else
    m_kspell = new KSpell( 0, i18n("Spellcheck"),
                            this, SLOT(ready()), qConfig.spellConfig);
#endif
     connect( m_kspell, SIGNAL(death()),
              this, SLOT(spellCleanDone()) );

     connect( m_kspell, SIGNAL(misspelling(const QString&, const QStringList&, unsigned int)),
              this, SLOT(misspelling(const QString&, const QStringList&, unsigned int)) );
     connect( m_kspell, SIGNAL(corrected(const QString&, const QString&, unsigned int)),
              this, SLOT(corrected(const QString&, const QString&, unsigned int)) );
     connect( m_kspell, SIGNAL(done(const QString&)),
              this, SLOT(spellResult(const QString&)) );

  }
}

 void SpellChecker::ready()
{
        m_currentDoc->setReadWrite( false );

        m_mispellCount = 0;
        m_replaceCount = 0;

        m_kspell->setProgressResolution( 1 );

        m_kspell->check( dynamic_cast<KTextEditor::EditInterface*>(m_currentDoc)->text() );
}

void SpellChecker::locatePosition( uint pos, uint& line, uint& col )
{
        KTextEditor::EditInterface* editIf = dynamic_cast<KTextEditor::EditInterface*>(m_currentDoc);
        uint cnt = 0;

        line = col = 0;

        // Find pos  -- CHANGEME: store the last found pos's cursor
        //   and do these searched relative to that to
        //   (significantly) increase the speed of the spellcheck
        for( ; line < editIf->numLines() && cnt <= pos; line++ )
                cnt += editIf->lineLength(line) + 1;

        line--;
        col = pos - (cnt - editIf->lineLength(line)) + 1;
}

void SpellChecker::misspelling( const QString& origword, const QStringList&, uint pos )
{
        m_mispellCount++;

        uint line, col;

        locatePosition( pos, line, col );

        dynamic_cast<KTextEditor::SelectionInterface*>(m_currentDoc)->setSelection( line, col, line, col + origword.length() );
}

void SpellChecker::corrected( const QString& originalword, const QString& newword, uint pos )
{
        KTextEditor::EditInterface* editIf = dynamic_cast<KTextEditor::EditInterface*>(m_currentDoc);
#ifdef BUILD_KAFKAPART
        KTextEditor::EditInterfaceExt* editIfExt = dynamic_cast<KTextEditor::EditInterfaceExt*>(m_currentDoc);
#endif
        m_replaceCount++;

        uint line, col;

        locatePosition( pos, line, col );
        QString lineText = editIf->textLine(line);
        lineText.remove(col,originalword.length());
        lineText.insert(col, newword);
        m_currentDoc->setReadWrite(true);
#ifdef BUILD_KAFKAPART
        if (editIfExt)
          editIfExt->editBegin();
#endif
        editIf->removeLine(line);
        editIf->insertLine(line, lineText);
#ifdef BUILD_KAFKAPART
        if (editIfExt)
          editIfExt->editEnd();
#endif
        m_currentDoc->setReadWrite(false);
}

void SpellChecker::spellResult( const QString& )
{
        dynamic_cast<KTextEditor::SelectionInterface*>(m_currentDoc)->clearSelection();
        m_currentDoc->setReadWrite( true );
        m_kspell->cleanUp();
}

void SpellChecker::spellCleanDone()
{
        KSpell::spellStatus status = m_kspell->status();

        if( status == KSpell::Error ) {
                KMessageBox::sorry( 0,
                  i18n("ISpell could not be started. "
                       "Please make sure you have ISpell "
                       "properly configured and in your PATH."));
        } else if( status == KSpell::Crashed ) {
                m_currentDoc->setReadWrite( true );
                KMessageBox::sorry( 0,
                  i18n("ISpell seems to have crashed."));
        }

        delete m_kspell;
        m_kspell = 0;
}


#include "spellchecker.moc"
