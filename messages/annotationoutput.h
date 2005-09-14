//
// C++ Interface: annotationoutput
//
// Description: 
//
//
// Author: Andras Mantia <amantia@kde.org>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ANNOTATIONOUTPUT_H
#define ANNOTATIONOUTPUT_H

#include <kurl.h>
#include <ktabwidget.h>

/**
@author Andras Mantia
*/

class QDomDocument;
class QListViewItem;
class QTimer;

class KListView;
class KListViewItem;

class MessageOutput;

enum AnnotationScope
{
  CurrentFile = 0,
  AllFiles
};

class AnnotationOutput : public KTabWidget
{
Q_OBJECT
public:
    AnnotationOutput(QWidget *parent = 0, const char *name = 0);

    ~AnnotationOutput();
    MessageOutput *currentFileAnnotations() const {return m_currentFileAnnotations;}
    KListView *allAnnotations() const {return m_allAnnotations;}
    void writeAnnotations(const QString &fileName, const QMap<uint, QPair<QString, QString> > &annotations);

public slots:
    /**
     * Scans the project files to see if there were annotations added or modified outside of Quanta. The scan is done in background.
     */
    void updateAnnotations();
    void readAnnotations(); 
    void clearAnnotations();
    void insertAnnotation(uint line, const QString& fileName, const QPair<QString, QString>& annotation);

private slots:
    void tabChanged(QWidget *w);
    void allAnnotationsItemExecuted(QListViewItem *item);
    void yourAnnotationsItemExecuted(QListViewItem *item);
    void slotUpdateNextFile();

signals:
    void clicked(const QString& fname, int line, int col);

private:
    /**
     * Updates the annotation for a file.
     * @param url 
     */
    void updateAnnotationForFile(const KURL& url);

    MessageOutput *m_currentFileAnnotations;
    KListView *m_allAnnotations;
    QDict<KListViewItem> m_annotatedFileItems;
    QMap<QListViewItem*, QString> m_fileNames;
    QMap<QListViewItem*, uint> m_lines;
    uint m_yourAnnotationsNum;

    KListView *m_yourAnnotations;
    QDict<KListViewItem> m_yourFileItems;
    QMap<QListViewItem*, QString> m_yourFileNames;
    QMap<QListViewItem*, uint> m_yourLines;

    QTimer *m_updateTimer;
    KURL::List m_files;
    uint m_fileIndex;
};

#endif
