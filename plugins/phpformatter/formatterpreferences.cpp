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

#include "formatterpreferences.h"
#include "formatter.h"

#include <KDE/KDebug>

using namespace Php;

FormatterPreferences::FormatterPreferences ( QWidget* parent, Qt::WindowFlags f )
        : KDevelop::SettingsWidget ( parent ) {
    Q_UNUSED ( f );
    setupUi ( this );
    m_formatter = new Formatter;
    optionList->setSortingEnabled(false);
    connect(optionList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*)));
    connect(indentSizeSbx, SIGNAL(valueChanged(int)), this, SLOT(indentChanged(int)));
}

FormatterPreferences::~FormatterPreferences() {
    if ( m_formatter )
        delete m_formatter;
}

void FormatterPreferences::load(const KDevelop::SourceFormatterStyle& style)
{
    m_options = Formatter::stringToOptionMap(style.content());
    if(!m_options.contains("indent_size"))
        m_options.insert("indent_size", "4");
    if(!m_options.contains("keep_redundant_lines"))
        m_options.insert("keep_redundant_lines", true);
    if(!m_options.contains("space_after_if"))
        m_options.insert("space_after_if", true);
    if(!m_options.contains("else_along_curly"))
        m_options.insert("else_along_curly", true);
    
    m_formatter->loadStyle(m_options);
    optionList->clear();
    
    typedef QPair<QString, QString> Pair;
    QList< Pair > list;
    list.append(Pair("Indentation and General Formatting:",""));
    list.append(Pair("indent_with_tabs","Indent with tabs"));
    list.append(Pair("keep_redundant_lines","Keep reduntant lines"));
    list.append(Pair("space_inside_parentheses","Space inside parentheses"));
    list.append(Pair("space_outside_parentheses","Space outside parentheses"));
    list.append(Pair("space_after_comma","Space after comma"));
    list.append(Pair("Operators:",""));
    list.append(Pair("space_around_assignment","Space around assignment"));
    list.append(Pair("align_var_assignment","Align variable assignment"));
    list.append(Pair("space_around_comparison","Space around comparison"));
    list.append(Pair("space_around_arithmetic","Space around arithmetic"));
    list.append(Pair("space_around_logical","Space around logical"));
    list.append(Pair("space_around_colon_question","Space around colon question"));
    list.append(Pair("Functions, Classes and Objects:",""));
    list.append(Pair("line_before_function","Line before function"));
    list.append(Pair("line_before_curly_function","Line before curly function"));
    list.append(Pair("line_after_curly_function","Line after curly function"));
    list.append(Pair("space_around_obj_operator","Space around object operator"));
    list.append(Pair("space_around_double_colon","Space around double colon"));
    list.append(Pair("Control Structures:",""));
    list.append(Pair("space_after_if","Space after if"));
    list.append(Pair("else_along_curly","Else along curly"));
    list.append(Pair("line_before_curly","Line before curly"));
    list.append(Pair("add_missing_braces","Add missing braces"));
    list.append(Pair("line_after_break","Line after break"));
    list.append(Pair("space_inside_for","Space inside for"));
    list.append(Pair("indent_case","Indent case"));
    list.append(Pair("Arrays and Concatenation:",""));
    list.append(Pair("line_before_array","Line before array"));
    list.append(Pair("vertical_array","Vertical array"));
    list.append(Pair("align_array_assignment","Align array assignment"));
    list.append(Pair("space_around_double_arrow","Space around double arrow"));
    list.append(Pair("vertical_concat","Vertical concat"));
    list.append(Pair("space_around_concat","Space around concat"));
    list.append(Pair("Comments:",""));
    list.append(Pair("line_before_comment_multi","Line before multiline comment"));
    list.append(Pair("line_after_comment_multi","Line after multiline comment"));
    list.append(Pair("line_before_comment","Line before comment"));
    list.append(Pair("line_after_comment","Line after comment"));

    foreach( const Pair &p, list) {
        QListWidgetItem * item = 0;
        if(!p.second.isEmpty()) {
            item = new QListWidgetItem(p.second);
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
            item->setData(Qt::UserRole, p.first);
            item->setCheckState(Qt::Unchecked);
            if(m_options.contains(p.first)
                && m_options.value(p.first).isValid()
                && m_options.value(p.first).toBool() == true) {
                item->setCheckState(Qt::Checked);
            }
        } else {
            item = new QListWidgetItem(p.first);
            item->setFlags(Qt::ItemIsEnabled);
            QFont font = item->font();
            font.setBold(true);
            item->setFont(font);
            item->setData(Qt::UserRole, "");
        }
        optionList->addItem(item);
    }

    indentSizeSbx->setValue(m_options.value("indent_size").toInt());

    updateText();
}

QString FormatterPreferences::save() {
    return KDevelop::ISourceFormatter::optionMapToString(m_options);
}

QMap< QString, QVariant > FormatterPreferences::getOptions() {
    return m_options;
}

void FormatterPreferences::indentChanged ( int i )
{
    m_options.insert ( "indent_size", QVariant ( i ) );
    updateText ( );
}

void FormatterPreferences::updateText ( ) {
    QString text = QString ("<?php\n") +
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
    "?>";
    m_formatter->loadStyle(m_options);
    emit previewTextChanged ( m_formatter->formatSource ( text ) );
}

void FormatterPreferences::itemChanged(QListWidgetItem* item)
{
    if(!item->data(Qt::UserRole).isValid())
        return;
    kDebug() << item->data(Qt::UserRole).toString();
    if(item->checkState() == Qt::Checked) {
        m_options.insert(item->data(Qt::UserRole).toString(), true);
    } else
        m_options.insert(item->data(Qt::UserRole).toString(), false);
    updateText ( );
}

#include "formatterpreferences.moc"
