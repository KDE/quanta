/***************************************************************************
    begin                : Mon Jun 27 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
//kde includes
#include <kdebug.h>

#include <ktexteditor/codecompletioninterface.h> //right now leave it as first, otherwise it does not compile
#include <ktexteditor/document.h>
#include <ktexteditor/view.h>

#include <idocument.h>

#include "quantadoc.h"
// TODO PORT
//#include "completionbase.h"
//#include "specialareagroupcompletion.h"
#include "dtds.h"
#include "editorsource.h"
#include "groupstorage.h"
#include "parsermanager.h"
#include "qtag.h"
#include "tagpair.h"
#include "quantacommon.h"
#include "quantacorepart.h"
#include "settings.h"

#include "parserstatus.h"
#include "quantaxmlinputsource.h"
#include "dombuilder.h"

//qt includes
#include <QDir>
#include <QTextCodec>
#include <QTimer>
#include <qpoint.h>
#include <QList>



QuantaDoc::QuantaDoc(KDevelop::IDocument *document, QuantaCorePart *qcore)
  : EditorSource(document, qcore), m_qcore(qcore)
{
  m_parseResult.baseNode = 0L;
  m_parseResult.dtepList.clear();
  m_parseResult.groupStorage = new GroupStorage();

  // set the default baseURL to the path of the document, the parse may overwrite this
  KUrl baseURL = document->url();
  baseURL.setFileName("");
  m_parseResult.baseURL = baseURL;

//   m_typingInProgress = false;
  m_parserEnabled = true;
  m_parsingNeeded = true;
  m_completionDTD = 0L;
  m_codeCompletionIf = dynamic_cast<KTextEditor::CodeCompletionInterface *>(m_view);
  m_completion = 0L;
  m_completionInProgress = false;
//FIXME  m_encoding = Settings::self()->defaultEncoding();
  m_encoding = m_document->encoding();
  if (m_encoding.isEmpty())
    m_encoding = "utf8";
  m_codec = QTextCodec::codecForName(m_encoding.toUtf8());

  AreaStruct area = parseForDTD(); //find the main DTD. It inits m_dtd as well.
  kDebug(24000) << "DTD area: " << area.start.x() << ", " << area.start.y() << " | " << area.end.x() << ", " << area.end.y() << endl;

//initial parsing
  QuantaXmlInputSource *inputSource = new QuantaXmlInputSource(document->textDocument());
  ParserStatus *parser = new ParserStatus(inputSource->newLocator(), ParserManager::self()->xmlStateMachine());
  DomBuilder *builder = new DomBuilder();
  parser->setContentHandler(builder);
  parser->setLexicalHandler(builder);
  parser->setErrorHandler(builder);
  parser->parse(inputSource);
  //parse();
  m_parsingNeeded = false;
//  Node::coutTree(m_parseResult.baseNode, 2);
  for (int i = 0; i < m_parseResult.dtepList.count(); i++)
  {
    kDebug(24000) << "DTEP: " << m_parseResult.dtepList[i]->name << endl;
  }

  m_idleTimer = new QTimer(this);
  m_idleTimer->setSingleShot(true);
  connect(m_idleTimer, SIGNAL(timeout()), SLOT(slotIdleTimerExpired()));


  connect(m_document, SIGNAL(textChanged(KTextEditor::Document*)), SLOT(slotTextChanged(KTextEditor::Document*)));
  connect(m_view, SIGNAL(cursorPositionChanged(KTextEditor::View *, const KTextEditor::Cursor &)), this, SLOT(slotCursorPositionChanged(KTextEditor::View *, const KTextEditor::Cursor &)));
  connect(m_view, SIGNAL(textInserted(KTextEditor::View *, const KTextEditor::Cursor &, const QString &)), this, SLOT(slotCharactersInserted(KTextEditor::View *, const KTextEditor::Cursor &, const QString &)));
  connect(m_view, SIGNAL(filterInsertString(KTextEditor::CompletionItem*, QString *)), this, SLOT(slotFilterCompletion(KTextEditor::CompletionItem*, QString *)));
  connect(m_view, SIGNAL(completionDone(KTextEditor::CompletionItem)), this, SLOT(slotCompletionDone(KTextEditor::CompletionItem)));
  connect(m_view, SIGNAL(completionAborted()), this, SLOT( slotCompletionAborted()));

}


QuantaDoc::~QuantaDoc()
{
  delete m_parseResult.groupStorage;
  m_parseResult.groupStorage = 0L;
  delete m_idleTimer;
}

AreaStruct QuantaDoc::parseForDTD()
{
 AreaStruct area;
 //Do some magic to find the document type
 int endLine = m_document->lines();
 QString foundText = "";
 int pos = 0;
 int i = 0;
 int line, startPos;
 QString text;
 do
 {
   text = m_document->line(i);
   //search for !DOCTYPE tags
   pos = text.indexOf("!doctype", 0, Qt::CaseInsensitive);
   if (pos != -1) //parse the found !DOCTYPE tag
   {
     int bl, bc, el, ec;
     line = i;
     bl = line;
     startPos = text.lastIndexOf('<', pos);
     while (startPos == -1 && line >= 0)
     {
       text = m_document->line(line);
       startPos = text.lastIndexOf('<');
       bl = line;
       line--;
     }
     if (startPos == -1)
     {
        i++;
        continue;
     }
     bc = startPos;
     line = i;
     text = m_document->line(i);
     startPos = text.indexOf('>', pos);
     el = line;
     while (startPos == -1 && line < endLine)
     {
       text = m_document->line(line);
       startPos = text.indexOf('>');
       el = line;
       line++;
     }
     if (startPos == -1)
     {
        i++;
        continue;
     }
     ec = startPos + 1;
     Tag *tag = new Tag();
     tag->setTagPosition(bl, bc, el, ec);
     text = m_document->text(KTextEditor::Range(bl, bc, el, ec));
     tag->parse(text);
     tag->setType(Tag::XmlTag);
     text.replace("\\\"", "\"");
     pos = text.indexOf("public", 0, Qt::CaseInsensitive);
     if (pos == -1) //if no PUBLIC info, use the word after !DOCTYPE as the doc.type
     {
       foundText = tag->attribute(0);
     } else
     {             //use the quoted string after PUBLIC as doc. type
       pos = text.indexOf("\"", pos+1);
       if (pos !=-1)
       {
         int endPos = text.indexOf("\"", pos+1);
         foundText = text.mid(pos+1, endPos-pos-1);
       }
     }
     area.set(bl, bc, el, ec);
     break;
   }
   i++;
 } while (i < endLine);

 QString dtdName =  foundText.toLower();

 kDebug(24000) << "defaultDTD: dtdName = " << dtdName << endl;
 m_dtd = DTDs::ref()->find(dtdName);
//FIXME  if (!dtd) dtd = DTDs::ref()->find(Project::ref()->defaultDTD());
 if (!m_dtd)
  m_dtd = DTDs::ref()->find(Settings::self()->defaultDTEP()); //this will always exists
 return area;
}

const DTDStruct* QuantaDoc::dtdAt(int line, int col) const
{
  const DTDStruct *dtd = m_dtd;
  Node *node = Node::nodeAt(m_parseResult.baseNode, line, col, false, true);
  if (node)
  {
    dtd = node->tag->dtd();
  }

  return dtd;
}

void QuantaDoc::slotTextChanged(KTextEditor::Document* document)
{
  Q_UNUSED(document);
  if (m_parserEnabled)
  {
//    kDebug(24000) << "Delayed text changed called." << endl;
    //delay the handling, otherwise we may get wrong values for (line,column)
    QTimer::singleShot(0, this, SLOT(slotDelayedTextChanged()));
  }
}

void QuantaDoc::slotDelayedTextChanged(bool forced)
{
  kDebug(24000) << "slotDelayedTextChanged" << endl;
   if (!forced && m_idleTimer->isActive())
   {
   //  kDebug(24000) << "Reparsing delayed!" << endl;
     m_parsingNeeded = true;
     QTimer::singleShot(1000, this, SLOT(slotDelayedTextChanged()));
     m_parserEnabled = false;
     return;
   }

    KTextEditor::Cursor cursor;
    QString oldNodeName = "";
    Node *node;
    Node *currentNode = 0L; //holds a copy of the node which is at (line,column)
    Node *previousNode = 0L;//holds a copy of the node before currentNode
    if (Settings::self()->updateClosingTags())
    {
      cursor = m_view->cursorPosition();
      node = Node::nodeAt(m_parseResult.baseNode, cursor.line(), cursor.column(), false);
      if (node &&
          (node->tag->isType(Tag::XmlTag) || node->tag->isType(Tag::XmlTagEnd)) )
      {
        Tag *tag;
        tag = new Tag(*node->tag);
        currentNode = new Node(0L);
        currentNode->removeAll = false;
        currentNode->tag = tag;

        node = node->previousSibling();
        if (node)
        {
          tag = new Tag(*node->tag);
          previousNode = new Node(0L);
          previousNode->removeAll = false;
          previousNode->tag = tag;
        }
      }
    }

    ParserManager::self()->rebuild(this, &m_parseResult, m_dtd, true);
    if (Settings::self()->updateClosingTags() && currentNode)
    {
      cursor = m_view->cursorPosition();
      node = Node::nodeAt(m_parseResult.baseNode, cursor.line(), cursor.column(), false);
      if (node &&
          node->tag->nameAndNS(false) != currentNode->tag->nameAndNS(false) &&
          (node->tag->isType(Tag::XmlTag) || node->tag->isType(Tag::XmlTagEnd)) && node->tag->validXMLTag)
      {
        int bl, bc, bl2, bc2;
        node->tag->beginPos(bl, bc);
        currentNode->tag->beginPos(bl2,bc2);
        if ( (bl != bl2 || bc !=bc2) && previousNode)
        {
          previousNode->tag->beginPos(bl2, bc2);
          currentNode->deleteNode(0L);
          currentNode = previousNode;
          previousNode = 0L;
        } else
        {
          previousNode->deleteNode(0L);
          previousNode = 0L;
        }
        if (bl == bl2 && bc == bc2 &&
           (currentNode->tag->isType(Tag::XmlTag) || currentNode->tag->isType(Tag::XmlTagEnd)))
        {
          QString newName = node->tag->nameAndNS(false);
          bool updateClosing = (currentNode->tag->isType(Tag::XmlTag)) && !node->tag->name().startsWith("!");
          int num = 1;
          if (updateClosing)
            node = node->nextSibling();
          else
            node = node->previousSibling();
          while (node)
          {
            if (node->tag->validXMLTag && (node->tag->isType(Tag::XmlTag) || node->tag->isType(Tag::XmlTagEnd)))
            {
              if (node->tag->nameAndNS(false) == currentNode->tag->nameAndNS(false) )
              {
                num++;
              }
              if ( (updateClosing && node->tag->doesClose(currentNode->tag)) ||
                  (!updateClosing && currentNode->tag->doesClose(node->tag)) )
              {
                num--;
              }
              if (num == 0)
              {
                m_parserEnabled = false;
                node->tag->beginPos(bl, bc);
                bc++;
                m_document->startEditing();
                int len = node->tag->nameAndNS().length();
                m_document->removeText(KTextEditor::Range(bl, bc, bl, bc + len));
                if (updateClosing)
                {
                  m_document->insertText(KTextEditor::Cursor(bl, bc), '/' + newName);
                } else
                {
                  m_document->insertText(KTextEditor::Cursor(bl, bc), newName);
                  if (bl == cursor.line())
                  {
                    cursor.setColumn(cursor.column() + (newName.length() - currentNode->tag->name().length()) + 1);
                  }
                }
                m_document->endEditing();
                m_view->setCursorPosition(KTextEditor::Cursor(bl, bc));
                //FIXME: docUndoRedo->mergeNextModifsSet();
                parse();
                m_view->setCursorPosition(cursor);
                m_parserEnabled = true;
                break;
              }
            }
            if (updateClosing)
              node = node->nextSibling();
            else
              node = node->previousSibling();
          }
          currentNode->deleteNode(0L);
          currentNode = 0L;
        }
      }
      currentNode->deleteNode(0L);
      previousNode->deleteNode(0L);
    }
  m_parserEnabled = true;
  m_parsingNeeded = false;
}

void QuantaDoc::slotCursorPositionChanged(KTextEditor::View *view, const KTextEditor::Cursor &newPosition)
{
  Q_UNUSED(view);
  Q_UNUSED(newPosition);
  startIdleTimer();
}

void QuantaDoc::startIdleTimer()
{
  m_idleTimer->start(500);
}


#if 0 // TODO PORT
void QuantaDoc::slotFilterCompletion(KTextEditor::CompletionItem *completion, QString *string)
{
  m_completion->filterCompletion(completion, string);
}

void QuantaDoc::slotCompletionDone(KTextEditor::CompletionItem completion)
{
  m_completionInProgress = false;
  switch (m_completion->completionDone(completion))
  {
    case CompletionBase::RequestCompletion:
      {
        //must be delayed, otherwise katepart does not show the new completion list
        QTimer::singleShot(0, this, SLOT(slotDelayedManualCodeCompletion()));
        break;
      }
    case CompletionBase::RequestHint:
      {
        //must be delayed, otherwise katepart does not show the new completion list
        QTimer::singleShot(0, this, SLOT(slotDelayedCodeCompletionHint()));
        break;
      }
    case CompletionBase::RequestNone:
      break;
  }
//   m_completion = 0L;

}
#endif

void QuantaDoc::slotCompletionAborted()
{
  kDebug(24000) << "Completion aborted" << endl;
//   m_completion = 0L;
  m_completionInProgress = false;
}

void QuantaDoc::slotDelayedCodeCompletionHint()
{
  codeCompletionHintRequested();
}

void QuantaDoc::slotDelayedShowCodeCompletion()
{
  kDebug(24000) << "slotDelayedShowCodeCompletion" << endl;
#if 0 // TODO PORT
  showCodeCompletions(m_completionEntries);
#endif
}

void QuantaDoc::slotDelayedManualCodeCompletion()
{
  codeCompletionRequested();
}

void QuantaDoc::slotDelayedReplaceChar()
{
  m_parserEnabled = false;
  m_document->removeText(KTextEditor::Range(m_replaceCharsPosition.x(), m_replaceCharsPosition.y(), m_replaceCharsPosition.x(), m_replaceCharsPosition.y()+ 1));
  insertText(m_replaceCharsString, true);
}

void QuantaDoc::replaceAccentedChars(const QPoint &position, const QChar& ch)
{

 if (Settings::self()->replaceAccented())
 {
    QString encoding = m_document->encoding();
    if (encoding.isEmpty())
      encoding = "utf8";
    if (encoding != m_encoding)
    {
      m_encoding = encoding;
      m_codec = QTextCodec::codecForName(encoding.toUtf8());
    }
    if (!m_codec->canEncode(ch))
    {
      m_replaceCharsPosition = position;
      m_replaceCharsString = QString("&#%1;").arg(ch.unicode());
      QTimer::singleShot(0, this, SLOT(slotDelayedReplaceChar()));
      return;
    }
  }
}


void QuantaDoc::slotCharactersInserted(KTextEditor::View *view, const KTextEditor::Cursor &cursor, const QString &string)
{
  Q_UNUSED(view);
  int line = cursor.line();
  int column = cursor.column();
  kDebug(24000) << "Inserted char: |" << string <<"|" << endl;
  QPoint position(line, column);
  replaceAccentedChars(position, string[0]);

  //Typing this characters indicates that the context may change
  //from Xml to some kind of special area, so we must rebuild
  //the node tree to know in what kind of area we are when showing
  //the autocompletion box
  if ( (string == ">") ||
        (string == "<") )
  {
    slotDelayedTextChanged(true);
  }


//TODO use this when Katepart does not emit completionAborted before completionDone
// if (!Settings::self()->enableAutocompletion() || m_completion) //completion is in progress
  if (!Settings::self()->enableAutocompletion() || m_completionInProgress)
  {
    return;
  }
  m_completion = 0L;

  Node *node = Node::nodeAt(m_parseResult.baseNode, line, column, false, true);
  if (!node)
    return; //problems, should not happen...
  const DTDStruct *dtd = node->tag->dtd();
//       kDebug(24000) << "Tested position: " << line << ", " << column << endl;
//       kDebug(24000) << "node: " << node->tag->tagStr() << " " << node->tag->type << " " << node->tag->validXMLTag << endl;
//       AreaStruct area(node->tag->area());
//       kDebug(24000) << "node area: " << area.bLine() << ", " << area.bCol() << ", " << area.eLine() << ", " << area.eCol() << endl;
  bool tempNodeCreated = false;
  if (m_parsingNeeded) //the node tree is not up to date, we need to build our custom node
  {
    AreaStruct area = findTagArea(position);
    QString areaString = text(area);

    Tag *tag = new Tag(area, areaString, dtd, true);
    if (tag->isType(Tag::Unknown))
      tag->setType(Tag::XmlTag);

    node = new Node(0L);
    node->tag = tag;
    tempNodeCreated = true;
/*    kDebug(24000) << "Tag should be between " << begin << " and " << end << endl;
    kDebug(24000) << "Tag text:" << areaString << endl;
    kDebug(24000) << "Tag:" << tag->toString() << endl;
    kDebug(24000) << "Tag type:" << tag->type << endl;*/
  }
  if (dtd->family == DTDStruct::Xml)
  {
#if 0 // TODO PORT
    if (string == "<")
    {
      m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::XmlTag, CompletionBase::Auto, m_qcore);
    } else
    if (string == " ")
    {
      m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::XmlAttribute, CompletionBase::Auto, m_qcore);
    } else
    if (string.startsWith(Settings::self()->attributeQuotationChar()))
    {
      m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::XmlAttributeValue, CompletionBase::Auto, m_qcore);
    } else
    if (string == "&")
    {
      m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::XmlEntity, CompletionBase::Auto, m_qcore);
    } else
