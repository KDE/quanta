/**
   File : qxsldbgdoc.h
   Author : Keith Isdale
   Date : 19th April 2002
   Description : The document to handle source and breakpoints
 */


#ifndef QXSLDBGDOC_H
#define QXSLDBGDOC_H

#include <kio/job.h>
#include <qptrlist.h>
#include <qstring.h>

class QXsldbgTextLine {
  public:
  enum BreakPointState {
    enabled,
    disabled,
    empty
  };

  QXsldbgTextLine(const QString & text);

  BreakPointState getBreakpointState();

  const QString & getText();
  /* */
  void enableBreakPoint(bool state);
  void deleteBreakPoint();
  void selectBreakPoint(bool state);
  bool isSelected();



 private:
  /* The text for this line */
  QString text;
  /* The state of break point as reported by xsldbg */
  BreakPointState breakPointState;

  /* xsldbg is at this location */
  bool selected;
};


class QXsldbgDoc : public QObject {

Q_OBJECT

 public:
  QXsldbgDoc();
  ~QXsldbgDoc();

  /** load file into document if it can be found */
  bool load(QString fileName);

  /**
   *  get our text structure at @p linenumber
   * @returns Non-null if breakpoint has been found,
   *          NULL otherwise
  */
  QXsldbgTextLine* getText(uint lineNumber);

  /*  */
  int lineCount() {return textLines.count();};

  /**
   * getFileName
   *
   * @returns the name of file for this document
   */
  QString getFileName();

signals:
    void docChanged();

private slots:
    void slotDataArrived( KIO::Job *job, const QByteArray& data);
    void slotResult( KIO::Job *job );

 private:
  QString fileName, fileContents;
  QPtrList<QXsldbgTextLine> textLines;
};

#endif
