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

#include "xmlvalidatorplugin.h"
#include "xmlschemadialog.h"
#include "xmlvalidator.h"

#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QFile>
#include <QtCore/QDir>

#include <KDE/KPluginFactory>
#include <KDE/KPluginLoader>
#include <KDE/KLocalizedString>
#include <KDE/KAboutData>
#include <KDE/KDebug>
#include <KDE/KMessageBox>
#include <KDE/KActionCollection>
#include <KDE/KAction>
#include <KDE/KIO/CopyJob>
#include <KDE/KTextEditor/Document>
#include <KDE/KTextEditor/SmartCursor>
#include <KDE/KTextEditor/View>
#include <KDE/KParts/MainWindow>

#include <interfaces/idocument.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/iuicontroller.h>
#include <interfaces/icore.h>
#include <interfaces/iplugin.h>
#include <interfaces/isourceformatter.h>

using namespace KDevelop;

K_PLUGIN_FACTORY ( KdevPlugFactory, registerPlugin<XmlValidatorPlugin>(); )
K_EXPORT_PLUGIN ( KdevPlugFactory ( KAboutData ( "kdevxmlvalidator", "kdevxmlvalidator", ki18n ( "XML Validator" ), "0.1", ki18n ( "Validates XML files against a DTD or schema" ), KAboutData::License_GPL ) ) )


XmlValidatorPlugin::XmlValidatorPlugin ( QObject *parent, const QVariantList& )
        : IPlugin ( KdevPlugFactory::componentData(), parent ) {

    m_validateAction = actionCollection()->addAction ( "xmlvalidate_xmllanguagesupport" );
    m_validateAction->setText ( i18n ( "Validate XML" ) );
    m_validateAction->setToolTip ( i18n ( "Validate the XML" ) );
    m_validateAction->setWhatsThis ( i18n ( "<b>Validate XML</b><p> Validate the XML" ) );
    m_validateAction->setEnabled ( false );
    connect ( m_validateAction, SIGNAL ( triggered ( bool ) ), this, SLOT ( slotValidateXml() ) );
}

XmlValidatorPlugin::~XmlValidatorPlugin() {
}

ContextMenuExtension XmlValidatorPlugin::contextMenuExtension ( Context* context ) {
    Q_UNUSED(context)
  
    ContextMenuExtension ext;
    m_validateAction->setEnabled ( false );
    IDocument* doc = ICore::self()->documentController()->activeDocument();
    if ( !doc )
        return ext;
    KTextEditor::Document *tdoc = doc->textDocument();
    if ( !tdoc )
        return ext;
    KMimeType::Ptr mime = doc->mimeType();
    if ( mime->is ( "text/xml" ) ||
            mime->is ( "application/xml" ) ||
            mime->is ( "application/xslt+xml" ) ||
            mime->is ( "application/xsd" ) ||
            mime->is ( "application/xsd+xml" ) ||
            mime->is ( "text/xsd" ) ) {

        m_validateAction->setEnabled ( true );
        ext.addAction ( ContextMenuExtension::EditGroup, m_validateAction );
    }

    return ext ;
}