#endif
    if (string == ">")
    {
      int index = node->tag->valueIndexAtPos(line, column);
      if (index == -1)
        closeTag(node);
    }
  } else
  if (dtd->family == DTDStruct::Script)
  {
#if 0 // TODO PORT
    if (dtd->name == "DTD" && string == " ") //special case
    {
       m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::DTDDefinition, CompletionBase::Auto, m_qcore);
    } else
    if (!node->tag->isType(Tag::Comment))
    {
      if ( string == " " || string.startsWith(dtd->attrAutoCompleteAfter) ) //if we need to list the arguments of a function
      {
        m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::SpecialAreaAttribute, CompletionBase::Auto, m_qcore);
      }
      if (!m_completion && (string == " " || string.startsWith(dtd->tagAutoCompleteAfter)
        || (!dtd->memberAutoCompleteAfter.pattern().isEmpty() && dtd->memberAutoCompleteAfter.lastIndexIn(text(line, 0, line, column)) != -1))
         )
      {
        m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::SpecialAreaTag, CompletionBase::Auto, m_qcore);
      }
      if (!m_completion)
      {
        StructTreeGroup group;
        uint count = dtd->structTreeGroups.count();
        for (uint j = 0; j < count; j++)
        {
          group = dtd->structTreeGroups[j];
          if (!group.autoCompleteAfterRx.pattern().isEmpty() &&
              ( group.autoCompleteAfterRx.indexIn(string) != -1))
          {
            m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::SpecialAreaGroups, CompletionBase::Auto, m_qcore);
            static_cast<SpecialAreaGroupCompletion*>(m_completion)->setGroup(&(dtd->structTreeGroups[j]));
            break;
          }
        }
      }
    }
    if (!m_completion)
