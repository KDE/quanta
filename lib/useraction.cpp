/***************************************************************************
    begin                : ?
    copyright            : (C) ? Dmitry Poplavsky
                           (C) 2002-2006 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//app includes
#include "useraction.h"
#include "editorsource.h"
#include "myprocess.h"
#include "extfileinfo.h"
#include "quantacoreif.h"
#include "tagpair.h"

//qt includes
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QTimer>

//kde includes
#include <kactioncollection.h>
#include <kapplication.h>
#include <kdebug.h>
#include <k3process.h>
#include <kicon.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kxmlguiwindow.h>
#include <kmessagebox.h>
#include <kshortcut.h>
#include <kstandarddirs.h>
#include <ktemporaryfile.h>

#include <kparts/mainwindow.h>

//kdevelop includes
#include <idocument.h>
#include <idocumentcontroller.h>
#include <iplugin.h>
#include <iproject.h>
#include <iplugincontroller.h>
#include <iprojectcontroller.h>
#include <iuicontroller.h>
#include <core.h>


UserAction::UserAction( QDomElement *element, KDevelop::IPlugin *plugin, bool toggle)
    : KToggleAction(element->attribute("text").isEmpty() ? QString("") : i18n(element->attribute("text").toUtf8()), KDevelop::Core::self()->uiController()->activeMainWindow()->actionCollection()),
  m_toggle(toggle)
{
  KDevelop::Core::self()->uiController()->activeMainWindow()->actionCollection()->addAction(element->attribute("name"), this);
  setShortcut(KShortcut(element->attribute("shortcut")));
  m_plugin = plugin;
  m_modified = false;
  m_useInputFile = false;
  m_useOutputFile = false;
  tag = element->cloneNode().toElement();
  QString s = tag.attribute("icon");
  if (!QFileInfo(s).exists())
  {
    s = QFileInfo(s).fileName();
  }
  if (plugin)
  {
    kDebug(24000) << "Load icon: " << s << " from: "<< plugin->iconLoader()->iconPath(s, K3Icon::Small, false) << endl;
    setIcon(KIcon(plugin->iconLoader()->iconPath(s, K3Icon::Small, false)));
  }
  else
    setIcon(KIcon(s));
  m_file = 0L;
  m_loopStarted = false;

  connect(this, SIGNAL(activated()), SLOT(slotActionActivated()));
  connect(this, SIGNAL(showMessage(const QString&, bool)), m_plugin, SLOT(slotShowMessage(const QString&, bool)));
  connect(this, SIGNAL(showOutputView()), m_plugin, SLOT(slotShowOutputView()));
//  connect(this, SIGNAL(clearMessages()), m_plugin, SIGNAL(clearMessages()));
}

UserAction::~UserAction()
{
}

bool UserAction::slotActionActivated()
{
  QuantaCoreIf *quantaCore = KDevelop::Core::self()->pluginController()->extensionForPlugin<QuantaCoreIf>("QuantaCoreIf", "KDevQuantaCore");
  if (!quantaCore)
  {
    KMessageBox::information(KDevelop::Core::self()->uiController()->activeMainWindow(), i18n("You cannot run a tag user action if the QuantaCore plugin is not loaded."), i18n("Missing QuantaCore"), "ShowQuantaCoreMissingWarning");
    return false;
  }
  EditorSource * source = quantaCore->activeEditorSource();
  if (!source)
  {
    kDebug(24000) << "non-document" << endl;
    return false;
  }
  kDebug(24000) << "editor-document" << endl;

  QString type = tag.attribute("type", "");
  if ( type == "tag" )
  {
     QDomElement startTagElement = (tag.namedItem("tag")).toElement();
     QDomElement endTagElement = (tag.namedItem("xtag")).toElement();
     QString endTag = "";
     if (endTagElement.attribute("use","false") == "true" )
     {
       endTag = endTagElement.text();
     }

     quantaCore->insertTag(TagPair(startTagElement.text(), endTag), endTagElement.attribute("inLine","true") == "true", startTagElement.attribute("useDialog","false") == "true");
  } else
#if 0
  QString space="";
  QString output;
  unsigned int line, col;

  Document *w = view->document();
  w->viewCursorIf->cursorPositionReal(&line, &col);
  space.fill( ' ', col);

  if (view->hadLastFocus() != QuantaView::VPLFocus)
  {
#endif
  if ( type == "text" )
  {
    source->insertText(tag.namedItem("text").toElement().text());
  } else
  if ( type == "script" )
  {
    proc = new MyProcess();
    KDevelop::IProject *proj = 0L;
    KDevelop::IDocument *doc = m_plugin->core()->documentController()->activeDocument();
    if (doc)
      proj = m_plugin->core()->projectController()->findProjectForUrl(doc->url());
    if (proj)
      proc->setWorkingDirectory(proj->folder().path());

    QDomElement script = tag.namedItem("script").toElement();
    QString command = script.text();


    if (!source->url().isEmpty())
    {
      QString fname = source->url().url();
      if (source->url().protocol() == "file")
        fname = source->url().path();
      command.replace("%filename", fname );
      command.replace("%f", fname );
    }

    pid_t pid = ::getpid();
    if (kapp->inherits("KUniqueApplication"))
    {
      command.replace("%pid", QString("unique %1").arg(pid));
    } else
    {
      command.replace("%pid", QString("%1").arg(pid));
    }
    QString buffer;
    QString inputType = script.attribute("input","none");


    if ( inputType == "current" )
    {
      buffer = source->text();
    } else
    if ( inputType == "selected") {
      buffer = source->selection();
    }
    command.replace("%input", buffer);
    command = command.trimmed();
    int pos = command.indexOf(' ');
    QString args;
    if (pos != -1)
    {
      args = command.mid(pos+1);
      command = command.left(pos);
    }
    if (command.startsWith("~"))
    {
      command = command.mid(1);
      command.prepend(QDir::homePath());
    }

    *proc << command.trimmed();
    args = args.trimmed();
    if (!args.isEmpty())
    {
      pos = 0;
      while (pos != -1 )
      {
        pos = args.indexOf("%scriptdir");
        QString scriptname;
        if (pos != -1)
        {
          int begin = args.lastIndexOf('"', pos);
          int end = -1;
          if (begin == -1)
          {
            begin = args.lastIndexOf('\'', pos);
            if (begin != -1)
                end = args.indexOf('\'', pos);
          }  else
          {
            end = args.indexOf('"', pos);
          }
          if (begin == -1 || end != -1)
          {
            begin = args.lastIndexOf(' ', pos);
            if (begin == -1)
                begin = 0;
            end = args.indexOf(' ', pos);
            if (end == -1)
                end = args.length();
          }
          scriptname = args.mid(begin, end - begin).trimmed();
          scriptname.replace("%scriptdir","scripts");
 //         kDebug(24000) << "Script name is: |" << scriptname << "|" << endl;
//FIXME: the scrips should be loaded from the language-plugin dir
          scriptname = ' ' + KStandardDirs::locate("data", "kdevusertoolbars/"+scriptname);
   //       kDebug(24000) << "Script found at: " << scriptname << endl;
          args.replace(begin, end - begin, scriptname);
          //kDebug(24000) << "Modified argument list: " << args << endl;
        }
      }
      int pos = args.indexOf("%projectbase");
      if (pos != -1)
      {
          QString s;
          if (proj)
            s = proj->folder().path();
          args.replace("%projectbase", s);
      }
      QStringList argsList1 = args.split(' ');
      QStringList argsList;
      for (int i = 0; i < argsList1.count(); i++)
      {
        if (argsList1[i] == "%userarguments")
        {
          for (int j = 0; j < m_argsList.count(); j++)
          {
            argsList.append(m_argsList[j]);
          }
        } else
          argsList.append(argsList1[i]);
      }
      m_argsList.clear();
      *proc << argsList;
    }
    m_firstOutput = true;
    m_firstError  = true;

    connect( proc, SIGNAL(receivedStdout(   K3Process*,char*,int)), this,
                 SLOT(  slotGetScriptOutput(K3Process*,char*,int)));
    connect( proc, SIGNAL(receivedStderr(   K3Process*,char*,int)), this,
                 SLOT(  slotGetScriptError(K3Process*,char*,int)));
    connect( proc, SIGNAL(processExited(   K3Process*)), this,
                 SLOT(  slotProcessExited(K3Process*)));

    if (!m_useOutputFile)
        scriptOutputDest = script.attribute("output","none");
    else
        scriptOutputDest = "file";
    scriptErrorDest  = script.attribute("error","none");
    if (scriptOutputDest == "message")
    {
//FIXME      emit showMessagesView();
    }

    if (m_useInputFile)
    {
      *proc << m_inputFileName;
    }

    if (proc->start(K3Process::NotifyOnExit, K3Process::All))
    {
/*FIXME      emit clearMessages();
      emit showMessage(i18n("The \"%1\" script started.\n",actionText()), false);*/
      if (!m_useInputFile)
      {
        if ( inputType == "current" || inputType == "selected" )
        {
          proc->writeStdin( buffer.toLocal8Bit(), buffer.length() );
        }
      }
      proc->closeStdin();
    } else
    {
      KMessageBox::error(KDevelop::Core::self()->uiController()->activeMainWindow(), i18n("<qt>There was an error running <b>%1</b>.<br>Check that you have the <i>%2</i> executable installed and it is accessible.</qt>", command + ' ' + args, command), i18n("Script Not Found"));
//FIXME       ViewManager::ref()->activeView()->setFocus();
      if (m_loopStarted)
      {
         emit internalFileInfo.exit_loop();
         m_loopStarted = false;
      }
      return false;
    }
  //}
 }
  return true;
}


