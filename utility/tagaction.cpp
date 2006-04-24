/***************************************************************************
                          tagaction.cpp  -  description
                             -------------------
    begin                : ?
    copyright            : (C) ? Dmitry Poplavsky
                           (C) 2002-2005 Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//other includes
#include <sys/types.h>
#include <unistd.h>


//qt includes
#include <qdir.h>
#include <qdom.h>
#include <qfile.h>
#include <qtimer.h>

//kde includes
#include <kapplication.h>
#include <kdebug.h>
#include <kprocess.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kshortcut.h>
#include <kstandarddirs.h>
#include <ktempfile.h>
#include <ktexteditor/document.h>
#include <ktexteditor/viewcursorinterface.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/selectioninterface.h>
#include <ktexteditor/selectioninterfaceext.h>

//app includes
#include "tagaction.h"
#include "myprocess.h"
#include "document.h"
#include "quantaview.h"
#include "quanta.h"
// #include "quantadoc.h"
#include "tagdialog.h"
#include "messageoutput.h"
#include "quantacommon.h"
#include "resource.h"
#include "qextfileinfo.h"
#include "undoredo.h"
#include "kafkacommon.h"
#include "wkafkapart.h"
#include "cursors.h"
#include "tag.h"
#include "project.h"

#include "viewmanager.h"

MyProcess::MyProcess():KProcess()
{
}

int MyProcess::commSetupDoneC()
{
  ::setpgid(pid_, 0);
  return KProcess::commSetupDoneC();
}

TagAction::TagAction( QDomElement *element, KMainWindow *parentMainWindow, bool toggle)
    : KToggleAction(element->attribute("text").isEmpty() ? QString("") : i18n(element->attribute("text").utf8()), 
                    KShortcut(element->attribute("shortcut")), 0, 0, parentMainWindow->actionCollection(), element->attribute("name")),
 //disable toggle now m_toggle(toggle)
  m_toggle(false)
{
  setToolTip(element->attribute("tooltip"));
  m_parentMainWindow = parentMainWindow;
  m_modified = false;
  m_useInputFile = false;
  m_useOutputFile = false;
  tag = element->cloneNode().toElement();
  QString s = tag.attribute("icon");
  if (!QFileInfo(s).exists())
  {
    s = QFileInfo(s).fileName();
  }
  setIcon(s);
  m_file = 0L;
  loopStarted = false;
#if KDE_VERSION >= KDE_MAKE_VERSION(3,4,0)
  connect(this, SIGNAL(activated(KAction::ActivationReason, Qt::ButtonState)), 
          SLOT(slotActionActivated(KAction::ActivationReason, Qt::ButtonState)));
#else
  connect(this, SIGNAL(activated()), SLOT(slotActionActivated()));
#endif
  connect(this, SIGNAL(showMessage(const QString&, bool)), m_parentMainWindow, SIGNAL(showMessage(const QString&, bool)));
  connect(this, SIGNAL(clearMessages()), m_parentMainWindow, SIGNAL(clearMessages()));
  connect(this, SIGNAL(showMessagesView()), m_parentMainWindow, SLOT(slotShowMessagesView()));
  connect(this, SIGNAL(createNewFile()), m_parentMainWindow, SLOT(slotFileNew()));
}

TagAction::~TagAction()
{
}

QString TagAction::type()
{
   return tag.attribute("type","");
}

#if KDE_VERSION >= KDE_MAKE_VERSION(3,4,0)
bool TagAction::slotActionActivated(KAction::ActivationReason reason, Qt::ButtonState /*state*/)
{
    QuantaView *view = ViewManager::ref()->activeView();
    if ( !view || !view->document())
        return false;

    unsigned int line, col;
    Document *w = view->document();
    w->viewCursorIf->cursorPositionReal(&line, &col);
    NodeModifsSet* modifs = new NodeModifsSet();
    
    QString space;
    space.fill( ' ', col);

    QString type = tag.attribute("type","");

    if ( type == "tag" && view->hadLastFocus() == QuantaView::VPLFocus && toggable())
    {        
        KafkaWidget* kafka_widget = KafkaDocument::ref()->getKafkaWidget();
        QString tag_name = XMLTagName();
        
        NodeSelectionInd selection;
        selection.fillWithVPLCursorSelection();

        Node* start_node = 0, *end_node = 0, *current_node = 0; 
        int start_offset = 0, end_offset = 0, current_offset = 0;        
        QString scope;
        if(kafka_widget->hasSelection())
        {
            // get selection
            start_node = kafkaCommon::getNodeFromLocation(selection.cursorNode());
            end_node = kafkaCommon::getNodeFromLocation(selection.cursorNodeEndSel());
            current_node = end_node;
            start_offset = selection.cursorOffset();
            end_offset = selection.cursorOffsetEndSel();
            current_offset = end_offset;
        }
        else
        {
            current_node = kafkaCommon::getNodeFromLocation(selection.cursorNode());
            Q_ASSERT(current_node);
            if (current_node)
            {
              current_offset = selection.cursorOffset();
              
              start_node = end_node = current_node;
              start_offset = end_offset = current_offset;
  
              QTag* tag_description = QuantaCommon::tagFromDTD(KafkaDocument::ref()->getCurrentDoc()->defaultDTD(), XMLTagName());
              scope = tag_description->scope();
  //             Q_ASSERT(!scope.isNull());
              if(scope.isNull())
                  scope = "word"; // FIXME temporary
              
              if(scope.lower() == "word")
              {
                  // Apply/deapply the tag in the word
                  if(kafkaCommon::isBetweenWords(current_node, current_offset))
                  {
                      kafkaCommon::getStartOfWord(start_node, start_offset);
                      kafkaCommon::getEndOfWord(end_node, end_offset);
                  }
              }
              else if(scope.lower() == "paragraph")
              {
                  kafkaCommon::getStartOfParagraph(start_node, start_offset);
                  kafkaCommon::getEndOfParagraph(end_node, end_offset);
              }
              else if(reason != KAction::EmulatedActivation) // is between words: save the state and return
              {
                  if(!toggled())
                      quantaApp->insertTagActionPoolItem(name());
                  else
                      quantaApp->removeTagActionPoolItem(name());
                  
                  return true;
              }
            }
        }        
        Q_ASSERT(start_node && end_node);
        
/*        kdDebug(23100) << "start node string: " << start_node->tag->tagStr() << endl;    
        kdDebug(23100) << "start node offset: " << start_offset << endl;    
        kdDebug(23100) << "start node string length: " << start_node->tag->tagStr().length() << endl;    */
        if (!start_node || !end_node)
          return true; //FIXME: AndraS: don't crash
        if(scope != "paragraph") {
            start_node = kafkaCommon::getCorrectStartNode(start_node, start_offset);
            end_node = kafkaCommon::getCorrectEndNode(end_node, end_offset);
            if (!start_node || !end_node)
              return true; //FIXME: AndraS: don't crash
        }
        NodeSelection cursor_holder;
        cursor_holder.setCursorNode(current_node);
        cursor_holder.setCursorOffset(current_offset);
        
        int inside_tag = kafkaCommon::isInsideTag(start_node, end_node, tag_name);
        if(inside_tag == -1)
        {
            applyTagInSelection(start_node, start_offset, end_node, end_offset, cursor_holder, modifs);
        }
        else if(inside_tag == 1)
        {
            QString attribute_name(tag.attribute("attribute_name", QString()));
            QString attribute_value(tag.attribute("attribute_value", QString()));
    
            // special case
            if(!attribute_name.isEmpty() && !attribute_value.isEmpty())
            {
                Node* tag_parent = kafkaCommon::hasParent(start_node, end_node, tag_name);
                
                Node* aux1 = start_node->previousSibling();
                while(aux1->tag->type == Tag::Empty)
                    aux1 = aux1->previousSibling();
                Node* aux2 = end_node->nextSibling();
                while(aux2->tag->type == Tag::Empty)
                    aux2 = aux2->nextSibling();
                
                if(aux1 == tag_parent && aux2 == tag_parent->getClosingNode())
                {
                    if(tag_parent->tag->attributeValue(attribute_name, true) == attribute_value)
                        kafkaCommon::editNodeAttribute(tag_parent, attribute_name, QString(), modifs);
                    else
                        kafkaCommon::editNodeAttribute(tag_parent, attribute_name, attribute_value, modifs);
                }
                else
                    applyTagInSelection(start_node, start_offset, end_node, end_offset, cursor_holder, modifs); 
            }
            else
                deapplyTagInSelection(start_node, start_offset, end_node, end_offset, cursor_holder, modifs);
        }
        else
        {
            applyTagInMixedSelection(start_node, start_offset, end_node, end_offset, cursor_holder, modifs);
        }
        w->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif, &cursor_holder);
        KafkaDocument::ref()->getKafkaWidget()->setCurrentNode(cursor_holder.cursorNode(), cursor_holder.cursorOffset());
        return true;
    }        
        
    if ( type == "tag" ) {
        QDomElement otag = (tag.namedItem("tag")).toElement();
        QDomElement xtag = (tag.namedItem("xtag")).toElement();

        QString attr = otag.text();
        if ( attr[0] == '<' )
            attr.remove(0,1);
        if ( attr.right(1) == ">" )
            attr.remove( attr.length()-1, 1 );
        attr = attr.stripWhiteSpace();
        int i = 0;
        while ( !attr[i].isSpace() && !attr[i].isNull() )  i++;
        QString name = attr.left(i);
        attr = attr.remove(0,i).stripWhiteSpace();

        if (otag.attribute("useDialog","false") == "true" && QuantaCommon::isKnownTag(w->defaultDTD()->name, name))
        {
            view->insertNewTag(name, attr, xtag.attribute("inLine","true") == "true");
        }
        else
        {
            QString s1 = QuantaCommon::tagCase(name);
            if (otag.text().left(1) == "<") s1 = "<"+s1;
            if (!attr.isEmpty())
                s1 += " "+QuantaCommon::attrCase(attr);
            if (otag.text().right(1) == ">")
            {
                QTag *dtdTag = QuantaCommon::tagFromDTD(w->defaultDTD(), name);
                if ( w->defaultDTD()->singleTagStyle == "xml" && dtdTag &&
                     (dtdTag->isSingle() || (!qConfig.closeOptionalTags && dtdTag->isOptional()))
                   )
                {
                    s1.append(" /");
                }

                s1.append(">");
            }

            QString s2;
            if ( xtag.attribute("use","false") == "true" )
            {
                if (qConfig.closeTags)
                    s2 = QuantaCommon::tagCase(xtag.text());
                if ( xtag.attribute("inLine","true") == "true" )
                {
                    /** FIXME this is quick and temporary */
                    view->insertOutputInTheNodeTree(s1, s2);
                }
                else
                {
                    view->insertOutputInTheNodeTree(s1, s2);
                }
            }
            else
                view->insertOutputInTheNodeTree(s1, s2);
        }
    }

    if (view->hadLastFocus() != QuantaView::VPLFocus)
    {


        if ( type == "text" )
            w->insertTag( tag.namedItem("text").toElement().text() );

        if ( type == "script" )
        {
            proc = new MyProcess();
            proc->setWorkingDirectory(quantaApp->projectBaseURL().path());

            QDomElement script = tag.namedItem("script").toElement();
            QString command = script.text();


            if ( !w->isUntitled() ) {
                QString fname = w->url().url();
                if ( w->url().protocol() == "file")
                    fname = w->url().path();
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

            if ( inputType == "current" ) {
                buffer = w->editIf->text();
            } else
                if ( inputType == "selected" && w->selectionIf) {
                    buffer = w->selectionIf->selection();
                }
                command.replace("%input", buffer);
                command = command.stripWhiteSpace();
                int pos = command.find(' ');
                QString args;
                if (pos != -1)
                {
                    args = command.mid(pos+1);
                    command = command.left(pos);
                }
                if (command.startsWith("~"))
                {
                    command = command.mid(1);
                    command.prepend(QDir::homeDirPath());
                }

                *proc << command.stripWhiteSpace();
                args = args.stripWhiteSpace();
                if (!args.isEmpty())
                {
                    pos = 0;
                    while (pos != -1 )
                    {
                        pos = args.find("%scriptdir");
                        QString scriptname;
                        if (pos != -1)
                        {
                            int begin = args.findRev('"', pos);
                            int end = -1;
                            if (begin == -1)
                            {
                                begin = args.findRev('\'', pos);
                                if (begin != -1)
                                    end = args.find('\'', pos);
                            }  else
                            {
                                end = args.find('"', pos);
                            }
                            if (begin == -1 || end != -1)
                            {
                                begin = args.findRev(' ', pos);
                                if (begin == -1)
                                    begin = 0;
                                end = args.find(' ', pos);
                                if (end == -1)
                                    end = args.length();
                            }
                            scriptname = args.mid(begin, end - begin).stripWhiteSpace();
                            scriptname.replace("%scriptdir","scripts");
   //       kdDebug(24000) << "Script name is: |" << scriptname << "|" << endl;
                            scriptname = " " + locate("appdata", scriptname);
   //       kdDebug(24000) << "Script found at: " << scriptname << endl;
                            args.replace(begin, end - begin, scriptname);
   //       kdDebug(24000) << "Modified argument list: " << args << endl;
                        }
                    }
                    int pos = args.find("%projectbase");
                    if (pos != -1)
                    {
                        QString s;
                        if (Project::ref()->hasProject())
                            s = Project::ref()->projectBaseURL().url();
                        args.replace("%projectbase", s);
                    }
                    QStringList argsList1 = QStringList::split(' ', args);
                    QStringList argsList;
                    for (uint i = 0; i < argsList1.count(); i++)
                    {
                        if (argsList1[i] == "%userarguments")
                        {
                            for (uint j = 0; j < m_argsList.count(); j++)
                            {
                                argsList.append(m_argsList[j]);
                            }      
                        } else
                            argsList.append(argsList1[i]);
                    }
                    m_argsList.clear();
                    *proc << argsList;
                }
                firstOutput = true;
                firstError  = true;

                connect( proc, SIGNAL(receivedStdout(   KProcess*,char*,int)), this,
                         SLOT(  slotGetScriptOutput(KProcess*,char*,int)));
                connect( proc, SIGNAL(receivedStderr(   KProcess*,char*,int)), this,
                         SLOT(  slotGetScriptError(KProcess*,char*,int)));
                connect( proc, SIGNAL(processExited(   KProcess*)), this,
                         SLOT(  slotProcessExited(KProcess*)));



                if (!m_useOutputFile)
                    scriptOutputDest = script.attribute("output","none");
                else
                    scriptOutputDest = "file";
                scriptErrorDest  = script.attribute("error","none");
                if (scriptOutputDest == "message")
                {
                    emit showMessagesView();
                }

                if (m_useInputFile)
                {
                    *proc << m_inputFileName;
                }

                if (proc->start(KProcess::NotifyOnExit, KProcess::All))
                {
                    emit clearMessages();
                    emit showMessage(i18n("The \"%1\" script started.\n").arg(actionText()), false);
                    if (!m_useInputFile)
                    {
                        if ( inputType == "current" || inputType == "selected" )
                        {
                            proc->writeStdin( buffer.local8Bit(), buffer.length() );
                        }
                    }
                    proc->closeStdin();
                } else
                {
                    KMessageBox::error(m_parentMainWindow, i18n("<qt>There was an error running <b>%1</b>.<br>Check that you have the <i>%2</i> executable installed and it is accessible.</qt>").arg(command + " " + args).arg(command), i18n("Script Not Found"));
                    ViewManager::ref()->activeView()->setFocus();
                    if (loopStarted)
                    {
                        qApp->exit_loop();
                        loopStarted = false;
                    }
                    return false;
                }
        }
    }    
    return true;
}
#else
 // hack to compile. moc doesn't check the "#ifdef" at the declaration and the compiler complains 
 // of no matching function.
