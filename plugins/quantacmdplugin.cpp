/***************************************************************************
                          quantacmdplugin.cpp  -  description
                             -------------------
    begin                : Wed Sep 18 2002
    copyright            : (C) 2002 by Marc Britton
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

/* QT INCLUDES */
#include <qwidget.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qregexp.h>

/* OTHER INCLUDES */
#include "quantacmdplugin.h"
#include "quantaplugin.h"
#include "../quanta.h"
#include "../quantacommon.h"
#include "../messages/messageoutput.h"
#include "../quantadoc.h"
#include "../document.h"

QuantaCmdPlugin::QuantaCmdPlugin(QuantaApp *a_app)
  : QuantaPlugin(), m_firstOutput(FALSE), m_process(0), m_app(a_app)
{
}

QuantaCmdPlugin::~QuantaCmdPlugin()
{
  if(isLoaded())
    unload();
}

bool QuantaCmdPlugin::isLoaded() const
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

  m_process = new KShellProcess;

  QString args = arguments();

  /* TODO
  QString text = m_app->getDoc()->write()->editIf->text();
  args.replace(QRegExp("document"), text);
  text = m_app->getDoc()->write()->selectionIf->selection();
  args.replace(QRegExp("selection"), text); */

  QString loc = location(); // locate first if location not specified
  if(loc.isEmpty())
  {
    const char *fn = fileName().latin1();
    KStandardDirs *dirs = QuantaCommon::pluginDirs("exe");
    loc = dirs->findResource("exe", fn);
    delete dirs;
  }
  else
  {
    loc += QString("/") + fileName();
  }

  QString ow = outputWindow();
  if(ow == "Konsole")
  {
    QString kon = locate("exe", "konsole");
    *m_process << kon << "-e" << loc;
    *m_process << args; // FIXME : Do we need to tokenize arguments here?
  }
  else if(ow == "Message Window")
  {
    *m_process << loc << args;    
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
    connect(m_process, SIGNAL(receivedStdout(KProcess *, char *, int)), SLOT(writeOutput(KProcess *, char *, int)));
    connect(m_process, SIGNAL(processExited(KProcess *)), SLOT(cleanupProcess(KProcess *)));
    if(!m_process->start(KProcess::NotifyOnExit, KProcess::Stdout))
    {
      qWarning("Unable to start process");
      unload();
      return FALSE;
    }
  }
  else
    return FALSE;

  setRunning(TRUE);   
  return TRUE;
}

/** Writes (inserts or appends) process' output */
void QuantaCmdPlugin::writeOutput(KProcess *, char *a_buffer, int a_len)
{
  char *buffer = new char[a_len+1]; //FIXME : is a_buffer 0 terminated? make sure.
  for(int i = 0;i < a_len;++i)
    buffer[i] = a_buffer[i];
  buffer[a_len] = 0;
  QString text(buffer);

  QString ow = outputWindow();
  if(ow == "Message Window")
  {
    if(m_app)
    {
      m_app->getMessages()->showMessage(text);
    }
  }
  else if(ow == "Konsole")
  {
    fprintf(stdin, text.latin1());
  }
  else
    qWarning("Unknown output window %s", ow.latin1());
  /* TODO : More output options */
  
  delete buffer;
}

/** Does post process cleanup */
void QuantaCmdPlugin::cleanupProcess(KProcess *)
{
  unload();
}
