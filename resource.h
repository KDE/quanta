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

#include "parser/qtag.h"
#include "parser/node.h"
#include "parser/parser.h"

class KDirWatch;

extern QDict<DTDStruct> *dtds;

extern uint tagsCase; // use capital for tags
extern uint attrsCase; // use capital letters for attributes of tag
extern QString attrsQuotation;
extern bool useAutoCompletion; //enable auto completion of tags
extern bool closeOptionalTags;
extern bool closeTags;
extern bool useMimeTypes;
extern uint refreshFrequency;
extern QString defaultDocType;

extern QString fileMaskHtml;
extern QString fileMaskPhp;
extern QString fileMaskJava;
extern QString fileMaskText;
extern QString fileMaskImage;
extern Node *baseNode;
extern Parser *parser;
extern QString defaultEncoding;
extern QValueList<QString> pluginsList; //holds the names of available plugins
extern KDirWatch *fileWatcher;


#endif // RESOURCE_H