#endif
    {
      if (Settings::self()->closeTags() && node->tag->validXMLTag &&
          node->tag->isType(Tag::ScriptTag) &&
          (!node->next || !node->next->tag->doesClose(node->tag)))
        insertText("</" + node->tag->name() + '>', false);
    }
  }
  if (m_completion)
  {
#if 0 // TODO PORT
    showCodeCompletions(m_completion->completionEntries());
#endif
  }
  if (tempNodeCreated) //a custom node was created
    node->deleteNode(0L);
}

void QuantaDoc::insertTag(const TagPair & tagPair)
{
  m_document->startEditing();

  QString selection;

  if (m_view->selection())
  {
    m_parserEnabled = false;
    selection = m_view->selectionText();
    m_view->removeSelectionText();
    m_parserEnabled = true;
  }
  insertText(tagPair.createTag(selection), false); // don't adjust cursor, thereby leaving it in the middle of tag


  m_document->endEditing();
  m_view->setFocus();
}

void QuantaDoc::insertChildTags(QTag *qtag, QTag *lastTag)
{
  if (!qtag || qtag == lastTag) //avoid infinite recursion
    return;

  QMap<QString, bool>::Iterator it;
  for (it = qtag->childTags.begin(); it != qtag->childTags.end(); ++it)
  {
    if (it.value())
    {
      QTag *childTag = QTag::tagFromDTD(qtag->parentDTD, it.key());
      QString tagStr =Tag::convertCase(it.key());
      bool single = ( qtag->parentDTD->singleTagStyle == "xml" &&
          ( childTag->isSingle() ||
          (childTag->isOptional() && !Settings::self()->closeOptionalTags())) );

      insertText(TagPair::decorateOpening(tagStr, single), true);

      insertChildTags(childTag, qtag);

      if ( (!childTag->isSingle() && !childTag->isOptional() && Settings::self()->closeTags()) ||
           (childTag->isOptional() && Settings::self()->closeOptionalTags()) )
      {
        insertText(TagPair::decorateClosing(tagStr), true);
      }
    }
  }
}