bool TagAction::slotActionActivated(KAction::ActivationReason /*reason*/, Qt::ButtonState /*state*/)
{return true;}
#endif

bool TagAction::slotActionActivated()
{
  QuantaView *view = ViewManager::ref()->activeView();
  if ( !view || !view->document())
     return false;

  QString space="";
  QString output;
  unsigned int line, col;

  Document *w = view->document();
  w->viewCursorIf->cursorPositionReal(&line, &col);
  space.fill( ' ', col);

  QString type = tag.attribute("type","");

  if ( type == "tag" ) {
     QDomElement otag = (tag.namedItem("tag")).toElement();
     QDomElement xtag = (tag.namedItem("xtag")).toElement();

     QString attr = otag.text();
     if ( attr[0] == '<' )
         attr.remove(0,1);
     if ( attr.right(1) == ">" )
         attr.remove( attr.length()-1, 1 );
     attr = attr.stripWhiteSpace();
     int i = 0;
     while ( !attr[i].isSpace() && !attr[i].isNull() )  i++;
     QString name = attr.left(i);
     attr = attr.remove(0,i).stripWhiteSpace();

     if (otag.attribute("useDialog","false") == "true" && QuantaCommon::isKnownTag(w->defaultDTD()->name, name))
     {
       view->insertNewTag(name, attr, xtag.attribute("inLine","true") == "true");
     }
     else
     {
       QString s1 = QuantaCommon::tagCase(name);
       if (otag.text().left(1) == "<") s1 = "<"+s1;
       if (!attr.isEmpty())
          s1 += " "+QuantaCommon::attrCase(attr);
       if (otag.text().right(1) == ">")
       {
         QTag *dtdTag = QuantaCommon::tagFromDTD(w->defaultDTD(), name);
         if ( w->defaultDTD()->singleTagStyle == "xml" && dtdTag &&
              (dtdTag->isSingle() || (!qConfig.closeOptionalTags && dtdTag->isOptional()))
            )
         {
           s1.append(" /");
         }

         s1.append(">");
       }

       QString s2;
       if ( xtag.attribute("use","false") == "true" )
       {
         if (qConfig.closeTags)
            s2 = QuantaCommon::tagCase(xtag.text());
         if ( xtag.attribute("inLine","true") == "true" )
         {
           /** FIXME this is quick and temporary */
           view->insertOutputInTheNodeTree(s1, s2);
         }
         else
         {
           view->insertOutputInTheNodeTree(s1, s2);
         }
       }
       else
         view->insertOutputInTheNodeTree(s1, s2);
     }
  }

  if (view->hadLastFocus() != QuantaView::VPLFocus)
  {


  if ( type == "text" )
    w->insertTag( tag.namedItem("text").toElement().text() );

  if ( type == "script" )
  {
    proc = new MyProcess();
    proc->setWorkingDirectory(quantaApp->projectBaseURL().path());

    QDomElement script = tag.namedItem("script").toElement();
    QString command = script.text();


    if ( !w->isUntitled() ) {
      QString fname = w->url().url();
      if ( w->url().protocol() == "file")
        fname = w->url().path();
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

    if ( inputType == "current" ) {
      buffer = w->editIf->text();
    } else
    if ( inputType == "selected" && w->selectionIf) {
      buffer = w->selectionIf->selection();
    }
    command.replace("%input", buffer);
    command = command.stripWhiteSpace();
    int pos = command.find(' ');
    QString args;
    if (pos != -1)
    {
      args = command.mid(pos+1);
      command = command.left(pos);
    }
    if (command.startsWith("~"))
    {
      command = command.mid(1);
      command.prepend(QDir::homeDirPath());
    }

    *proc << command.stripWhiteSpace();
    args = args.stripWhiteSpace();
    if (!args.isEmpty())
    {
      pos = 0;
      while (pos != -1 )
      {
        pos = args.find("%scriptdir");
        QString scriptname;
        if (pos != -1)
        {
          int begin = args.findRev('"', pos);
          int end = -1;
          if (begin == -1)
          {
            begin = args.findRev('\'', pos);
            if (begin != -1)
                end = args.find('\'', pos);
          }  else
          {
            end = args.find('"', pos);
          }
          if (begin == -1 || end != -1)
          {
            begin = args.findRev(' ', pos);
            if (begin == -1)
                begin = 0;
            end = args.find(' ', pos);
            if (end == -1)
                end = args.length();
          }
          scriptname = args.mid(begin, end - begin).stripWhiteSpace();
          scriptname.replace("%scriptdir","scripts");
   //       kdDebug(24000) << "Script name is: |" << scriptname << "|" << endl;
          scriptname = " " + locate("appdata", scriptname);
   //       kdDebug(24000) << "Script found at: " << scriptname << endl;
          args.replace(begin, end - begin, scriptname);
   //       kdDebug(24000) << "Modified argument list: " << args << endl;
        }
      }
      int pos = args.find("%projectbase");
      if (pos != -1)
      {
          QString s;
          if (Project::ref()->hasProject())
            s = Project::ref()->projectBaseURL().url();
          args.replace("%projectbase", s);
      }
      QStringList argsList1 = QStringList::split(' ', args);
      QStringList argsList;
      for (uint i = 0; i < argsList1.count(); i++)
      {
        if (argsList1[i] == "%userarguments")
        {
          for (uint j = 0; j < m_argsList.count(); j++)
          {
            argsList.append(m_argsList[j]);
          }      
        } else
          argsList.append(argsList1[i]);
      }
      m_argsList.clear();
      *proc << argsList;
    }
    firstOutput = true;
    firstError  = true;

    connect( proc, SIGNAL(receivedStdout(   KProcess*,char*,int)), this,
                 SLOT(  slotGetScriptOutput(KProcess*,char*,int)));
    connect( proc, SIGNAL(receivedStderr(   KProcess*,char*,int)), this,
                 SLOT(  slotGetScriptError(KProcess*,char*,int)));
    connect( proc, SIGNAL(processExited(   KProcess*)), this,
                 SLOT(  slotProcessExited(KProcess*)));



    if (!m_useOutputFile)
        scriptOutputDest = script.attribute("output","none");
    else
        scriptOutputDest = "file";
    scriptErrorDest  = script.attribute("error","none");
    if (scriptOutputDest == "message")
    {
      emit showMessagesView();
    }

    if (m_useInputFile)
    {
      *proc << m_inputFileName;
    }

    if (proc->start(KProcess::NotifyOnExit, KProcess::All))
    {
      emit clearMessages();
      emit showMessage(i18n("The \"%1\" script started.\n").arg(actionText()), false);
      if (!m_useInputFile)
      {
        if ( inputType == "current" || inputType == "selected" )
        {
          proc->writeStdin( buffer.local8Bit(), buffer.length() );
        }
      }
      proc->closeStdin();
    } else
    {
      KMessageBox::error(m_parentMainWindow, i18n("<qt>There was an error running <b>%1</b>.<br>Check that you have the <i>%2</i> executable installed and it is accessible.</qt>").arg(command + " " + args).arg(command), i18n("Script Not Found"));
      ViewManager::ref()->activeView()->setFocus();
      if (loopStarted)
      {
          qApp->exit_loop();
          loopStarted = false;
      }
      return false;
    }
  }
 }
  return true;
}

void TagAction::slotGetScriptOutput( KProcess *, char *buffer, int buflen )
{
  QCString tmp( buffer, buflen + 1 );
  QString text( QString::fromLocal8Bit(tmp) );
//  kdDebug(24000) << "Script output received: |" << text << "|" << endl;
  Document *w = ViewManager::ref()->activeDocument();
  if (!w)
  {
    kdDebug(24000) << "Document not found." << endl;
    return;
  }
  if ( scriptOutputDest == "cursor" )
  {
     w->insertTag( text );
  } else
  if ( scriptOutputDest == "selection" )
  {
     if ( firstOutput )
     {
       int line = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndLine();
       int col = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndCol();
       w->viewCursorIf->setCursorPositionReal(line, col);
       if (w->selectionIf) 
        w->selectionIf->removeSelectedText();
     }
     w->insertTag( text );
  } else
  if ( scriptOutputDest == "replace" )
  {
    if ( firstOutput )
       w->editIf->clear();
    w->insertTag( text );
  } else
  if ( scriptOutputDest == "new" )
  {
    if (firstOutput)
    {
        emit createNewFile();
        w = ViewManager::ref()->activeDocument();
    }
    w->insertTag( text );
  } else
  if ( scriptOutputDest == "message" )
  {
    if ( firstOutput )
    {
      emit showMessagesView();
      emit showMessage(i18n("The \"%1\" script output:\n").arg(actionText()), false);
    }
    emit showMessage(text, true);
  } else
  if ( scriptOutputDest == "file" && m_file)
  {
    if (!m_file->isOpen())
       m_file->open(IO_ReadWrite);
    m_file->writeBlock(buffer, buflen);
  }

  firstOutput = false;
}

void TagAction::slotGetScriptError( KProcess *, char *buffer, int buflen )
{
  Document *w = ViewManager::ref()->activeDocument();
  QCString tmp( buffer, buflen + 1 );
  QString text( QString::fromLocal8Bit(tmp) );

  if ( scriptErrorDest == "merge" )
  {
    scriptErrorDest = scriptOutputDest;
    firstError = firstOutput;
  }
  if ( scriptErrorDest == "cursor" )
     w->insertTag( text );
  else
  if ( scriptErrorDest == "selection" )
  {
     if ( firstError )
     {
       int line = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndLine();
       int col = dynamic_cast<KTextEditor::SelectionInterfaceExt*>(w->doc())->selEndCol();
       w->viewCursorIf->setCursorPositionReal(line, col);
       if (w->selectionIf)
        w->selectionIf->removeSelectedText();
     }
     w->insertTag( text );
  } else
  if ( scriptErrorDest == "replace" )
  {
    if ( firstError )
       w->editIf->clear();
    w->insertTag( text );
  } else
  if ( scriptErrorDest == "new" )
  {
    if (firstError)
    {
      emit createNewFile();
      w = ViewManager::ref()->activeDocument();
    }
    w->insertTag( text );
  } else
  if ( scriptErrorDest == "message" )
  {
    if ( firstError )
    {
      emit showMessagesView();
      emit showMessage(i18n("The \"%1\" script output:\n").arg(actionText()), false);
    }
    emit showMessage(text, true);
  }

  firstError = false;
}

void TagAction::scriptDone()
{
    delete proc;
    proc = 0;
}

void TagAction::setOutputFile(QFile* file)
{
  m_file = file;
}

void TagAction::setInputFileName(const QString& fileName)
{
  m_inputFileName = fileName;
}

QString TagAction::actionText()
{
   QString t = tag.attribute("text");
   int pos = t.find('&');
   if (pos < (int)t.length()-1 && t[pos+1] != '&')
     return t.remove(pos, 1);
   else
     return t;
}

QString TagAction::XMLTagName() const
{
    if(tag.attribute("type","").lower() != "tag")
        return QString();
    
    QDomElement otag = (tag.namedItem("tag")).toElement();
    QDomElement xtag = (tag.namedItem("xtag")).toElement();

    QString attr = otag.text();
    if ( attr[0] == '<' )
        attr.remove(0,1);
    if ( attr.right(1) == ">" )
        attr.remove( attr.length()-1, 1 );
    attr = attr.stripWhiteSpace();
    int i = 0;
    while ( !attr[i].isSpace() && !attr[i].isNull() )  
        ++i;
    QString name = attr.left(i);

    return name;    
}

QString TagAction::openXMLTagString() const
{
    QString name = XMLTagName();
    
    QDomElement otag = (tag.namedItem("tag")).toElement();
    QDomElement xtag = (tag.namedItem("xtag")).toElement();
    
    QString attr = otag.text();
    if ( attr[0] == '<' )
        attr.remove(0,1);
    if ( attr.right(1) == ">" )
        attr.remove( attr.length()-1, 1 );
    attr = attr.stripWhiteSpace();
    attr.remove(0, name.length());
    
    QString s1 = QuantaCommon::tagCase(name);
    if (otag.text().left(1) == "<") s1 = "<"+s1;
    if (!attr.isEmpty())
        s1 += " "+QuantaCommon::attrCase(attr);
    if (otag.text().right(1) == ">")
    {
        Document* w = ViewManager::ref()->activeView()->document();
        QTag *dtdTag = QuantaCommon::tagFromDTD(w->defaultDTD(), name);
        if ( w->defaultDTD()->singleTagStyle == "xml" && dtdTag &&
             (dtdTag->isSingle() || (!qConfig.closeOptionalTags && dtdTag->isOptional()))
           )
        {
            s1.append(" /");
        }

        s1.append(">");
    }
    
    return s1;
}

QString TagAction::closeXMLTagString() const
{
    QString s2;
    QDomElement xtag = (tag.namedItem("xtag")).toElement();
    if ( xtag.attribute("use","false") == "true" )
    {
        if (qConfig.closeTags)
            s2 = QuantaCommon::tagCase(xtag.text());
    }
    return s2;
}

void TagAction::slotActivated()
{
//     if(m_toggle)
    KToggleAction::slotActivated();
//Andras: Disable toggle behavior. It is just too broken.
    setChecked(false);
/*
    if(!m_toggle)
        setChecked(!isChecked());
*/
}

void TagAction::slotProcessExited(KProcess *process)
{
  if (loopStarted)
  {
    qApp->exit_loop();
    loopStarted = false;
  }
  emit showMessage(i18n("The \"%1\" script has exited.").arg(actionText()), false);
  delete process;
}

void TagAction::addArguments(const QStringList &arguments)
{
  m_argsList = arguments;
}

void TagAction::execute(bool blocking)
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
        timer->start(180*1000, true);
        QExtFileInfo internalFileInfo;
        loopStarted = true;
        m_killCount = 0;
        internalFileInfo.enter_loop();
        delete timer;
        m_useInputFile = false;
        m_useOutputFile = false;
    }
  } else
    slotActionActivated();
}

