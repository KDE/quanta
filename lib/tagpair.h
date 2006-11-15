/***************************************************************************
    begin                : Fri Jul 15 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef TAGPAIR_H
#define TAGPAIR_H

#include "tag.h"
#include "tagattr.h"
#include "quantaexport.h"

#include <QString>
#include <QPair>

/**
pair of opening and closing parts of a tag

@author Jens Herden
*/
class KDEVQUANTA_EXPORT TagPair : QPair<QString, QString> {
public:
  TagPair();

  inline TagPair(const QString & opening, const QString & closing) {first = opening; second = closing;};

  ~TagPair();

  inline QString createTag(const QString & middle) const {return first + middle + second;};

  inline QString & opening() {return first;};
  inline QString & closing() {return second;};

  inline const QString & opening() const {return first;};
  inline const QString & closing() const {return second;};

  /**
   * sets the opening of the TagPair and adds < > or < /> around
   * @param tag new value
   * @param isSingle true = add "/>" and delete closing
   */
  inline void setOpening(const QString &tag, bool isSingle = false)
    {
      first = TagPair::decorateOpening(tag, isSingle);
      if (isSingle)
        second = "";
    };

  /**
   * sets the closing of the TagPair and adds </ > around
   * @param tag new value
   */
  inline void setClosing(const QString &tag)
    {
      second = decorateClosing(tag);
    };

  static inline QString decorateOpening(const QString &tag, bool isSingle)
    {return tag.trimmed().prepend("<").append(isSingle ? " />" : ">");};

  static inline QString decorateClosing(const QString &tag)
    {return tag.trimmed().prepend("</").append(">");};

  /**
   * creates a link tag from a reference
   * @param ref the url
   * @return a TagPair with <a href="ref"> </a>
   */
  static TagPair createLinkTag(const QString & ref)
    {return TagPair(Tag::convertCase("<a ") + TagAttr::convertCase("href=") + TagAttr::quoteAttributeValue(ref) + ">", Tag::convertCase("</a>"));};
};
#endif
