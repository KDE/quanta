/**
   File : qxsldbgview.cpp
   Author : Keith Isdale
   Date : 19th April 2002
   Description : The view to see source and breakpoints
*/


#include <qpainter.h>
#include <qpixmap.h>
#include "qxsldbgview.h"
#include "qxsldbgdoc.h"
#include <kstandarddirs.h> 
#include <kdebug.h>
#define NDEBUG


QXsldbgView::QXsldbgView(QWidget *parent)
 :  QScrollView(parent, "QXsldbg"), markPixMap(0), hashPixMap(0)
{
  setVScrollBarMode(QScrollView::Auto);
  setHScrollBarMode(QScrollView::Auto);
  setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
  setMinimumSize(500,400);
  cursorVisible = FALSE;
  _line = 0;
  _row = 0;
  doc = 0L;

  /* Set the size of our cursor area (not mouse cursor)*/
  cursorHeight = fontMetrics().ascent();
  cursorWidth = 15; // fontMetrics().maxWidth();
  hideCursor = TRUE;

  setCursorPosition(0, 0);
  /* Flash a cursor every 1/2 of a second */
  startTimer(500);

  KStandardDirs kdeDirs;
  markPixMap = new QPixmap(kdeDirs.findResource("icon", "mark.png"));
  hashPixMap = new QPixmap(kdeDirs.findResource("icon", "hash.png"));
  /* How big is our blob to show breakpoint state of line */
  blobHeight = fontMetrics().ascent();
  blobWidth = blobHeight;
  /* Show much horizontal space between blob and the text */
  blobSpacing = 5;
  blobYoffset = blobHeight * -1;

  /* How big is the marker showing where xsldbg is stoped at */
  markerHeight = 5;
  markerWidth = 15;
  markerLineNo = 0;


  /* make sure we can get keyboard focus */
  setFocusPolicy(QWidget::StrongFocus);
  //  viewport()->setFocus(); // needed for QT2 
  viewport()->setBackgroundMode(PaletteBrightText);
}

QXsldbgView::~QXsldbgView()
{
  delete markPixMap;
  delete hashPixMap;			
}
 
void QXsldbgView::setDocument(QXsldbgDoc *doc)
{
  this->doc = doc;
  if (doc != 0L){
    QFontMetrics fm = fontMetrics();
    QXsldbgTextLine *item;
    int textHeight = fm.lineSpacing();
    uint lineWidth = 0;
    for (int index = 1; index < doc->lineCount(); index++){
      item = doc->getText(index);
      if (item != 0L){
	if (item->getText().length() > lineWidth)
	  lineWidth = item->getText().length();
      }
    } 
    resizeContents(fm.maxWidth() * lineWidth, doc->lineCount() * textHeight);
  }
#if QT_VERSION >=300
  repaintContents();
#else
  repaintContents(0, 0, contentsWidth(), contentsHeight(), TRUE);
#endif
}


void QXsldbgView::getCursorPosition(int *line, int *row)
{
  CHECK_PTR(line);
  CHECK_PTR(row);
  if ((line != 0L) && ( row != 0L)){
    *line = this->_line;
    *row = this->_row;
  }
}


void QXsldbgView::setCursorPosition(int line, int row)
{
  QXsldbgTextLine *item;
  QFontMetrics fm = fontMetrics();
  QRect mergedRect = _cursorRect;
  int textHeight = fm.lineSpacing();
  //  bool repositionContents = line != getLineNo();
  hideCursor = true;

  if (doc != 0L){
    /* compute the area that needs to be repainted */
    item = doc->getText(line);
    
    if (item != 0L){

      QSize s1;
      this->_line = line;
      this->_row = row;
      s1 = fm.size(ExpandTabs, item->getText().left(getRowNo()));

      _cursorRect.setLeft(s1.width() + blobWidth + blobSpacing); 
      _cursorRect.setTop(getLineNo() * textHeight);
      _cursorRect.setRight(_cursorRect.left() + cursorWidth);
      _cursorRect.setBottom(_cursorRect.top() + cursorHeight);
      mergedRect = mergedRect.unite(_cursorRect);
      

      if ((contentsY() + visibleHeight() < getLineNo() * textHeight) ||
	  getLineNo() * textHeight < contentsY()){
	if (getLineNo() > 11)
	  setContentsPos(contentsX(), (getLineNo() - 11) * textHeight);
	else 
	  setContentsPos(contentsX(), 0);
	
      }
      repaintContents(mergedRect.x(), mergedRect.y(), 
		      mergedRect.width(), mergedRect.height(), TRUE);

      emit cursorPositionChanged(line, row);

    }
  }

  hideCursor = false;
}

void QXsldbgView::setMarkerPosition(int line, int row)
{
  QXsldbgTextLine *item;
  if (doc != 0L){
    item = doc->getText(markerLineNo);
    if (item != 0L)
      item->selectBreakPoint(false);
    markerLineNo = line;
    item = doc->getText(line);
    if (item != 0L)
      item->selectBreakPoint(true);
    setCursorPosition(line, row);
    repaintAll();
  }  
}

void  QXsldbgView::repaintAll()
{
  repaintContents(contentsX(), contentsY(), contentsWidth(), contentsHeight());
}


