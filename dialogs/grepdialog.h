/***************************************************************************
                          grepdialog.h  -  grep frontend                              
                             -------------------                                         
    copyright            : (C) 1999 by Bernd Gehrmann
    email                : bernd@physik.hu-berlin.de

    Modifications for Quanta: (c) 2000 by Richard Moore, rich@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/


#ifndef _GREPDIALOG_H_
#define _GREPDIALOG_H_

#include <qdialog.h>

class QLineEdit;
class QComboBox;
class QCheckBox;
class QListBox;
class QPushButton;
class QLabel;
class KProcess;

/**
 * A dialog that puts a front end on grep and provides recursive searches using
 * find.
 *
 * @version $Id$
 */
class GrepDialog : public QDialog
{
    Q_OBJECT

public:
    GrepDialog(const QString &dirname, QWidget *parent=0, const char *name=0);
    ~GrepDialog();
    void  setDirName(const QString &);

signals:
    void itemSelected(const QString &abs_filename, int line);
    
public slots:
    void slotSearchFor(const QString &pattern);
		
private slots:
    void dirButtonClicked();
    void childExited();
    void receivedOutput(KProcess *proc, char *buffer, int buflen);
    void itemSelected(const QString &item);
    void slotSearch();
    void slotStop();
    void slotClear();

private:
    void processOutput();
    void finish();
    
    QLineEdit *pattern_edit, *dir_edit;
    QComboBox *files_combo;
    QCheckBox *recursive_box;
    QListBox *resultbox;
    QPushButton *search_button, *stop_button;
    QLabel *status_label, *matches_label;
    KProcess *childproc;
    QString buf;
};

/**
 * Used internally by the grep results parser.
 *
 * @internal
 */
class StringTokenizer
{
public:
    StringTokenizer();
    ~StringTokenizer();

    void tokenize( const char *, const char * );
    const char* nextToken();
    bool hasMoreTokens() { return ( pos != 0 ); }

protected:
    char *pos;
    char *end;
    char *buffer;
    int  bufLen;
};

#endif





