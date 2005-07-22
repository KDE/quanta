/*
 * Copyright (C) 2005 by Linus McCabe, <Linus@McCabe.nu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef qbytearrayfifo_H
#define qbytearrayfifo_H

#include <qstring.h>
#include <qcstring.h>

class QByteArrayFifo
{

  public:
    QByteArrayFifo();
    bool append(const char * chars, size_t size);
    QString retrieve();
    QString base64Encoded();
    long find(char character);
    size_t length() { return m_size; }

  private:
    QByteArray m_array;
    size_t m_size;

};

#endif // qbytearrayfifo_H
