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

#ifndef KDEVSGMLFORMATTERPLUGIN_H_
#define KDEVSGMLFORMATTERPLUGIN_H_

#include <QtCore/QObject>

#include <KDE/KAction>

#include <interfaces/iplugin.h>
#include <interfaces/isourceformatter.h>
#include <interfaces/contextmenuextension.h>


namespace Php {

class FormatterPreferences;
class Formatter;
    
class FormatterPlugin : public KDevelop::IPlugin, public KDevelop::ISourceFormatter {
    Q_OBJECT
    Q_INTERFACES ( KDevelop::ISourceFormatter )

public:
    explicit FormatterPlugin ( QObject *parent, const QVariantList & = QVariantList() );
    ~FormatterPlugin();

    virtual KDevelop::ContextMenuExtension contextMenuExtension ( KDevelop::Context* context );

    virtual QString name();
    virtual QString caption();
    virtual QString description();

    virtual QString highlightModeForMime ( const KMimeType::Ptr &mime );

    /** Formats using the current style.
    */
    virtual QString formatSource(const QString& text, const KUrl& url, const KMimeType::Ptr& mime, const QString& leftContext, const QString& rightContext);

    virtual QString formatSourceWithStyle(KDevelop::SourceFormatterStyle, const QString& text, const KUrl& url, const KMimeType::Ptr& mime, const QString& leftContext, const QString& rightContext);

    /** \return The widget to edit a style.
    */
    virtual KDevelop::SettingsWidget* editStyleWidget ( const KMimeType::Ptr &mime );

    /** \return The text used in the config dialog to preview the current style.
    */
    virtual QString previewText ( const KMimeType::Ptr &mime );

    /** \return The indentation type of the currently selected style.
    */
    virtual IndentationType indentationType();

    /** \return The number of spaces used for indentation if IndentWithSpaces is used,
    * or the number of spaces per tab if IndentWithTabs is selected.
    */
    virtual int indentationLength();
    
    virtual QList< KDevelop::SourceFormatterStyle > predefinedStyles();
    virtual void setStyle(const KDevelop::SourceFormatterStyle& );
    virtual KDevelop::SourceFormatterStyle style() const;

protected:
    KDevelop::SourceFormatterStyle m_style;
    QList<KDevelop::SourceFormatterStyle> m_styles;
    Formatter * m_formatter;
};

}
#endif //KDEVSGMLFORMATTERPLUGIN_H_
