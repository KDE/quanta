/* This file is part of the KDE project
   Copyright (C) 2001 Christoph Cullmann <cullmann@kde.org>
   Copyright (C) 2001 Joseph Wenninger <jowenn@kde.org>
   Copyright (C) 2001 Anders Lund <anders.lund@lund.tdcadsl.dk>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

// $Id$

#include "kategrepdialog.h"
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlistbox.h>
#include <qregexp.h>
#include <qwhatsthis.h>
#include <kbuttonbox.h>
#include <kfiledialog.h>
#include <kprocess.h>
#include <kapplication.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kwin.h>
#include <kurlrequester.h>
#include <kurlcompletion.h>
#include <kcombobox.h>

const char *template_desc[] = {
    "normal",
    "assignment",
    "->MEMBER(",
    "class::MEMBER(",
    "OBJECT->member(",
    0
};

const char *template_str[] = {
    "%s",
    "\\<%s\\>[\t ]*=[^=]",
    "\\->[\\t ]*\\<%s\\>[\\t ]*(",
    "[a-z0-9_$]\\+[\\t ]*::[\\t ]*\\<%s\\>[\\t ]*(",
    "\\<%s\\>[\\t ]*\\->[\\t ]*[a-z0-9_$]\\+[\\t ]*(",
    0
};


GrepDialog::GrepDialog(QString dirname, QWidget *parent, const char *name)
    : KDialog(parent, name, false), childproc(0)
{
    setCaption(i18n("Find in Files"));
    KWin::setType(winId(),NET::Tool);
    config = KGlobal::config();
    config->setGroup("GrepDialog");
    lastSearchItems = config->readListEntry("LastSearchItems");
    lastSearchPaths = config->readListEntry("LastSearchPaths");

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

    pattern_combo = new QComboBox(true, this);
    pattern_combo->insertStringList(lastSearchItems);
    pattern_combo->setEditText(QString::null);
    pattern_combo->setInsertionPolicy(QComboBox::NoInsertion);
    pattern_label->setBuddy(pattern_combo);
    pattern_combo->setFocus();
    pattern_combo->setMinimumSize(pattern_combo->sizeHint());
    input_layout->addWidget(pattern_combo, 0, 1);

    QLabel *template_label = new QLabel(i18n("&Template:"), this);
    template_label->setFixedSize(template_label->sizeHint());
    input_layout->addWidget(template_label, 1, 0, AlignRight | AlignVCenter);

    QBoxLayout *template_layout = new QHBoxLayout(4);
    input_layout->addLayout(template_layout, 1, 1);

    template_edit = new QLineEdit(this);
    template_label->setBuddy(template_edit);
    template_edit->setText(template_str[0]);
    template_edit->setMinimumSize(template_edit->sizeHint());
    template_layout->addWidget(template_edit);

    QComboBox *template_combo = new QComboBox(false, this);
    template_combo->insertStrList(template_desc);
    template_combo->adjustSize();
    template_combo->setFixedSize(template_combo->size());
    template_layout->addWidget(template_combo);

    QLabel *files_label = new QLabel(i18n("&Files:"), this);
    files_label->setFixedSize(files_label->sizeHint());
    input_layout->addWidget(files_label, 2, 0, AlignRight | AlignVCenter);

    files_combo = new QComboBox(true, this);
    files_label->setBuddy(files_combo->focusProxy());
    files_combo->setMinimumSize(files_combo->sizeHint());
    files_combo->insertItem("*.h,*.hxx,*.cpp,*.cc,*.C,*.cxx,*.idl,*.c");
    files_combo->insertItem("*.cpp,*.cc,*.C,*.cxx,*.c");
    files_combo->insertItem("*.h,*.hxx,*.idl");
    files_combo->insertItem("*");
    input_layout->addWidget(files_combo, 2, 1);

    QLabel *dir_label = new QLabel(i18n("&Directory:"), this);
    dir_label->setFixedSize(dir_label->sizeHint());
    input_layout->addWidget(dir_label, 3, 0, AlignRight | AlignVCenter);

    QBoxLayout *dir_layout = new QHBoxLayout(3);
    input_layout->addLayout(dir_layout, 3, 1);
/*
    dir_combo = new QComboBox(true, this);
    dir_combo->insertStringList(lastSearchPaths);
    dir_combo->setInsertionPolicy(QComboBox::NoInsertion);
    dir_combo->setEditText(dirname);
    dir_combo->setMinimumSize(dir_combo->sizeHint());
    dir_label->setBuddy(dir_combo);
    dir_layout->addWidget(dir_combo, 10);

    QPushButton *dir_button = new QPushButton(this, "dirButton");
		QPixmap pix = SmallIcon("fileopen");
    dir_button->setPixmap(pix);
    dir_button->setFixedHeight(dir_combo->sizeHint().height());
    dir_button->setFixedWidth(30);
    dir_layout->addWidget(dir_button);
*/
    // anders: KDE is an amazing tool:)
    dir_combo = new KURLRequester( new KComboBox(true, this), this, "dir combo" );
    dir_combo->completionObject()->setMode(KURLCompletion::DirCompletion);
    dir_combo->comboBox()->insertStringList(lastSearchPaths);
    dir_combo->setMode( KFile::Directory|KFile::LocalOnly );
    dir_layout->addWidget(dir_combo);
    dir_label->setBuddy(dir_combo);

    recursive_box = new QCheckBox(i18n("&Recursive"), this);
    recursive_box->setMinimumWidth(recursive_box->sizeHint().width());
    recursive_box->setChecked(true);
    dir_layout->addSpacing(10);
    dir_layout->addWidget(recursive_box);

    KButtonBox *actionbox = new KButtonBox(this, Qt::Vertical);
    layout->addWidget(actionbox, 0, 2);
    actionbox->addStretch();
    search_button = actionbox->addButton(i18n("&Search"));
    search_button->setDefault(true);
    cancel_button = actionbox->addButton(i18n("Cancel"));
    cancel_button->setEnabled(false);
    QPushButton *clear_button = actionbox->addButton(i18n("Clear"));
    QPushButton *done_button = actionbox->addButton(i18n("Done"));
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

    layout->activate();

    QWhatsThis::add(pattern_combo,
		    i18n("Enter the regular expression you want to search for here.<br>"
			 "Possible meta characters are:<br>"
			 "<b>.</b> - Matches any character<br>"
			 "<b>^</b> - Matches the beginning of a line<br>"
			 "<b>$</b> - Matches the end of a line<br>"
	                 "<b>\\\\\\&lt;</b> - Matches the beginning of a word<br>"
                         "<b>\\\\\\&gt;</b> - Matches the end of a word<br>"
			 "<br>"
			 "The following repetition operators exist:<br>"
			 "<b>?</b> - The preceding item is matched at most once<br>"
			 "<b>*</b> - The preceding item is matched zero or more times<br>"
			 "<b>+</b> - The preceding item is matched one or more times<br>"
			 "<b>{<i>n</i>}</b> - The preceding item is matched exactly <i>n</i> times<br>"
			 "<b>{<i>n</i>,}</b> - The preceding item is matched <i>n</i> or more times<br>"
			 "<b>{,<i>n</i>}</b> - The preceding item is matched at most <i>n</i> times<br>"
			 "<b>{<i>n</i>,<i>m</i>}</b> - The preceding item is matched at least <i>n</i>,<br>"
			 "   but at most <i>m</i> times.<br>"
			 "<br>"
			 "Furthermore, backreferences to bracketed subexpressions are<br>"
			 "available via the notation \\\\<i>n</i>."
			 ));
    QWhatsThis::add(files_combo,
		    i18n("Enter the file name pattern of the files to search here.\n"
			 "You may give several patterns separated by commas"));
    QWhatsThis::add(template_edit,
		    i18n("You can choose a template for the pattern from the combo box\n"
			 "and edit it here. The string %s in the template is replaced\n"
			 "by the pattern input field, resulting in the regular expression\n"
			 "to search for."));
    QWhatsThis::add(dir_combo,
		    i18n("Enter the directory which contains the files you want to search in."));
    QWhatsThis::add(recursive_box,
		    i18n("Check this box to search in all subdirectories."));
    QWhatsThis::add(resultbox,
		    i18n("The results of the grep run are listed here. Select a\n"
			 "filename/line number combination and press Enter or doubleclick\n"
			 "on the item to show the respective line in the editor."));

    connect( template_combo, SIGNAL(activated(int)),
	     SLOT(templateActivated(int)) );
