/***************************************************************************
                          grepdialog.cpp  -  grep frontend                              
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


#include "grepdialog.h"
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlistbox.h>
#include <qregexp.h>
#include <qlabel.h>
#include <qwhatsthis.h>
#include <kbuttonbox.h>
#include <kfiledialog.h>
#include <kprocess.h>
#include <kapp.h>
#include <klocale.h>

enum quoteEnum { NO_QUOTE=0, SINGLE_QUOTE, DOUBLE_QUOTE };

GrepDialog::GrepDialog(const QString &dirname, QWidget *parent, const char *name)
    : QDialog( parent, name, false ), 
    childproc(0)
{
    setCaption(i18n("Search in Files..."));
    
    QGridLayout *layout = new QGridLayout(this, 6, 3, 10, 4);
    layout->setColStretch(0, 10);
    layout->addColSpacing(1, 10);
    layout->setColStretch(1, 0);
    layout->setColStretch(2, 1);
    layout->addRowSpacing(1, 10);
    layout->setRowStretch(1, 0);
    layout->setRowStretch(2, 10);
    layout->addRowSpacing(4, 10);
    layout->setRowStretch(4, 0);
    
    QGridLayout *input_layout = new QGridLayout(4, 2, 4);
    layout->addLayout(input_layout, 0, 0);
    input_layout->setColStretch(0, 0);
    input_layout->setColStretch(1, 20);

    QLabel *pattern_label = new QLabel(i18n("&Pattern:"), this);
    pattern_label->setFixedSize(pattern_label->sizeHint());
    input_layout->addWidget(pattern_label, 0, 0, AlignRight | AlignVCenter);

    pattern_edit = new QLineEdit(this);
    pattern_label->setBuddy(pattern_edit);
    pattern_edit->setFocus();
    pattern_edit->setMinimumSize(pattern_edit->sizeHint());
    input_layout->addWidget(pattern_edit, 0, 1);
    
    QLabel *files_label = new QLabel(i18n("&Files:"), this);
    files_label->setFixedSize(files_label->sizeHint());
    input_layout->addWidget(files_label, 2, 0, AlignRight | AlignVCenter);

    files_combo = new QComboBox(true, this);
    files_label->setBuddy(files_combo->focusProxy());
    files_combo->setMinimumSize(files_combo->sizeHint());
    files_combo->insertItem("*.html,*.htm,*.shtml,*.HTML,*.HTM,*.css,*.js,*.jsp,*.asp");
    files_combo->insertItem("*.html,*.htm,*.shtml,*.HTML,*.HTM");
    files_combo->insertItem("*.css,*.js");
    files_combo->insertItem("*.js,*.jsp,*.asp");
    files_combo->insertItem("*");
    input_layout->addWidget(files_combo, 2, 1);

    QLabel *dir_label = new QLabel(i18n("&Directory:"), this);
    dir_label->setFixedSize(dir_label->sizeHint());
    input_layout->addWidget(dir_label, 3, 0, AlignRight | AlignVCenter);

    QBoxLayout *dir_layout = new QHBoxLayout(4);
    input_layout->addLayout(dir_layout, 3, 1);
    
    dir_edit = new QLineEdit(this);
    dir_label->setBuddy(dir_edit);
    dir_edit->setText(dirname);
    dir_edit->setMinimumSize(dir_edit->sizeHint());
    dir_layout->addWidget(dir_edit, 10);

    QPushButton *dir_button = new QPushButton(this, "dirButton");
    dir_button->setPixmap( UserIcon("open") );
    dir_button->setFixedHeight(dir_edit->sizeHint().height());
    dir_button->setFixedWidth(30);
    dir_layout->addWidget(dir_button);
    
    recursive_box = new QCheckBox(i18n("&Recursive"), this);
    recursive_box->setMinimumWidth(recursive_box->sizeHint().width());
    recursive_box->setChecked(true);
    dir_layout->addSpacing(10);
    dir_layout->addWidget(recursive_box);

    KButtonBox *actionbox = new KButtonBox(this, KButtonBox::Vertical);
    layout->addWidget(actionbox, 0, 2);
    actionbox->addStretch();
    search_button = actionbox->addButton(i18n("&Search"));
    search_button->setDefault(true);
    stop_button = actionbox->addButton(i18n("S&top"));
    stop_button->setEnabled(false);
    actionbox->addStretch();
    actionbox->layout();

    resultbox = new QListBox(this);
    QFontMetrics rb_fm(resultbox->fontMetrics());
    resultbox->setMinimumSize(rb_fm.width("0")*55,
			      rb_fm.lineSpacing()*15);
    layout->addMultiCellWidget(resultbox, 2, 2, 0, 2);

    QFrame *status_frame = new QFrame(this);
    status_frame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    QBoxLayout *status_layout = new QHBoxLayout(status_frame, 2);

    status_label = new QLabel(i18n("Ready"), status_frame);
    status_layout->addWidget(status_label, 10);

    matches_label = new QLabel(status_frame);
    QFontMetrics ml_fm(matches_label->fontMetrics());
    matches_label->setFixedWidth(ml_fm.width(i18n("9999 matches")));
    matches_label->setFixedHeight(ml_fm.lineSpacing());
    status_layout->addWidget(matches_label, 0);

    status_layout->activate();
    status_frame->adjustSize();
    status_frame->setMinimumSize(status_frame->size());
    layout->addMultiCellWidget(status_frame, 3, 3, 0, 2);

    KButtonBox *buttonbox = new KButtonBox(this);
    layout->addMultiCellWidget(buttonbox, 5, 5, 0, 2);
    QPushButton *clear_button = buttonbox->addButton(i18n("Clear"));
    buttonbox->addStretch();
    QPushButton *done_button = buttonbox->addButton(i18n("Done"));
    buttonbox->layout();

    layout->activate();

    QWhatsThis::add(pattern_edit,
		    i18n("<qt><p>Enter the regular expression you want to search for here.</p>"
			 "Possible meta characters are:<br/>"
             "<ul>"
			 "<li><b>.</b> - Matches any character.</li>"
			 "<li><b>^</b> - Matches the beginning of a line.</li>"
			 "<li><b>$</b> - Matches the end of a line.</li>"
             "<li><b>\\\\\\&lt;</b> - Matches the beginning of a word.</li>"
             "<li><b>\\\\\\&gt;</b> - Matches the end of a word.</li>"
			 "</ul>"
			 "The following repetition operators exist:<br/>"
             "<ul>"
			 "<li><b>?</b> - The preceding item is matches at most once.</li>"
			 "<li><b>*</b> - The preceding item is matched zero or more times.</li>"
			 "<li><b>+</b> - The preceding item is matched once or more times.</li>"
			 "<li><b>{<i>n</i>}</b> - The preceding item is matched exactly <i>n</i> times.</li>"
			 "<li><b>{<i>n</i>,}</b> - The preceding item is matched <i>n</i> or more times.</li>"
			 "<li><b>{,<i>n</i>}</b> - The preceding item is matched at most <i>n</i> times.</li>"
			 "<li><b>{<i>n</i>,<i>m</i>}</b> - The preceding item is matched at least <i>n</i>,"
			 " but at most <i>m</i> times.</li>"
			 "</ul>"
			 "<p>Furthermore, backreferences to bracketed subexpressions are"
			 "available via the notation \\\\<i>n</i>.</p></qt>"
			 ));
    QWhatsThis::add(files_combo,
		    i18n("<qt>Enter the file name pattern of the files to search here. "
			 "You may give several patterns separated by commas.</qt>"));
    QWhatsThis::add(resultbox,
		    i18n("<qt>The results of the grep run are listed here. Select a "
			 "filename/line number combination and press Enter or doubleclick "
			 "on the item to show the respective line in the editor.</qt>"));

    connect( dir_button, SIGNAL(clicked()),
	     SLOT(dirButtonClicked()) );
    connect( resultbox, SIGNAL(selected(const QString &)),
	     SLOT(itemSelected(const QString &)) );
    connect( search_button, SIGNAL(clicked()),
	     SLOT(slotSearch()) );
    connect( stop_button, SIGNAL(clicked()),
	     SLOT(slotStop()) );
    connect( clear_button, SIGNAL(clicked()),
	     SLOT(slotClear()) );
    connect( done_button, SIGNAL(clicked()),
	     SLOT(accept()) );
}


GrepDialog::~GrepDialog()
{
    if (childproc)
      delete childproc;
}


void GrepDialog::dirButtonClicked()
{
    dir_edit->setText(KFileDialog::getExistingDirectory(dir_edit->text()));
}

#include <iostream.h>
void GrepDialog::itemSelected(const QString &item)
{
  int pos;
  QString filename, linenumber;
  
  QString str = item;
  if ( (pos = str.find(':')) != -1)
    {
      filename = str.left(pos);
      str = str.right(str.length()-1-pos);
      if ( (pos = str.find(':')) != -1)
	{
	  linenumber = str.left(pos);
	  emit itemSelected(filename,linenumber.toInt()-1);
	  //		    cout << "Selected file " << filename << ", line " << linenumber << endl;
	}
    }
}


void GrepDialog::processOutput()
{
  int pos;
  while ( (pos = buf.find('\n')) != -1)
    {
      QString item = buf.left(pos);
      if (!item.isEmpty())
	resultbox->insertItem(item);
      buf = buf.right(buf.length()-pos-1);
    }
  
  QString str;
  str.setNum(resultbox->count());
  str += i18n(" matches");
  matches_label->setText(str);
}


void GrepDialog::slotSearch()
{
  search_button->setEnabled(false);
  stop_button->setEnabled(true);
  resultbox->clear();
  
  //
  // Build the find command string
  //
  QString files_temp = files_combo->currentText();
  if ( files_temp.right(1) != "," ) { 
      files_temp = files_temp + ",";
  }

  QString files = " -name ";
  StringTokenizer tokener;
  tokener.tokenize(files_temp,",");

  if ( tokener.hasMoreTokens() ) {
    files = files + " '" + QString( tokener.nextToken() ) + "'" ;
  }
  while( tokener.hasMoreTokens() ) {
    files = files + " -o -name " + "'" + QString( tokener.nextToken() ) + "'";
  }

  QString findCommand = "find '";
  findCommand += dir_edit->text();
  findCommand += "'";

  if ( !recursive_box->isChecked() )
      findCommand += " -maxdepth 1";
  findCommand += files;

  //  warning( "Find command: %s\n", (const char *) findCommand );

  //
  // Build the main search command line
  //

  QString pattern = pattern_edit->text();
  QString commandLine = "%1 | xargs -i grep -n -H -i -e '%2' \"{}\"";
  commandLine = commandLine.arg( findCommand ).arg( pattern );

  //  warning( "Command line: %s\n", (const char *) commandLine );

  //
  // Invoke the command
  //

  childproc = new KShellProcess("/bin/sh");
  *childproc << commandLine;

  connect( childproc, SIGNAL(processExited(KProcess *)),
           SLOT(childExited()) );
  connect( childproc, SIGNAL(receivedStdout(KProcess *, char *, int)),
           SLOT(receivedOutput(KProcess *, char *, int)) );

  status_label->setText(i18n("Searching..."));
  childproc->start(KProcess::NotifyOnExit, KProcess::Stdout);
}

void GrepDialog::slotSearchFor(const QString &pattern){
		slotClear();
		pattern_edit->clear();
		pattern_edit->setText(pattern);
		slotSearch();
}

void GrepDialog::finish()
{
    search_button->setEnabled(true);
    stop_button->setEnabled(false);

    buf += '\n';
    processOutput();
    if (childproc)
      delete childproc;
    childproc = 0;
}


void GrepDialog::slotStop()
{
    finish();

    status_label->setText(i18n("Stopped"));
}


void GrepDialog::childExited()
{
    if ( childproc->normalExit() ) {
        int status = childproc->exitStatus();   
        finish();

        status_label->setText( (status == 1)? i18n("No matches found")
                              : (status == 2)? i18n("Syntax error in pattern")
                               : i18n("Ready") );
        if (status != 0)
            matches_label->setText("");
    }
    else {
        finish();
        status_label->setText( i18n("Child process died unexpectedly") );
    }
}


void GrepDialog::receivedOutput(KProcess */*proc*/, char *buffer, int buflen)
{
    buf += QString::fromLocal8Bit(buffer, buflen+1);
    processOutput();
}


