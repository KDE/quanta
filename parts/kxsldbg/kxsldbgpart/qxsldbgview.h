/**
   File : qxsldbgview.h
   Author : Keith Isdale
   Date : 19th April 2002
   Description : The view to see source and breakpoints
*/

#ifndef QXSLDBGVIEW_H
#define QXSLDBGVIEW_H

#include <qscrollview.h>
#include <qrect.h>

class QCursor;
class QXsldbgDoc;
class QPixMap;

class QXsldbgView : public QScrollView {

Q_OBJECT

 public:
  QXsldbgView(QWidget *parent);
  ~QXsldbgView();
  
  void getCursorPosition(int *line, int *row);
  void setCursorPosition(int line, int row);
  void setMarkerPosition(int line, int row);
  void setDocument(QXsldbgDoc *doc);
  int getLineNo() {return _line;};
  int getRowNo() {return _row;};
  QRect cursorRect() {return _cursorRect;};
  void repaintAll();

public slots:
 void show();
 void docChanged(); 
 void resizeView();
 
 protected:
  void drawContents(QPainter* p, int clipx, int clipy, int clipw, int cliph);
  void contentsMousePressEvent( QMouseEvent *e );
  void timerEvent( QTimerEvent *e );
  void keyPressEvent( QKeyEvent *e );  
  void focusInEvent ( QFocusEvent *e);
  void focusOutEvent ( QFocusEvent *e );
 
 signals:
  void cursorPositionChanged(int para, int pos);
  void openFile(QString fileName, int lineNumber, int columnNumber);
  void addBreakPoint(int lineNumber);
  void enableBreakPoint(int lineNumber);
  void deleteBreakPoint(int lineNumber);

 private:
  QXsldbgDoc *doc;
  int _line, _row, markerLineNo;
  int cursorHeight, cursorWidth;
  QPixmap *markPixMap,*hashPixMap; 
  /* The size of the blob that shows the breakpoint state  of a line */
  int blobHeight, blobWidth, blobSpacing, blobYoffset;
  int markerHeight, markerWidth;
  bool cursorVisible, hideCursor;  
  QRect _cursorRect;
};

#endif