/*    connect( dir_button, SIGNAL(clicked()),
	     SLOT(dirButtonClicked()) );*/
    connect( resultbox, SIGNAL(selected(const QString&)),
	     SLOT(itemSelected(const QString&)) );
    connect( search_button, SIGNAL(clicked()),
	     SLOT(slotSearch()) );
    connect( cancel_button, SIGNAL(clicked()),
	     SLOT(slotCancel()) );
    connect( clear_button, SIGNAL(clicked()),
	     SLOT(slotClear()) );
    connect( done_button, SIGNAL(clicked()),
        SLOT(accept()) );
    connect ( pattern_combo->lineEdit(), SIGNAL(textChanged ( const QString & )),
	      SLOT( patternTextChanged( const QString & )));
    patternTextChanged( pattern_combo->lineEdit()->text());
}


GrepDialog::~GrepDialog()
{
    delete childproc;
}

void GrepDialog::patternTextChanged( const QString & _text)
{
    search_button->setEnabled( !_text.isEmpty() );
}

/*
void GrepDialog::dirButtonClicked()
{
    dir_combo->setEditText(KFileDialog::getExistingDirectory(dir_combo->currentText()));
}
*/

void GrepDialog::templateActivated(int index)
{
    template_edit->setText(template_str[index]);
}