void GrepDialog::slotClear()
{
    finish();
    resultbox->clear();

    status_label->setText(i18n("Ready"));
    matches_label->setText("");
}


void  GrepDialog::setDirName(const QString &dir){
  dir_edit->setText(dir);
}


StringTokenizer::StringTokenizer()
{
    buffer = 0;
    pos    = 0;
    end    = 0;
    bufLen = 0;
}

void StringTokenizer::tokenize( const char *str, const char *_separators )
{
    if ( *str == '\0' )
    {
	pos = 0;
	return;
    }

    int strLength = strlen( str ) + 1;

    if ( bufLen < strLength )
    {
	delete [] buffer;
	buffer = new char[ strLength ];
	bufLen = strLength;
    }

    const char *src = str;
    end = buffer;
    int quoted = NO_QUOTE;
    
    for ( ; *src != '\0'; src++ )
    {
	char *x = strchr( _separators, *src );
        if (( *src == '\"' ) && !quoted)
           quoted = DOUBLE_QUOTE;
        else if (( *src == '\'') && !quoted)
           quoted = SINGLE_QUOTE;
        else if ( (( *src == '\"') && (quoted == DOUBLE_QUOTE)) ||
                 (( *src == '\'') && (quoted == SINGLE_QUOTE)))
           quoted = NO_QUOTE;
	else if ( x && !quoted )
	    *end++ = 0;
	else
	    *end++ = *src;
    }

    *end = 0;

    if ( end - buffer <= 1 )
	pos = 0;	// no tokens
    else
	pos = buffer;
}

const char* StringTokenizer::nextToken()
{
    if ( pos == 0 )
	return 0;

    char *ret = pos;
    pos += strlen( ret ) + 1;
    if ( pos >= end )
	pos = 0;

    return ret;
}

StringTokenizer::~StringTokenizer()
{
    if ( buffer != 0 )
	delete [] buffer;
}






