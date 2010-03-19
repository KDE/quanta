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

#include "xmlformatterplugin.h"
#include "xmlformatter.h"
#include "xmlformatterpreferences.h"

#include <QtCore/QPair>
#include <QtCore/QList>

#include <KDE/KPluginFactory>
#include <KDE/KPluginLoader>
#include <KDE/KLocalizedString>
#include <KDE/KAboutData>
#include <KDE/KTextEditor/Document>
#include <KDE/KDebug>
#include <KDE/KMessageBox>
#include <KDE/KActionCollection>
#include <KDE/KAction>
#include <KDE/KTextEditor/SmartCursor>
#include <KDE/KTextEditor/View>

#include <interfaces/icore.h>
#include <interfaces/iplugin.h>
#include <interfaces/isourceformatter.h>
#include <interfaces/idocument.h>
#include <interfaces/idocumentcontroller.h>

using namespace KDevelop;

K_PLUGIN_FACTORY ( KdevPlugFactory, registerPlugin<XmlFormatterPlugin>(); )
K_EXPORT_PLUGIN ( KdevPlugFactory ( KAboutData ( "kdevxmlformatter", "kdevxmlformatter", ki18n ( "XML Formatter" ), "0.1", ki18n ( "A plugin for formatting XML files" ), KAboutData::License_GPL ) ) )


XmlFormatterPlugin::XmlFormatterPlugin ( QObject *parent, const QVariantList& )
        : IPlugin ( KdevPlugFactory::componentData(), parent ) {
    KDEV_USE_EXTENSION_INTERFACE ( ISourceFormatter );

    m_formatter = new XmlFormatter();
    
    SourceFormatterStyle defaultStyle("XML");
    defaultStyle.setContent("INDENT=2");
    defaultStyle.setCaption("XML");
    m_style = defaultStyle;
    m_styles.append(m_style);

    m_compactAction = actionCollection()->addAction ( "xmlcompact_xmllanguagesupport" );
    m_compactAction->setText ( i18n ( "Compact XML" ) );
    m_compactAction->setToolTip ( i18n ( "Compact the XML by removing all white-space characters" ) );
    m_compactAction->setWhatsThis ( i18n ( "<b>Compact XML</b><p> Compact the XML by removing all white-space characters</p>" ) );
    m_compactAction->setEnabled ( false );
    connect ( m_compactAction, SIGNAL ( triggered ( bool ) ), this, SLOT ( slotCompactXml() ) );

    m_escapeAction = actionCollection()->addAction ( "xmlescape_xmllanguagesupport" );
    m_escapeAction->setText ( i18n ( "Escape XML" ) );
    m_escapeAction->setToolTip ( i18n ( "Encode the XML by escaping all unsafe characters" ) );
    m_escapeAction->setWhatsThis ( i18n ( "<b>Encode XML</b><p> Encode the XML by escaping all unsafe characters</p>" ) );
    m_escapeAction->setEnabled ( false );
    connect ( m_escapeAction, SIGNAL ( triggered ( bool ) ), this, SLOT ( slotEncodeXml() ) );

    m_unescapeAction = actionCollection()->addAction ( "xmlunescape_xmllanguagesupport" );
    m_unescapeAction->setText ( i18n ( "Un-escape XML" ) );
    m_unescapeAction->setToolTip ( i18n ( "Un-escape the XML by removing all escaped strings" ) );
    m_unescapeAction->setWhatsThis ( i18n ( "<b>Un-escape XML</b><p> Un-escape the XML by removing all escaped strings</p>" ) );
    m_unescapeAction->setEnabled ( false );
    connect ( m_unescapeAction, SIGNAL ( triggered ( bool ) ), this, SLOT ( slotDecodeXml() ) );
}

XmlFormatterPlugin::~XmlFormatterPlugin() {
    if ( m_formatter )
        delete m_formatter;
}

ContextMenuExtension XmlFormatterPlugin::contextMenuExtension ( Context* context ) {
    Q_UNUSED(context);
    
    ContextMenuExtension ext;

    m_compactAction->setEnabled ( false );

    IDocument* doc = ICore::self()->documentController()->activeDocument();
    if ( !doc )
        return ext;

    KTextEditor::Document *tdoc = doc->textDocument();
    if ( !tdoc )
        return ext;

    KMimeType::Ptr mime = doc->mimeType();
    if ( mime->is("application/xml") ||
        mime->is("application/xsd") ||
        mime->is("application/xslt+xml") ||
        mime->is("application/wsdl+xml") ||
        mime->is("application/x-wsdl")) {
        m_compactAction->setEnabled ( true );
        m_unescapeAction->setEnabled ( true );
        m_escapeAction->setEnabled ( true );
        ext.addAction ( ContextMenuExtension::EditGroup, m_compactAction );
        ext.addAction ( ContextMenuExtension::EditGroup, m_unescapeAction );
        ext.addAction ( ContextMenuExtension::EditGroup, m_escapeAction );
    }

    return ext ;
}