/** Timeout occurred while waiting for some network function to return. */
void TagAction::slotTimeout()
{
  if ((m_killCount == 0) && (KMessageBox::questionYesNo(m_parentMainWindow, i18n("<qt>The filtering action <b>%1</b> seems to be locked.<br>Do you want to terminate it?</qt>").arg(actionText()), i18n("Action Not Responding"), i18n("Terminate"), i18n("Keep Running")) == KMessageBox::Yes))
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
    if (loopStarted)
    {
        qApp->exit_loop();
        loopStarted = false;
    }
    return;
  }
  timer->start(180*1000, true);
}

void TagAction::applyTagInSelection(Node* start_node, int start_offset, Node* end_node, int end_offset, 
                                    NodeSelection& selection, NodeModifsSet* modifs) const
{
    QuantaView *view = ViewManager::ref()->activeView();    
    Document* w = view->document();
        
    Q_ASSERT(view->hadLastFocus() == QuantaView::VPLFocus);
    Q_ASSERT(toggable());
        
    QString tag_name = XMLTagName();
    Q_ASSERT(kafkaCommon::isInsideTag(start_node, end_node, tag_name) == -1);
    
    QString open_tag = openXMLTagString();
    
    //We build the node from the tag name
    Node* node = kafkaCommon::createNode("", "", Tag::XmlTag, w);
    node->tag->parse(open_tag, w);
    node->tag->name = QuantaCommon::tagCase(node->tag->name);
    node->tag->single = QuantaCommon::isSingleTag(w->defaultDTD()->name,
            node->tag->name);

    long cursor_offset = selection.cursorOffset();
    
    Node* nodeCursor = start_node;        
    Node* nodeParent = start_node;
    if (nodeParent->tag->type == Tag::Text)
        nodeParent = nodeParent->parent;

    //Checking if at least one parent of node can have a Text Node as child, otherwise
    //it is impossible for the
    //user to add this node. In that case, try to insert the Node in the closest parent accepting it.
    //e.g. TR : a normal insertion would require to have the caret in the TABLE Node, but it is
    //impossible
    QTag* nodeQTag = QuantaCommon::tagFromDTD(w->defaultDTD(), node->tag->name);
    if (!nodeQTag) return;

    bool specialTagInsertion = false;
    QPtrList<QTag> qTagList = nodeQTag->parents();
    QTag* qTag = 0;
    for (qTag = qTagList.first(); qTag; qTag = qTagList.next())
    {
        if (qTag->isChild("#text", false))
            break;
        if (qTag == qTagList.getLast())
            specialTagInsertion = true;
    }

    bool nodeTreeModified = false;
    
    if (specialTagInsertion) // Attention: not smartTagInsertion
    {
        //let's try to insert this node in the closest parent accepting it.
        while (nodeParent)
        {
            QTag* nodeParentQTag = QuantaCommon::tagFromDTD(w->defaultDTD(), nodeParent->tag->name);
            if (nodeParentQTag && nodeParentQTag->isChild(node))
            {
                nodeCursor = kafkaCommon::createMandatoryNodeSubtree(node, w);
                start_offset = 0;
                kafkaCommon::insertNodeSubtree(node, nodeParent, 0L, 0L, modifs);
                nodeTreeModified = true;
                break;
            }
            nodeParent = nodeParent->parent;
        }
    }
    else if(!nodeQTag->isSingle())
    {
        //If some text is selected in kafka, surround the selection with the new Node.
        if(!start_node|| !end_node)
            return;
        nodeTreeModified = kafkaCommon::DTDinsertRemoveNode(node, start_node, start_offset,
                end_node, end_offset, w, &nodeCursor, cursor_offset, modifs);
    }

//     w->docUndoRedo->addNewModifsSet(modifs, undoRedo::NodeTreeModif);
    
    // FIXME Set the cursor right: the selection can be inverted
    if(KafkaDocument::ref()->getKafkaWidget()->hasSelection())
    {
        nodeCursor = end_node;
        cursor_offset = end_node->tag->tagStr().length();
    }
    
    selection.setCursorNode(nodeCursor);
    selection.setCursorOffset(cursor_offset);
    
    //Now update the VPL cursor position
//     KafkaDocument::ref()->getKafkaWidget()->setCurrentNode(nodeCursor, cursor_offset);
    
    if (!nodeTreeModified)
        quantaApp->slotStatusMsg(i18n("Cannot insert the tag: invalid location."));
}

