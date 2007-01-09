/***************************************************************************
                           styleeditor  -  implementation
    begin              : Wed Apr 7 2004
    copyright          : (C) 2004 by Luciano Gulmini <gulmini.luciano@student.unife.it>
 ***************************************************************************/

/***************************************************************************
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; version 2 of the License.
 *
 ***************************************************************************/

//qt includes
#include <qlineedit.h>
#include <qtooltip.h>
#include <qiconset.h>
#include <qlabel.h>
#include <qfileinfo.h>

//kde includes
#include <kdialog.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kurl.h>
#include <ktexteditor/editinterface.h>
#include <ktexteditor/viewcursorinterface.h>

//app includes
#include "parser.h"
#include "tag.h"
#include "node.h"
#include "project.h"
#include "resource.h"
#include "document.h"
#include "styleeditor.h"
#include "viewmanager.h"
#include "csseditor.h"

StyleEditor::StyleEditor(QWidget *parent, const char* name) : TLPEditor(parent,name){
  connect(m_pb, SIGNAL(clicked()), this, SLOT(openCSSEditor()));
  setToolTip(i18n("Open css dialog"));
  QIconSet iconSet =  SmallIconSet(QString::fromLatin1("stylesheet"));
  QPixmap pixMap = iconSet.pixmap( QIconSet::Small, QIconSet::Normal );
  m_pb->setIconSet(iconSet);
  m_iconWidth = pixMap.width();
  m_iconHeight = pixMap.height(); 
  m_pb->setFixedSize( m_iconWidth+8, m_iconHeight+8 );
  m_label->hide();
}

void StyleEditor::setButtonIcon(int width, int height){
  m_pb->setFixedSize( m_iconWidth+width, m_iconHeight+height );
}

void StyleEditor::openCSSEditor(){
  Document *w = ViewManager::ref()->activeDocument();
  if (!w) return;
  uint line, col;
  int bLine, bCol, eLine, eCol;
  bLine = bCol = eLine = eCol = 0;
  w->viewCursorIf->cursorPositionReal(&line, &col);
  if (line == 0 && col == 0)
    col++;
  //parser->rebuild(w);
  Node *node = parser->nodeAt(line, col, false);
  unsigned int lastLine = w->editIf->numLines() - 1;
  unsigned int lastCol = w->editIf->lineLength(lastLine);
  Node *styleNode = node;

  if (styleNode->tag->type == Tag::XmlTagEnd && styleNode->prev)
    styleNode = styleNode->prev;

  QString fullDocument = w->editIf->text().stripWhiteSpace();

  if (styleNode && (styleNode->tag->type == Tag::XmlTag || styleNode->tag->type == Tag::Empty) ) {
    CSSEditor *dlg = new CSSEditor(this);
    QFileInfo fi(ViewManager::ref()->currentURL());
    dlg->setFileToPreview(Project::ref()->projectBaseURL().path() +  fi.baseName(),false);

    styleNode->tag->beginPos(bLine, bCol);
    styleNode->tag->endPos(eLine, eCol);
    dlg->setFooter(">" + w->text(eLine, eCol + 1, lastLine, lastCol));

    QString temp;
    if (styleNode->tag->hasAttribute("style")) {
      dlg->setInlineStyleContent(styleNode->tag->attributeValue("style"));
      Tag tempTag(*(styleNode->tag));
      tempTag.deleteAttribute("style");
      temp = tempTag.toString();
    }
    else {
      dlg->setInlineStyleContent(QString::null);
      temp = styleNode->tag->toString();
    }

    temp = temp.left(temp.length()-1);//remove >
    temp = temp.right(temp.length()-1);//remove <
    dlg->setHeader(w->text(0, 0, bLine, bCol) + temp);

    dlg->initialize();
    if( dlg->exec() ) m_le->setText(dlg->generateProperties());
    delete dlg;
  }
}

#include "styleeditor.moc"
