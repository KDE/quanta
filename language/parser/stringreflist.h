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

#ifndef STRINGREFTREE_H
#define STRINGREFTREE_H

#include <QtCore/QString>
#include <QtCore/QList>

namespace Xml {

class StringRef;

/** An iteratable list of string references.
 *  This class is mainly used for parsing DTD. DTD have parameter entities which text may be referenced
 *  anywhere in the source:<br/>
 *  <pre>
 *  <!entity % some.string "some | string">
 *  <!element some (can, have, (%some.string;))>
 *  </pre><br/>
 *  In the exmaple above a string reference will be kept to "some | string" in the place of "%some.string;".
 */
class StringRefStream
{
    friend class Iterator;
public:
    class Iterator {
        friend class StringRefStream;
    public:
        Iterator & operator -= (qint64 count) {
            if (count < m_offset) {
                m_offset -= count;
                return *this;
            }
            count -= m_offset;
            m_index--;
            while (m_index > -1 && count > m_stream->m_list[m_index].length()) {
                count -= m_stream->m_list[m_index].length();
                m_index--;
            }
            m_offset = m_stream->m_list[m_index].length() - count;
        }
        inline Iterator & operator += (qint64 count) {
            if (eof()) return *this;
            if (m_offset + count < m_stream->m_list[m_index].length()) {
                m_offset += count;
                return *this;
            }
            count -= m_stream->m_list[m_index].length() - m_offset;
            m_index++;
            while (!eof() && count > m_stream->m_list[m_index].length()) {
                count -= m_stream->m_list[m_index].length();
                m_index++;
            }
            m_offset = count;
        }
        inline Iterator & operator ++ (int) {
            return (*this)+=1;
        }
        inline Iterator & operator -- (int) {
            return (*this)-=1;
        }
        inline const QChar & operator -> () {
            static const QChar nullChar = QChar::Null;
            if (eof()) return nullChar;
            return *(m_stream->m_list[m_index].data() + offset());
        }
        inline const QChar & operator * () {
            static const QChar nullChar = QChar::Null;
            if (eof()) return nullChar;
            return *(m_stream->m_list[m_index].data() + offset());
        }
        inline void reset() {
            m_index = 0;
            m_offset = 0;
        }
        inline qint64 offset() {
            return m_offset;
        }

        inline bool eof() {
            return m_index >= m_stream->m_list.length()
                || (m_index == m_stream->m_list.length() - 1 && m_offset >= m_stream->m_list.last().length());
        }
    protected:
        const StringRefStream *m_stream;
        int m_index;
        qint64 m_offset;
    };

    StringRefStream(const QString* content);
    virtual ~StringRefStream();

    Iterator iterator() const;

    inline const QString* content() const {
        return m_content;
    }

    void replace(const QString &str, const QStringRef &ref);

    inline qint64 length() const {
        return m_length;
    }

    int referenceCount() const {
        return m_list.size();
    }

    QString str() const;
protected:
    void replace(Iterator start,  Iterator end, const QStringRef &ref);
    qint64 m_length;
    QList<QStringRef> m_list;
    const QString* m_content;
};
}

#endif // STRINGREFTREE_H