void TagAction::applyTagInMixedSelection(Node* start_node, int start_offset, Node* end_node, int end_offset, 
                                         NodeSelection& selection, NodeModifsSet* modifs) const
{
    Q_ASSERT(start_node != end_node);

    QString const tag_name = XMLTagName();
    
    // FIXME o pai pode ser do endNode. nao sei se esta merda eh precisa
/*    Node* tag_parent = kafkaCommon::hasParent(start_node, tag_name);
    Q_ASSERT(tag_parent);*/
    
    QuantaView *view = ViewManager::ref()->activeView();    
    Document* w = view->document();
    
    // Set start and end nodes to the correct node
    start_node = kafkaCommon::getCorrectStartNode(start_node, start_offset);
    end_node = kafkaCommon::getCorrectEndNode(end_node, end_offset);
    
    // look for commonParent
    QValueList<int> commonParentStartChildLocation;
    QValueList<int> commonParentEndChildLocation;

    Node* commonParent = kafkaCommon::DTDGetCommonParent(start_node, end_node, commonParentStartChildLocation, commonParentEndChildLocation, 0);
    if(!commonParent) return;

    Node* cursor_node = selection.cursorNode();
    long cursor_offset = selection.cursorOffset();
    kafkaCommon::splitStartAndEndNodeSubtree(start_node, start_offset, end_node, end_offset, commonParent, 
                                             commonParentStartChildLocation, commonParentEndChildLocation,
                                             selection, 0, modifs);

    Q_ASSERT(start_node != end_node);
    
//     kafkaCommon::coutTree(baseNode, 3);

    //We build the node from the tag name
    QString const open_tag_string = openXMLTagString();    
    Node* new_node = kafkaCommon::createNode("", "", Tag::XmlTag, w);
    new_node->tag->parse(open_tag_string, w);
    new_node->tag->name = QuantaCommon::tagCase(new_node->tag->name);
    new_node->tag->single = QuantaCommon::isSingleTag(w->defaultDTD()->name,
            new_node->tag->name);
    
    Q_ASSERT(new_node->tag->type == Tag::XmlTag);

    Node* commonParentStartChild = kafkaCommon::getNodeFromLocation(commonParentStartChildLocation, commonParent);
    Node* commonParentEndChild = kafkaCommon::getNodeFromLocation(commonParentEndChildLocation, commonParent);
//     if(!commonParentStartChild)
    commonParentStartChild = kafkaCommon::getCommonParentChild(start_node, commonParent);    
/*    if(!commonParentEndChild)
    commonParentEndChild = kafkaCommon::getCommonParentChild(end_node, commonParent);*/
    
    // insert the node, child of commonParent and commonParentStartChild as nextSibling
    kafkaCommon::insertNode(new_node, commonParent, commonParentStartChild, commonParentStartChild, modifs);
    
    // move commonParentStartChild and commonParentEndChild inside new_node
    kafkaCommon::moveNode(commonParentStartChild, new_node, 0, selection, modifs, true, true);        
    if(commonParentEndChild)
        kafkaCommon::moveNode(commonParentEndChild, new_node, 0, selection, modifs, true, true);
    
    // FIXME Set the cursor right: the selection can be inverted
    if(KafkaDocument::ref()->getKafkaWidget()->hasSelection())
    {
        /*Node* */cursor_node = end_node;
        /*int */cursor_offset = end_node->tag->tagStr().length();
                selection.setCursorNode(cursor_node);
                selection.setCursorOffset(cursor_offset);
    }
    cursor_node = selection.cursorNode();
    cursor_offset = selection.cursorOffset();
        
    Q_ASSERT(new_node->getClosingNode());

    // FIXME remove possible equal tags inside the main surrounding tag    
    kafkaCommon::mergeInlineNode(new_node, new_node->getClosingNode(), &cursor_node, cursor_offset, modifs);
    selection.setCursorNode(cursor_node);
    selection.setCursorOffset(cursor_offset);

    //Now update the VPL cursor position
//     KafkaDocument::ref()->getKafkaWidget()->setCurrentNode(cursor_node, cursor_offset);
}