void UserAction::scriptDone()
{
  delete proc;
  proc = 0L;
}

QString UserAction::actionText()
{
  QString t = tag.attribute("text");
  int pos = t.indexOf('&');
  if (pos < (int)t.length()-1 && t[pos+1] != '&')
    return t.remove(pos, 1);
  else
    return t;
}


void UserAction::slotActivated()
{
//     if(m_toggle)
    KToggleAction::slotToggled(m_toggle);
    if(!m_toggle)
        setChecked(!isChecked());
}

void UserAction::slotProcessExited(K3Process *process)
{
  if (m_loopStarted)
  {
    emit internalFileInfo.exit_loop();
    m_loopStarted = false;
  }
  delete process;
}

void UserAction::addArguments(const QStringList &arguments)
{
  m_argsList = arguments;
}

void UserAction::execute(bool blocking)
{
  m_useInputFile = false;
  m_useOutputFile = false;
  if (blocking)
  {
    m_useInputFile = !m_inputFileName.isEmpty();
    m_useOutputFile = (m_file);
    if (slotActionActivated())
    {
      //To avoid lock-ups, start a timer.
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), SLOT(slotTimeout()));
        timer->setSingleShot(true);
        timer->start(180 * 1000);
        m_loopStarted = true;
        m_killCount = 0;
        internalFileInfo.enter_loop();
        delete timer;
        m_useInputFile = false;
        m_useOutputFile = false;
    }
  } else
    slotActionActivated();
}