#if 0 // TODO PORT
void QuantaDoc::showCodeCompletions(QList<KTextEditor::CompletionItem> *completions)
{
  bool reparse = m_parserEnabled;
  m_parserEnabled = false;
  m_completionInProgress = true;
  //FIXME: Code completion needs to be rewritten using a CompletionProvider
//  m_codeCompletionIf->showCompletionBox(*completions, false);
  if (completions->count() == 0)
  {
//     m_completion = 0L;
    m_completionInProgress = false;
  }
  m_parserEnabled = reparse;
  delete completions;
}
#endif

void QuantaDoc::codeCompletionRequested()
{
  if (m_completionInProgress)
    return;

#if 0 // TODO PORT
  m_completion = 0L;
  slotDelayedTextChanged(true);
  KTextEditor::Cursor cursor = m_view->cursorPosition();
  int line = cursor.line();
  int column = cursor.column();

  QPoint position(line, column); //the position passed to the m_completion entry. Might not be exactly the cursor position.
  Node *node = Node::nodeAt(m_parseResult.baseNode, line, column, false, true);
  if (!node)
    return; //problems, should not happen...
  const DTDStruct *dtd = node->tag->dtd();
  if (dtd->family == DTDStruct::Xml)
  {
    //check for entity completion first
    m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::XmlEntity, CompletionBase::Manual, m_qcore);
    if (!m_completion && node->tag->isType(Tag::XmlTag))
    {
      m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::XmlTag, CompletionBase::Manual, m_qcore);
      if (!m_completion)
      {
        m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::XmlAttributeValue, CompletionBase::Manual, m_qcore);
        if (!m_completion)
        {
          m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::XmlAttribute, CompletionBase::Manual, m_qcore);
        }
      }
    }
  } else
  if (dtd->family == DTDStruct::Script)
  {
    if (dtd->name == "DTD") //special case
    {
      m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::DTDDefinition, CompletionBase::Manual, m_qcore);
    } else
    {
      m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::SpecialAreaGroups, CompletionBase::Manual, m_qcore);
      if (!m_completion)
        m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::SpecialAreaAttribute, CompletionBase::Manual, m_qcore);
      if (!m_completion)
        m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::SpecialAreaTag, CompletionBase::Manual, m_qcore);

    }
  }
  if (m_completion)
    showCodeCompletions(m_completion->completionEntries());
