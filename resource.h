/***************************************************************************
                          resource.h  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon
    email                : pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef RESOURCE_H
#define RESOURCE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

class QStrList;

extern QStrList *tagsList; // list of known tags
extern QStrList *tagsCore; // list of tags, allowed core attributes ( id, class, id, title )
extern QStrList *tagsI18n; // list of tags, allowed i18 attribs.
extern QStrList *tagsScript; // list of tags, allowed script attribs ( onClicl, omMouseOver... )
extern QStrList *quotedAttribs; // list of attribs, that have quoted values ( alt, src ... )
extern QStrList *lCore;          // list of core attributes ( id, class, style ... )
extern QStrList *lI18n;
extern QStrList *lScript;
extern QStrList *singleTags; // tags without end  part </ >
extern QStrList *optionalTags; // tags with optional end part

extern bool tagsCapital; // use capital for tags
extern bool attrCapital; // use capital letters for attributes of tag
extern bool useCloseTag; // use close tag if optional

extern QString fileMaskHtml;
extern QString fileMaskPhp;
extern QString fileMaskJava;
extern QString fileMaskText;
extern QString fileMaskImage;

#endif // RESOURCE_H
