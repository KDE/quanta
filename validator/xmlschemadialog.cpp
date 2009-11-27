/*
Copyright (C) 2009  Ruan Strydom <ruan@jcell.co.za>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xmlschemadialog.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>

#include <KDE/KLocalizedString>
#include <KDE/KFileDialog>


XmlSchemaDialog::XmlSchemaDialog ( QWidget* parent, Qt::WindowFlags f ) : KDialog ( parent, f ) {
    QWidget * w = new QWidget();
    QVBoxLayout * vlayout = new QVBoxLayout();
    QHBoxLayout * hlayout = new QHBoxLayout();
    m_comboBox = new QComboBox();
    m_comboBox->setEditable ( true );
    KFileDialog * fdiag = new KFileDialog ( KUrl ( "~" ), "*.xsd *.XSD *.dtd *.DTD|DTD and XML Schema Documents\n*|All Files", this );
    QPushButton *btn = new QPushButton ( i18n ( "Browse.." ) );
    connect ( btn, SIGNAL ( pressed() ), fdiag, SLOT ( show() ) );
    connect ( fdiag, SIGNAL ( fileSelected ( QString ) ), m_comboBox, SLOT ( setEditText ( QString ) ) );
    vlayout->addWidget ( new QLabel ( i18n ( "Select/Enter a XML schema URL, DTD URL or DOCTYPE:" ) ) );
    hlayout->addWidget ( m_comboBox, 100 );
    hlayout->addWidget ( btn );
    vlayout->addLayout ( hlayout );
    vlayout->addStretch(100);
    w->setLayout ( vlayout );
    setMainWidget ( w );
    setMaximumWidth(500);
    setMaximumHeight(300);
}

XmlSchemaDialog::~XmlSchemaDialog() {}

void XmlSchemaDialog::setItems ( QStringList items ) {
    m_comboBox->clear();
    foreach ( QString s, items ) {
        m_comboBox->addItem ( s );
    }
}

QString XmlSchemaDialog::getItem() {
    return m_comboBox->currentText();
}