void GrepDialog::itemSelected(const QString& item)
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
            //	kdDebug() << "Selected file " << filename << ", line " << linenumber << endl;
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
    if (pattern_combo->currentText().isEmpty())
        return;

    search_button->setEnabled(false);
    cancel_button->setEnabled(true);

    QString files;
    QString files_temp = files_combo->currentText();
    if (files_temp.right(1) != ",")
        files_temp = files_temp + ",";

    QStringList tokens = QStringList::split ( ",", files_temp, FALSE );
    QStringList::Iterator it = tokens.begin();
    if (it != tokens.end())
        files = " '"+(*it++)+"'" ;

    for ( ; it != tokens.end(); it++ )
        files = files + " -o -name " + "'"+(*it)+ "'";

    status_label->setText(i18n("Searching..."));

    QString pattern = template_edit->text();
    pattern.replace(QRegExp("%s"), pattern_combo->currentText());
    pattern.replace(QRegExp("'"), "'\\''");

    QString filepattern = "`find '";
    filepattern += dir_combo->/*currentText*/url();
    filepattern += "'";
    if (!recursive_box->isChecked())
        filepattern += " -maxdepth 1";
    filepattern += " \\( -name ";
    filepattern += files;
    filepattern += " \\) -print";
    filepattern += "`";

    childproc = new KShellProcess();
    *childproc << "grep";
    *childproc << "-n";
    *childproc << (QString("-e '") + pattern + "'");
    *childproc << filepattern;
    *childproc << "/dev/null";

    connect( childproc, SIGNAL(processExited(KProcess *)),
         SLOT(childExited()) );
    connect( childproc, SIGNAL(receivedStdout(KProcess *, char *, int)),
         SLOT(receivedOutput(KProcess *, char *, int)) );
    // actually it should be checked whether the process was started succesfully
    /*bool success=*/childproc->start(KProcess::NotifyOnExit, KProcess::Stdout);
}

void GrepDialog::slotSearchFor(QString pattern){
    slotClear();
    pattern_combo->setEditText(pattern);
    slotSearch();
}

void GrepDialog::finish()
{
    search_button->setEnabled(true);
    cancel_button->setEnabled(false);

    buf += '\n';
    processOutput();
    delete childproc;
    childproc = 0;

    config->setGroup("GrepDialog");
    if (lastSearchItems.contains(pattern_combo->currentText()) == 0) {
        pattern_combo->insertItem(pattern_combo->currentText(), 0);
        lastSearchItems.prepend(pattern_combo->currentText());
        if (lastSearchItems.count() > 10) {
            lastSearchItems.remove(lastSearchItems.fromLast());
            pattern_combo->removeItem(pattern_combo->count() - 1);
        }
        config->writeEntry("LastSearchItems", lastSearchItems);
    }
    if (lastSearchPaths.contains(dir_combo->/*currentText*/url()) == 0) {
        dir_combo->comboBox()->insertItem(dir_combo->/*currentText*/url(), 0);
        lastSearchPaths.prepend(dir_combo->/*currentText*/url());
        if (lastSearchPaths.count() > 10) {
            lastSearchPaths.remove(lastSearchPaths.fromLast());
            dir_combo->comboBox()->removeItem(dir_combo->comboBox()->count() - 1);
        }
        config->writeEntry("LastSearchPaths", lastSearchPaths);
    }
}


void GrepDialog::slotCancel()
{
    finish();

    status_label->setText(i18n("Canceled"));
}


void GrepDialog::childExited()
{
    int status = childproc->exitStatus();

    finish();

    status_label->setText( (status == 1)
                              ? i18n("No matches found")
                              : (status == 2)
                                  ? i18n("Syntax error in pattern")
                                  : i18n("Ready") );
    if (status != 0)
	      matches_label->setText("");

}


void GrepDialog::receivedOutput(KProcess */*proc*/, char *buffer, int buflen)
{
    buf += QCString(buffer, buflen+1);
    processOutput();
}


void GrepDialog::slotClear()
{
    finish();
    resultbox->clear();

    status_label->setText(i18n("Ready"));
    matches_label->setText("");
}


void  GrepDialog::setDirName(QString dir){
//    dir_combo->setEditText(dir);
    dir_combo->setURL(dir);
}
#include "kategrepdialog.moc"
