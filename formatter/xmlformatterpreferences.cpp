/*****************************************************************************
 * Copyright (c) 2009 Ruan Strydom <rm3dom@gmail.com>                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#include "xmlformatterpreferences.h"
#include "xmlformatter.h"

#include <KDE/KDebug>


XmlFormatterPreferences::XmlFormatterPreferences ( QWidget* parent, Qt::WindowFlags f )
        : KDevelop::SettingsWidget ( parent ) {
    Q_UNUSED ( f );
    setupUi ( this );
    connect ( spinBox, SIGNAL ( valueChanged ( int ) ), this, SLOT ( updateText ( int ) ) );
    m_formatter = new XmlFormatter;
}

XmlFormatterPreferences::~XmlFormatterPreferences() {
    if ( m_formatter )
        delete m_formatter;
}

void XmlFormatterPreferences::load(const KDevelop::SourceFormatterStyle& style)
{
    kDebug() << style.name() << style.content();
    m_formatter->loadStyle ( style.content() );
    int indent = getOptions()["INDENT"].toInt();
    if ( indent < 0 || indent > 99 )
      indent = 2;
    spinBox->setValue(indent);
}

QString XmlFormatterPreferences::save() {
    return m_formatter->saveStyle();
}

QMap< QString, QVariant > XmlFormatterPreferences::getOptions() {
    return m_formatter->options();
}

void XmlFormatterPreferences::updateText ( int indent ) {
    QString text = QString ( "<?xml version='1.0' encoding='UTF-8'?>\n" ) +
                   "<Quote type=\"virtual\" >\n" +
                   "  <ContactAboutQuote value=\"true\" />\n" +
                   "  <SendMarketingMaterial value=\"false\" />\n" +
                   "  <DisplayCommission value=\"false\" />\n" +
                   "  <RequestingEntity entityId=\"1051920013\" >\n" +
                   "    <RequestingEntityAttribute attribute=\"Name\" >Peterpan</RequestingEntityAttribute>\n" +
                   "  </RequestingEntity>\n" +
                   "</Quote>\n";
    m_formatter->options().insert ( "INDENT", QVariant ( indent ) );
    emit previewTextChanged ( m_formatter->formatSource ( text ) );
}

#include "xmlformatterpreferences.moc"