void TagAction::deapplyTagInSelection(Node* start_node, int start_offset, Node* end_node, int end_offset, 
                                      NodeSelection& selection, NodeModifsSet* modifs) const
{
//     QuantaView *view = ViewManager::ref()->activeView();    
//     Document* w = view->document();

    QString const tag_name = XMLTagName();
    
    // Set start and end nodes to the correct node
    start_node = kafkaCommon::getCorrectStartNode(start_node, start_offset);
    end_node = kafkaCommon::getCorrectEndNode(end_node, end_offset);
    
    // look for commonParent
    QValueList<int> commonParentStartChildLocation;
    QValueList<int> commonParentEndChildLocation;

    Node* commonParent = kafkaCommon::DTDGetCommonParent(start_node, end_node, commonParentStartChildLocation, commonParentEndChildLocation, 0);
    if(!commonParent) return;

/*    Node* cursor_node = selection.cursorNode();
    int cursor_offset = selection.cursorOffset();*/
    kafkaCommon::splitStartAndEndNodeSubtree(start_node, start_offset, end_node, end_offset, commonParent, 
                                             commonParentStartChildLocation, commonParentEndChildLocation,
                                             selection, /*cursor_node, cursor_offset, */0, modifs); 
    
//     kafkaCommon::coutTree(baseNode, 3);
    
    Node* tag_parent = kafkaCommon::hasParent(start_node, end_node, tag_name);
    Q_ASSERT(tag_parent);
    
    QString attribute_name(tag.attribute("attribute_name", QString()));
    QString attribute_value(tag.attribute("attribute_value", QString()));
    
    if(!attribute_name.isEmpty() && !attribute_value.isEmpty())
    {
        kafkaCommon::editNodeAttribute(tag_parent, attribute_name, QString(), modifs);
    }
    
    else
    {
        Node* common_parent_start_child = kafkaCommon::getCommonParentChild(start_node, tag_parent);
        Node* common_parent_end_child = kafkaCommon::getCommonParentChild(end_node, tag_parent);
    
        Node* parent_of_tag_parent = tag_parent->parent;
        if(common_parent_end_child == common_parent_start_child)
            common_parent_end_child = 0;
        if(!common_parent_start_child)
            common_parent_start_child = kafkaCommon::getCommonParentChild(start_node, commonParent);
        kafkaCommon::moveNode(common_parent_start_child, parent_of_tag_parent, tag_parent, selection, modifs, true, true);
    
        if(common_parent_end_child)
            kafkaCommon::moveNode(common_parent_end_child, parent_of_tag_parent, tag_parent, selection, modifs, true, true);
    
    // Remove tag_parent node subtree if empty
        if(!tag_parent->hasChildNodes())
            kafkaCommon::extractAndDeleteNode(tag_parent, modifs);
    
    // FIXME Set the cursor right: the selection can be inverted
        if(KafkaDocument::ref()->getKafkaWidget()->hasSelection())
        {
            Node* cursor_node = end_node;
            int cursor_offset = end_node->tag->tagStr().length();
            selection.setCursorNode(cursor_node);
            selection.setCursorOffset(cursor_offset);
        }                
    //Now update the VPL cursor position
//     KafkaDocument::ref()->getKafkaWidget()->setCurrentNode(cursor_node, cursor_offset);
    }
}

// void TagAction::deapplyTagInMixedSelection(Node* start_node, int start_offset, Node* end_node, int end_offset, NodeModifsSet* modifs) const
// {
//     
// }


#include "tagaction.moc"
#include "myprocess.moc"
