/***************************************************************************
                          resource.h  -  description
                             -------------------
    begin                : Втр Май  9 13:29:57 EEST 2000
    copyright            : (C) 2000 by Dmitry Poplavsky & Alexander Yakovlev & Eric Laffoon <pdima@users.sourceforge.net,yshurik@linuxfan.com,sequitur@easystreet.com>
                           (C) 2001-2003 Andras Mantia <amantia@kde.org>
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


class KDirWatch;
class KProgress;
class QuantaApp;
class QStringList;
class KTempFile;
class KTempDir;

class Node;
class Parser;
struct QConfig;
struct DTDStruct;


extern QConfig qConfig;
extern QString tmpDir;

extern QRegExp scriptBeginRx;
extern QRegExp scriptEndRx;

extern Node *baseNode;
extern Parser *parser;

extern KDirWatch *fileWatcher;

extern QString toolbarExtension;
extern QRegExp newLineRx;
extern QStringList charList; //hold the list of the &char; chars. See the data/chars file.
extern QPtrList<KTempFile> tempFileList;
extern QPtrList<KTempDir> tempDirList;

extern KProgress *progressBar;
extern QuantaApp *quantaApp;
extern const QString resourceDir;
extern bool typingInProgress;

extern int nodeNum;

#endif // RESOURCE_H
