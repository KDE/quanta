/***************************************************************************
                          quantacmdplugin.cpp  -  description
                             -------------------
    begin                : Wed Sep 18 2002
    copyright            : (C) 2002 by Marc Britton
                           (C) 2003 Andras Mantia <amantia@freemail.hu>
    email                : consume@optushome.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <cstdio>

/* KDE INCLUDES */
#include <kprocess.h>
#include <kstandarddirs.h>
#include <klocale.h>

/* QT INCLUDES */
#include <qwidget.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <qfile.h>

/* OTHER INCLUDES */
#include "quantacmdplugin.h"
#include "quantaplugin.h"
#include "../quanta.h"
#include "../quantacommon.h"
#include "../messages/messageoutput.h"
#include "../quantadoc.h"
#include "../document.h"
#include "../resource.h"

QuantaCmdPlugin::QuantaCmdPlugin()
  : QuantaPlugin(), m_firstOutput(FALSE), m_process(0)
{
}

QuantaCmdPlugin::~QuantaCmdPlugin()
{
  if(isLoaded())
    unload();
}

bool QuantaCmdPlugin::isLoaded()
{
  return m_process != 0;
}

bool QuantaCmdPlugin::unload()
{
  if(m_process)
    delete m_process;
  m_process = 0;

  m_firstOutput = TRUE;

  setRunning(FALSE);

  return TRUE;
}

bool QuantaCmdPlugin::load()
{
  if(isLoaded())
    return FALSE;

  m_process = new KProcess;
  m_process->setUseShell(true);

  QString args = arguments();
  
  if (!args.isEmpty())
  {
    //FIXME: This will not work if the arguments contain spaces, but will
    //do it for now, until the plugin system is corrected
    QStringList argsList = QStringList::split(" ", args);
    args = "";
    for (uint i = 0; i < argsList.count(); i++)
    {
      args += KProcess::quote(argsList[i])+" ";
    }
  }
  
  /* TODO
  QString text = quantaApp->doc()->write()->editIf->text();
  args.replace(QRegExp("document"), text);
  text = quantaApp->doc()->write()->selectionIf->selection();
  args.replace(QRegExp("selection"), text); */

  QString loc = location(); // locate first if location not specified
  if(loc.isEmpty())
  {
    QString fn = fileName();
    KStandardDirs *dirs = QuantaCommon::pluginDirs("exe");
    loc = dirs->findResource("exe", fn);
    delete dirs;
  }
  else
  {
    loc += QString("/") + fileName();
  }

  QString ow = outputWindow();
  if(ow == i18n("Konsole"))
  {
    QString kon = locate("exe", "konsole");
    *m_process << kon << "-e " << KProcess::quote(loc);
    *m_process << args; // FIXME : Do we need to tokenize arguments here?
  }
  else if(ow == i18n("Message Window"))
  {
    *m_process << KProcess::quote(loc) << args;
  }
  else
    qWarning("Unknown output window %s", ow.latin1());

  return TRUE;
}

bool QuantaCmdPlugin::run()
{
  if(!isLoaded())
    load();

  if(isLoaded())
  {
    connect(m_process, SIGNAL(receivedStdout(KProcess *, char *, int)), SLOT(writeStdout(KProcess *, char *, int)));
    connect(m_process, SIGNAL(receivedStderr(KProcess *, char *, int)), SLOT(writeStderr(KProcess *, char *, int)));
    connect(m_process, SIGNAL(processExited(KProcess *)), SLOT(cleanupProcess(KProcess *)));
    if(!m_process->start(KProcess::NotifyOnExit, KProcess::AllOutput))
    {
      qWarning("Unable to start process");
      unload();
      return FALSE;
    }
    emit pluginStarted();
  }
  else
    return FALSE;

  setRunning(TRUE);
  return TRUE;
}
// FIXME : writeStdout and writeStderr can currently only write to the same output window!

/** Writes (inserts or appends) process' output */
void QuantaCmdPlugin::writeStdout(KProcess *, char *a_buffer, int a_len)
{
  QString text = makeQString(a_buffer, a_len);

  QString ow = outputWindow();
  if(ow == i18n("Message Window"))
  {
    if(quantaApp)
    {
      quantaApp->getMessages()->showMessage(text);
    }
  }
  else if(ow == i18n("Konsole"))
  {
    fprintf(stdin, text.local8Bit());
  }
  else if(ow == i18n("None"))
  {
  }
  else
    qWarning("Unknown output window %s", ow.latin1());
  /* TODO : More output options */


  emit wroteStdout(text);
}

void QuantaCmdPlugin::writeStderr(KProcess *, char *a_buffer, int a_len)
{
  QString text = makeQString(a_buffer, a_len);

  QString ow = outputWindow();
  if(ow == i18n("Message Window"))
  {
    if(quantaApp)
    {
      quantaApp->getMessages()->showMessage(text);
    }
  }
  else if(ow == i18n("Konsole"))
  {
    fprintf(stdin, text.local8Bit());
  }
  else if(ow == i18n("None"))
  {
  }
  else
    qWarning("Unknown output window %s", ow.latin1());
  /* TODO : More output options */

  emit wroteStderr(text);
}

QString QuantaCmdPlugin::makeQString(const char *a_buffer, int a_len)
{
  char *buffer = new char[a_len+1]; //FIXME : is a_buffer 0 terminated? make sure.
  for(int i = 0;i < a_len;++i)
    buffer[i] = a_buffer[i];
  buffer[a_len] = 0;
  QString text(buffer);
  delete[] buffer;
  return text;
}

/** Does post process cleanup */
void QuantaCmdPlugin::cleanupProcess(KProcess *)
{
  unload();
  emit pluginStopped();
}

#include "quantacmdplugin.moc"
