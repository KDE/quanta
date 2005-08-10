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
    void writeAnnotations(const QString &fileName, const QMap<uint, QString> &annotations); 

public slots:
    /**
     * Scans the project files to see if there were annotations added or modified outside of Quanta. The scan is done in background.
     */
    void updateAnnotations();
    void readAnnotations(); 
    void clearAnnotations();
    void insertAnnotation(uint line, const QString& fileName, const QString& text);

private slots:
    void tabChanged(QWidget *w);
    void itemExecuted(QListViewItem *item);
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
    QTimer *m_updateTimer;
    KURL::List m_files;
    uint m_fileIndex;
};

#endif