#endif
}

void QuantaDoc::codeCompletionHintRequested()
{
  if (m_completionInProgress)
    return;

  m_completion = 0L;
  slotDelayedTextChanged(true);
  KTextEditor::Cursor cursor = m_view->cursorPosition();
  QPoint position(cursor.line(), cursor.column()); //the position passed to the m_completion
  Node *node = Node::nodeAt(m_parseResult.baseNode, cursor.line(), cursor.column(), false, true);
  if (!node)
    return; //problems, should not happen...
  const DTDStruct *dtd = node->tag->dtd();
  if (dtd->family == DTDStruct::Script)
  {
#if 0 // TODO PORT
    m_completion = CompletionBase::getCompletionObject(&m_parseResult, this, position, node, CompletionBase::SpecialAreaAttribute, CompletionBase::Manual, m_qcore);
  }
  if (m_completion)
  {
    CompletionBase::HintResult result = m_completion->completionHint();
  //FIXME: Code completion needs to be rewritten using a CompletionProvider    //m_codeCompletionIf->showArgHint(result.argList, result.wrapper, result.delimiter);
#endif
  }

}


void QuantaDoc::parse()
{
  ParserManager::self()->parse(this, &m_parseResult, m_dtd, true);
}

bool QuantaDoc::isSameDocument(KDevelop::IDocument *document)
{
  return document == m_ideDocument;
}