void XmlValidatorPlugin::slotValidateXml() {
    IDocument* doc = ICore::self()->documentController()->activeDocument();
    if ( !doc )
        return;

    KTextEditor::Document *tdoc = doc->textDocument();
    if ( !tdoc )
        return;

    QString strError;
    int line = -1;
    int col = -1;

    QStringList schemas;

    QDomDocument xdoc;
    if ( !xdoc.setContent ( tdoc->text(), true, &strError, &line, &col ) ) {
        KMessageBox::error ( 0, QString ( "%1:\n%2:%3 %4" ).arg ( i18n ( "Error in document format" ) ).arg ( line ).arg ( col ).arg ( strError ) );
        return;
    }

    QDomDocumentType doctype = xdoc.doctype();
    if ( !doctype.isNull() ) {
        QString systemId = doctype.systemId();
        if ( !systemId.isEmpty() )
            schemas.append ( systemId );
        if ( !doctype.name().isEmpty() )
            schemas.append ( QString ( "DOCTYPE %1" ).arg ( doctype.name() ) );
    }

    QDomNodeList nodeList = xdoc.childNodes();
    for ( int i = 0; i < nodeList.size(); i++ ) {
        QDomNode node = nodeList.at ( i );

        QString nameSpace = node.namespaceURI();
        if ( node.hasAttributes() ) {
            QDomNamedNodeMap m = node.attributes();
            for ( int j = 0; j < m.count(); j++ ) {
                QDomNode attribute = m.item ( j );
                if ( attribute.namespaceURI() == "http://www.w3.org/2001/XMLSchema" || attribute.namespaceURI() == "http://www.w3.org/2001/XMLSchema-instance" ) {
                    if ( attribute.localName() == "schemaLocation" ) {
                        QStringList sl = attribute.nodeValue().split ( QRegExp ( "\\s*" ) );
                        for ( int k = 0; k < sl.size(); k += 2 )
                            schemas.append ( sl[k+1] );
                    }
                    if ( attribute.localName() == "noNamespaceSchemaLocation" ) {
                        schemas.append ( attribute.nodeValue() );
                    }
                }
            }
        }
    }

    QString schema;
    static XmlSchemaDialog *w = ( {
        XmlSchemaDialog *w = new XmlSchemaDialog ( ICore::self()->uiController()->activeMainWindow()->widget() );
        connect ( w, SIGNAL ( okClicked() ), this, SLOT ( slotValidate() ) );
        w;
    } );
    w->setItems ( schemas );
    w->show();
}

void XmlValidatorPlugin::slotValidate() {
    QObject * o = this->sender();
    XmlSchemaDialog *w = dynamic_cast<XmlSchemaDialog *> ( o );
    if ( !w )
        return;
    w->hide();

    IDocument* doc = ICore::self()->documentController()->activeDocument();
    if ( !doc )
        return;

    QString schema = w->getItem();
    if ( !schema.isEmpty() ) {
        XmlValidator v;
        if ( schema.startsWith ( "DOCTYPE" ) ) {
            v.validateDTD ( doc->url().toLocalFile() );
        } else {
            QString localSchema = getLocalURLForSchema ( doc->url(), KUrl ( schema ) );
            if ( localSchema.isEmpty() || !QFile::exists ( localSchema ) ) {
                KMessageBox::error ( 0, QString ( "%1: %2" ).arg ( i18n ( "Unable to locate" ) ).arg ( schema ) );
                return;
            }

            KMimeType::Ptr mime = KMimeType::findByUrl ( KUrl ( localSchema ) );
            if ( mime->is ( "application/xsd" ) || mime->is ( "application/xsd+xml" ))
                v.validateSchema ( doc->url().toLocalFile(), localSchema );
            else
                v.validateDTD ( doc->url().toLocalFile(), localSchema );
        }
    }
}

QString XmlValidatorPlugin::getLocalURLForSchema ( const KUrl& file, const KUrl& schemaUrl ) {
    if ( file.isEmpty() || schemaUrl.isEmpty() )
        return QString();

    if ( schemaUrl.isRelative() ) {
        QString schemaDir = schemaUrl.directory ( KUrl::IgnoreTrailingSlash );
        if ( !schemaDir.isEmpty() )
            return QString ( "%1/%2/%3" ).arg ( file.directory ( KUrl::IgnoreTrailingSlash ), schemaDir, schemaUrl.fileName() );
        else
            return QString ( "%1/%2" ).arg ( file.directory ( KUrl::IgnoreTrailingSlash ), schemaUrl.fileName() );
    }

    if ( schemaUrl.isLocalFile() )
        return schemaUrl.toLocalFile();

    if ( !schemaUrl.isLocalFile() ) {
        QString localFileName = QString ( schemaUrl.toEncoded().constData() ).replace ( QRegExp ( "[/]|[:]" ), "_" );
        KUrl localUrl ( QString ( "/tmp/%1" ).arg ( localFileName ) );
        if ( QFile::exists ( localUrl.toLocalFile() ) )
            return localUrl.toLocalFile();
        KJob *job = KIO::copy ( schemaUrl, localUrl );
        if ( job->exec() )
            return localUrl.toLocalFile();
        else
            KMessageBox::error ( 0, i18n ( "Unable to download the schema" ) );
    }

    return QString();
}

#include "xmlvalidatorplugin.moc"