void UserAction::slotGetScriptOutput( K3Process *, char *buffer, int buflen )
{
  QByteArray tmp( buffer, buflen + 1 );
  QString text( QString::fromLocal8Bit(tmp) );
  if (scriptOutputDest == "message")
  {
    if (m_firstOutput)
    {
      emit showOutputView();
      emit showMessage(i18n("The \"%1\" script output:\n", actionText()), false);
      m_firstOutput = false;
    }
    emit showMessage(text, true);
  }
}

void UserAction::slotGetScriptError( K3Process *, char *buffer, int buflen )
{
  QByteArray tmp( buffer, buflen + 1 );
  QString text( QString::fromLocal8Bit(tmp) );
}

/** Timeout occurred while waiting for some network function to return. */
void UserAction::slotTimeout()
{
  if ((m_killCount == 0) && (KMessageBox::questionYesNo(KDevelop::Core::self()->uiController()->activeMainWindow(), i18n("<qt>The filtering action <b>%1</b> seems to be locked.<br>Do you want to terminate it?</qt>", actionText()), i18n("Action Not Responding")) == KMessageBox::Yes))
  {
    if (::kill(-proc->pid(), SIGTERM))
    {
      m_killCount++;
      return;
    }
  }
  if (m_killCount > 0)
  {
    ::kill(-proc->pid(), SIGKILL);
    if (m_loopStarted)
    {
      emit internalFileInfo.exit_loop();
      m_loopStarted = false;
    }
    return;
  }
  timer->setSingleShot(true);
  timer->start(180 * 1000);
}


#include "useraction.moc"
