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

#include <ktabwidget.h>

/**
@author Andras Mantia
*/

class MessageOutput;

enum AnnotationScope
{
  CurrentFile = 0,
  AllFiles,
};

class AnnotationOutput : public KTabWidget
{
Q_OBJECT
public:
    AnnotationOutput(QWidget *parent = 0, const char *name = 0);

    ~AnnotationOutput();
    MessageOutput *currentFileAnnotations() const {return m_currentFileAnnotations;}
    MessageOutput *allAnnotations() const {return m_allAnnotations;}

public slots:
    void clearAnnotations();
    void insertAnnotation(uint line, uint line, const QString& text);
    void refreshAnnotations();
    void tabChanged(QWidget *w);

private:
    MessageOutput *m_currentFileAnnotations;
    MessageOutput *m_allAnnotations;
};

#endif