void QuantaDoc::slotIdleTimerExpired()
{
  KTextEditor::Cursor cursor = m_view->cursorPosition();
  m_qcore->emitNewCursorPosition(QPoint(cursor.line(), cursor.column())); // signal in QuantaCoreIf
}


QPoint QuantaDoc::findTagBegin(const QPoint& position)
{
  int line = position.x();
  int col = position.y();
  while (line > 0 || col > 0)
  {
    getPreviousPosition(line, col);
    const QString text = m_document->line(line);
    for ( ; col >= 0; --col)
    {
      QChar c = text.at(col);
      if (c == '<')
        return QPoint(line, col);

      if (c == '>')
        return QPoint(line, ++col);
    }
  }
  return QPoint(line, col);
}

QPoint QuantaDoc::findTagEnd(const QPoint& position)
{
  int line = position.x();
  int col = position.y();
  int numLines = m_document->lines();
  while (line < numLines)
  {
    const QString text = m_document->line(line);
    int lineLength = text.length();
    for ( ; col < lineLength; ++col)
    {
      QChar c = text.at(col);
      if (c == '<')
      {
        getPreviousPosition(line, col);
        return QPoint(line, col);
      }
      if (c == '>')
      {
        return QPoint(line, col);
      }
    }
    if (line == numLines - 1)
    {
      return QPoint(line, --col);
    }
    line++;
    col = 0;
  }
  return QPoint(line, col);
}


