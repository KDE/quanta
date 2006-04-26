/***************************************************************************
    begin                : Mon Jul 4 2005
    copyright            : (C) 2005 by Andras Mantia <amantia@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef TAGDIALOGSIF_H
#define TAGDIALOGSIF_H

#include "tagpair.h"

#include <kdevplugin.h>
#include <kurl.h>

// class EditorSource;
class QTag;

/**
@short The interface class for the TagDialogs plugin. 

The Tag Dialogs plugin offers dialogs to edit the tags.
You can get a pointer to the implementation if you call this in a plugin:

@code
TagDialogsIf * plugin = extension<TagDialogsIf>("KDevelop/TagDialogs");
@endcode
   
@author Andras Mantia
*/


class TagDialogsIf : public KDevPlugin
{
  Q_OBJECT

public:
  TagDialogsIf(const KDevPluginInfo * info, QObject *parent);
  ~TagDialogsIf();

   virtual TagPair createNewTag(QTag *dtdTag, const QString& selection, const QString& attrs = QString::null, const KUrl& baseURL = KUrl()) = 0;

};

#endif