QString XmlFormatterPlugin::name() {
    return "xmlformatter";
}

QString XmlFormatterPlugin::caption() {
    return "XML Formatter";
}

QString XmlFormatterPlugin::description() {
    return i18n ( "<b>XML Formatter</b> is a XML indenter, formatter." );
}

QString XmlFormatterPlugin::highlightModeForMime ( const KMimeType::Ptr &mime ) {
    Q_UNUSED ( mime );
    return "XML";
}

QString XmlFormatterPlugin::formatSource ( const QString& text, const KMimeType::Ptr& mime, const QString& leftContext, const QString& rightContext ) {
    Q_UNUSED ( mime );
    m_formatter->loadStyle(m_style.content());
    return m_formatter->formatSource ( text, leftContext, rightContext );
}

QString XmlFormatterPlugin::formatSourceWithStyle(SourceFormatterStyle style, const QString& text, const KMimeType::Ptr& mime, const QString& leftContext, const QString& rightContext)
{
    Q_UNUSED ( mime );
    m_formatter->loadStyle(style.content());
    return m_formatter->formatSource ( text, leftContext, rightContext );
}

/*
QMap<QString, QString> XmlFormatterPlugin::predefinedStyles ( const KMimeType::Ptr & ) {
    QMap<QString, QString> styles;
    styles.insert ( "XML", "XML" );
    return styles;
}
*/

/*
void XmlFormatterPlugin::setStyle ( const QString &name, const QString &content ) {
    m_formatter->loadStyle ( content );
    m_formatter->loadPredefStyle ( name );
}
*/

QList< SourceFormatterStyle > XmlFormatterPlugin::predefinedStyles()
{
  return m_styles;
}

void XmlFormatterPlugin::setStyle(const KDevelop::SourceFormatterStyle& style)
{
    kDebug() << style.name() << style.content();
  m_style = style;
  m_formatter->loadStyle(style.content());
}

SourceFormatterStyle XmlFormatterPlugin::style() const
{
  return m_style;
}

KDevelop::SettingsWidget* XmlFormatterPlugin::editStyleWidget ( const KMimeType::Ptr &mime ) {
    Q_UNUSED ( mime );
    return new XmlFormatterPreferences ( 0 );
}

QString XmlFormatterPlugin::previewText ( const KMimeType::Ptr & ) {
    return QString ( "<?xml version='1.0' encoding='UTF-8'?>\n" ) +
           "<Quote type=\"virtual\" >\n" +
           "  <ContactAboutQuote value=\"true\" />\n" +
           "  <SendMarketingMaterial value=\"false\" />\n" +
           "  <DisplayCommission value=\"false\" />\n" +
           "  <RequestingEntity entityId=\"1051920013\" >\n" +
           "    <RequestingEntityAttribute attribute=\"Name\" >Peterpan</RequestingEntityAttribute>\n" +
           "  </RequestingEntity>\n" +
           "</Quote>\n";
}

ISourceFormatter::IndentationType XmlFormatterPlugin::indentationType() {
    return ISourceFormatter::IndentWithSpacesAndConvertTabs;
}

int XmlFormatterPlugin::indentationLength() {
    return m_formatter->options() ["INDENT"].toInt();
}

void XmlFormatterPlugin::slotCompactXml() {
    IDocument* doc = ICore::self()->documentController()->activeDocument();
    if ( !doc )
        return;

    KTextEditor::Document *tdoc = doc->textDocument();
    if ( !tdoc )
        return;

    if ( tdoc->activeView()->selection() )
        tdoc->replaceText ( tdoc->activeView()->selectionRange(),
                            m_formatter->compactSource ( tdoc->activeView()->selectionText() ) );
    else
        tdoc->setText ( m_formatter->compactSource ( tdoc->text() ) );
}

void XmlFormatterPlugin::slotDecodeXml() {
    IDocument* doc = ICore::self()->documentController()->activeDocument();
    if ( !doc )
        return;

    KTextEditor::Document *tdoc = doc->textDocument();
    if ( !tdoc )
        return;

    if ( tdoc->activeView()->selection() )
        tdoc->replaceText ( tdoc->activeView()->selectionRange(),
                            m_formatter->unescapeSource ( tdoc->activeView()->selectionText() ) );
    else
        tdoc->setText ( m_formatter->unescapeSource ( tdoc->text() ) );

}

void XmlFormatterPlugin::slotEncodeXml() {
    IDocument* doc = ICore::self()->documentController()->activeDocument();
    if ( !doc )
        return;

    KTextEditor::Document *tdoc = doc->textDocument();
    if ( !tdoc )
        return;

    if ( tdoc->activeView()->selection() )
        tdoc->replaceText ( tdoc->activeView()->selectionRange(),
                            m_formatter->escapeSource ( tdoc->activeView()->selectionText() ) );
    else
        tdoc->setText ( m_formatter->escapeSource ( tdoc->text() ) );
}

#include "xmlformatterplugin.moc"