void QuantaDoc::closeTag(Node *node)
{
  QString tagName = node->tag->name();
  QTag *qTag = QTag::tagFromDTD(node->tag->dtd(), tagName);
  if (node->tag->isClosingTag() || tagName.isEmpty())
    return;
  if (!node->tag->tagStr().endsWith("/>"))
  {
      if ( Settings::self()->closeTags() &&
           (!qTag || (!qTag->isSingle() && !qTag->isOptional())) ||
           (qTag->isOptional() &&
            Settings::self()->closeOptionalTags()) )
      {
        if (!node->next || !node->next->tag->doesClose(node->tag))
        {
          if (!node->tag->nameSpace().isEmpty())
              tagName.prepend(node->tag->nameSpace() + ':');
          insertText("</" + tagName + '>', false);
        }
      } else
      if ( qTag->parentDTD->singleTagStyle == "xml" &&
           (qTag->isSingle() || (!Settings::self()->closeOptionalTags() && qTag->isOptional()))
         )
      {
        KTextEditor::Cursor cursor = m_view->cursorPosition();
        cursor.setColumn(cursor.column() -1 );
        m_document->insertText(cursor, " /");
        cursor.setColumn(cursor.column() + 3 );
        m_view->setCursorPosition(cursor);
      }
  }
  if (qTag && !qTag->childTags.isEmpty())
  {
    m_parserEnabled = false;
    //insertText("\n", false, false);
    insertChildTags(qTag);
    m_parserEnabled = true;
    ParserManager::self()->rebuild(this, &m_parseResult, m_dtd, true);
  }
}

#include "quantadoc.moc"
