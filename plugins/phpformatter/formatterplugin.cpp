/*****************************************************************************
 * Copyright (c) 2010 Ruan Strydom <rm3dom@gmail.com>                        *
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

#include "formatterplugin.h"

#include "formatter.h"
#include "formatterpreferences.h"

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
#include <interfaces/isourceformattercontroller.h>


using namespace KDevelop;
using namespace Php;

K_PLUGIN_FACTORY ( KdevPlugFactory, registerPlugin<Php::FormatterPlugin>(); )
K_EXPORT_PLUGIN ( KdevPlugFactory ( KAboutData ( "kdevphpformatter", "kdevphpformatter", ki18n ( "PHP Formatter" ), "0.1", ki18n ( "A plugin for formatting PHP files" ), KAboutData::License_GPL ) ) )

FormatterPlugin::FormatterPlugin ( QObject *parent, const QVariantList& )
        : IPlugin ( KdevPlugFactory::componentData(), parent ) {
    KDEV_USE_EXTENSION_INTERFACE ( ISourceFormatter );
    m_formatter = new Formatter();
    
    SourceFormatterStyle defaultStyle("DEFAULT");
    defaultStyle.setContent("indent_size=4,keep_redundant_lines=true,else_along_curly=true,space_after_if=true");
    defaultStyle.setCaption("DEFAULT");
    m_style = defaultStyle;
    m_styles.append(m_style);
    
}

FormatterPlugin::~FormatterPlugin() {
    if(m_formatter)
        delete m_formatter;
}

ContextMenuExtension FormatterPlugin::contextMenuExtension ( Context* context ) {
    Q_UNUSED(context);
    ContextMenuExtension ext;
    return ext ;
}

QString FormatterPlugin::name() {
    return "phpformatter";
}

QString FormatterPlugin::caption() {
    return "PHP Formatter";
}

QString FormatterPlugin::description() {
    return i18n ( "PHP Formatter" );
}

QString FormatterPlugin::highlightModeForMime ( const KMimeType::Ptr &mime ) {
    return "PHP";
}

QString FormatterPlugin::formatSource(const QString& text, const KUrl& url, const KMimeType::Ptr& mime, const QString& leftContext = QString(), const QString& rightContext = QString())
{
    if(m_formatter) {
        SourceFormatterStyle style = ICore::self()->sourceFormatterController()->styleForMimeType(mime);
        m_style = style;
        m_formatter->loadStyle(style.content());
        return m_formatter->formatSource(text, leftContext, rightContext);
    }
    return text;
}

QString FormatterPlugin::formatSourceWithStyle(SourceFormatterStyle style, const QString& text, const KUrl& url, const KMimeType::Ptr& mime, const QString& leftContext = QString(), const QString& rightContext = QString())
{
    if(m_formatter) {
        m_formatter->loadStyle(style.content());
        return m_formatter->formatSource(text, leftContext, rightContext);
    }
    return text;
}

QList< SourceFormatterStyle > FormatterPlugin::predefinedStyles()
{
    return m_styles;
}

void FormatterPlugin::setStyle(const KDevelop::SourceFormatterStyle& style)
{
    m_style = style;
}

SourceFormatterStyle FormatterPlugin::style() const
{
    return m_style;
}

KDevelop::SettingsWidget* FormatterPlugin::editStyleWidget ( const KMimeType::Ptr &mime ) {
    Q_UNUSED ( mime );
    return new FormatterPreferences(0);
}

QString FormatterPlugin::previewText ( const KMimeType::Ptr & ) {
    return  QString ("<?php\n") +
    "/** Multiple\n" +
    " *  comment */\n" +
    "function make3($i){\n" +
    "$i=$i%4;\n" +
    "\n" +
    "if($i==0) {\n" +
    "}elseif($i<3){\n" +
    "switch($i){\n" +
    "case 1:$i+=2; break;\n" +
    "case 2:$i=$i+1; break;\n" +
    "default:$i=3; break;\n" +
    "}\n" +
    "\n" +
    "} else { $i=$i&0|3;}\n" +
    "\n" +
    "//Single line comment\n" +
    "return $i;\n" +
    "}\n" +
    "\n" +
    "echo make3(3==3?3:33) . \"\\n\";\n" +
    "\n" +
    "for($i=0;$i<10;$i++) {echo $i;} \n" +
    "\n" +
    "while(feof($f)==false) {echo fgets($f);} \n" +
    "\n" +
    "class Test { \n" +
    "protected $member; \n" +
    "private $member2 = 1; \n" +
    "public function make2(array $args1, array $args2=array(), $args3=null) { \n" +
    "foreach($args1 as $key=>$val){ \n" +
    "echo $args2[$key.$val] . ' ' . $args3. \"\\n\"; \n" +
    "$args1[$key]= array(0=>$val, 1=>'done',); \n" +
    "} \n" +
    "} \n" +
    "public function otherFunc() {print __CLASS__;} \n" +
    "} \n" +
    "\n" +
    "?>";
}

/** \return The indentation of the style applicable for the given url.
 */
ISourceFormatter::Indentation FormatterPlugin::indentation(const KUrl& url)
{
  static Formatter *formatter = new Php::Formatter();
  formatter->loadStyle(m_style.content());

  ISourceFormatter::Indentation indent;
  indent.indentationTabWidth = formatter->options() ["indent_size"].toInt();
  indent.indentWidth = formatter->options() ["indent_size"].toInt();

  return indent;
}


#include "formatterplugin.moc"


