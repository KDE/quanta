/*
<one line to give the program's name and a brief idea of what it does.>
Copyright (C) <year>  <name of author>

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

#include "stringreflist.h"
#include <QList>
#include <KDebug>

using namespace Xml;

StringRefStream::StringRefStream(const QString* content)
{
    m_content = content;
    m_list.append(QStringRef(m_content, 0, m_content->length()));
    m_length = m_content->length();
}

StringRefStream::~StringRefStream()
{

}

StringRefStream::Iterator StringRefStream::iterator() const
{
    Iterator iter;
    iter.m_index = 0;
    iter.m_offset = 0;
    iter.m_stream = this;
    return iter;
}

void StringRefStream::replace(Xml::StringRefStream::Iterator start, Xml::StringRefStream::Iterator end, const QStringRef& ref)
{
    if(start.m_index == end.m_index && start.m_offset == end.m_offset)
        return;
    for(int i = start.m_index+1; i < end.m_index;) {
        m_list.removeAt(i);
        end.m_index--;
    }
    QStringRef &left = m_list[start.m_index];
    QStringRef &right = m_list[end.m_index];
    if(end.m_index == start.m_index) {
        m_list.insert(end.m_index++, right);
        right = m_list[end.m_index];
    }
    m_list.insert(end.m_index++, ref);
    kDebug() << "left" << QStringRef(m_content, left.position(), start.m_offset).toString();
    m_list.replace(start.m_index, QStringRef(m_content, left.position(), start.m_offset));
    kDebug() << "right" << QStringRef(m_content, right.position() + end.m_offset+1, right.length() - end.m_offset-1).toString();
    m_list.replace(end.m_index, QStringRef(m_content, right.position() + end.m_offset+1, right.length() - end.m_offset));
    //m_list.replace(end.m_index, QStringRef(m_content, right.position() + end.m_offset, right.length() - end.m_offset));
}

void StringRefStream::replace(const QString& str, const QStringRef& ref)
{
    if(ref.string() != m_content)
        return;
    Iterator start = iterator();
    Iterator end = start;
    int match = 0;
    while(!end.eof()) {
        if(*end == str[match]) {
            end++;
            match++;
            if(match >= str.length()) {
                end--;
                replace(start, end, ref);
                end.m_index+=2; //jump over replacement
                end.m_offset = 0;
                start = end;
                match = 0;
                m_length += ref.length() - str.length();
            }
        } else {
            end++;
            start = end;
            match = 0;
        }
    }
}

QString StringRefStream::str() const
{
    QString ret;
    foreach(const QStringRef & ref, m_list) {
        if(!ref.isNull())
            ret += ref.toString();
    }
    return ret;
}