void QXsldbgView::drawContents(QPainter* p, int clipx, int clipy, 
			       int clipw, int cliph)
{
  
  if (doc == 0L || ( p == 0L)){
    return;
  }

  QFontMetrics fm = fontMetrics();
  int textHeight = fm.lineSpacing();
  int py;
  QString str;
  QXsldbgTextLine *item;

  for (int index = 0 ; index < doc->lineCount(); index++){    
    py = index * textHeight;
    /* reduce the amount of text to draw and only draw text if
       we are inside area to be repainted */
    if ((py >= clipy - textHeight) && (py <= clipy + cliph + textHeight)){
      item = doc->getText(index);
      if (item != 0L){
	p->drawText(blobWidth + blobSpacing, py, 
		    contentsWidth(), textHeight, ExpandTabs, item->getText());
	/* Draw a coloured blob to show the state of a breakpoint */
	switch(item->getBreakpointState()){
	case QXsldbgTextLine::enabled:
	  {
	    QBrush brush(Qt::red, Qt::SolidPattern);
	    p->setBrush(brush);
	    p->drawEllipse(0, py + blobYoffset, blobWidth, blobHeight);
	  }
	  break;

	case QXsldbgTextLine::disabled:
	  {
	    QBrush brush(Qt::yellow, Qt::SolidPattern);
	    p->setBrush(brush);
	    p->drawEllipse(0, py + blobYoffset, blobWidth, blobHeight);
	  }
	  break;


	case QXsldbgTextLine::empty:
	  break;	  
	}

	/* Show a marker where we are up to */
	if (item->isSelected()){
	  p->fillRect(0, py + textHeight - fm.ascent()/2, 
		      markerWidth, markerHeight,  Qt::green);
	}

      }

    }
  }

  /* Flash the cursor */
  if ((hideCursor == FALSE) && (cursorVisible == TRUE)){
    p->fillRect(cursorRect().x(), cursorRect().y(), 
	      cursorRect().width(), cursorRect().height(), 
			black);
  }
}


void QXsldbgView::contentsMousePressEvent( QMouseEvent *e )
{
  QFontMetrics fm = fontMetrics();
  int textHeight=fm.lineSpacing();

  QPoint p = e->pos();

  int currentLine = p.y() / textHeight + 1;
  
  if (doc != 0L){
      QXsldbgTextLine *item;
      item = doc->getText(currentLine);
      if (item != 0L){
	switch(item->getBreakpointState()){

	case QXsldbgTextLine::enabled:
	  emit enableBreakPoint(currentLine);
	  item->enableBreakPoint(false);
	  break;
	  
	case QXsldbgTextLine::disabled:
	  emit deleteBreakPoint(currentLine);
	  item->deleteBreakPoint();
	  break;
	  
	  
	case QXsldbgTextLine::empty:
	  emit addBreakPoint(currentLine);
	 item->enableBreakPoint(true);
	  break;	  
	}

	repaintContents(0, currentLine * textHeight +  blobYoffset, 
		blobWidth, blobHeight, TRUE); 
      }
  }
}


void  QXsldbgView::timerEvent( QTimerEvent * )
{
  cursorVisible = !cursorVisible;

  /* repaint the cursor */
  repaintContents(cursorRect().x(), cursorRect().y(), 
		  cursorRect().width(), cursorRect().height(), TRUE);
}


void QXsldbgView::keyPressEvent( QKeyEvent *e ) 
{
  if (doc != 0L){
    QFontMetrics fm = fontMetrics();
    int textHeight = fm.lineSpacing();

    switch (e->key()){
    case Key_Up:
      if (getLineNo() > 0){
	if (contentsY() > (getLineNo() - 1) * textHeight){
	  setContentsPos(contentsX(), contentsY() - textHeight);
	}
	setCursorPosition(getLineNo() - 1, getRowNo());
      }
      break;

    case Key_Down:
      if (getLineNo() < doc->lineCount()){
	if ((getLineNo() + 3) * textHeight > contentsY() + visibleHeight ()){
	  setContentsPos(contentsX(), contentsY() + textHeight);
	}
	setCursorPosition(getLineNo() + 1, getRowNo());    
      }
      break;

    case Key_Left:
      if (getRowNo() > 0)
	setCursorPosition(getLineNo(), getRowNo() - 1);
      break;

    case Key_Right:
      setCursorPosition(getLineNo(), getRowNo() + 1);
      break;

    case Key_PageUp:
      if (getLineNo() > 22){
	if (contentsY() > (getLineNo() - 23) * textHeight){
	  setContentsPos(contentsX(), contentsY() - textHeight * 22);
	}
	setCursorPosition(getLineNo() - 22, getRowNo());
      }else{
	setContentsPos(contentsX(), 0);
	setCursorPosition(0, getRowNo());	
      }
      
      break;

    case Key_PageDown:
      if (getLineNo() < doc->lineCount() + 22){      
	if ((getLineNo() + 22 + 3) * textHeight > 
	    contentsY() + visibleHeight ()){
	  setContentsPos(contentsX(), contentsY() + textHeight * 22);
	}
	setCursorPosition(getLineNo() + 22, getRowNo());    
      }
      break;

    case Key_Home:
      setCursorPosition(0, getRowNo());
      break;
    }
  }
}


void QXsldbgView::focusInEvent ( QFocusEvent *e )
{
  QScrollView::focusInEvent(e);
  hideCursor = FALSE;
}

void QXsldbgView::focusOutEvent ( QFocusEvent *e)
{
  QScrollView::focusOutEvent(e);
  hideCursor = TRUE;
}

#include "qxsldbgview.moc"
