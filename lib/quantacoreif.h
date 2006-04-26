/***************************************************************************
    begin                : Mon Jun 20 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef QUANTACOREIF_H
#define QUANTACOREIF_H

#include "helper.h"

#include <kdevplugin.h>

class QPoint;

class ParseResult;
class AreaStruct;
class EditorSource;
class KUrl;
class TagPair;

/**
@short The interface class for the Quanta Core plugin. 

The Quanta Core plugin is a service provider for the other plugins.
It should always be loaded. 
You can get a pointer to the implementation if you call this in a plugin:

@code
QuantaCoreIf * qcore = extension<QuantaCoreIf>("KDevelop/Quanta");
@endcode
   
There is a global Settings class. All Quanta specific plugins share this class.
It is KConfig XT based so you can easily add an entry to quanta.kcfg,
but make sure to use a group per plugin.

This lines will give you the global Settings class:
@code
#include "settings.h"

Settings * s = Settings::self();

@endcode

@author Jens Herden
*/

class QuantaCoreIf : public KDevPlugin
{
  Q_OBJECT

public:
  QuantaCoreIf(const KDevPluginInfo * info, QObject *parent);

  ~QuantaCoreIf();

  /**
   * Inserts a new tag into the current document.
   * @param tagName The full opening tag string
   * @param closingTag The full closing tag string
   * @param inLine if true, the tag and the closing tag will be in the same line. Otherwise they will be in different lines.
   * @param showDialog if true, the tag dialog for the tagName is shown
   */
  virtual void insertTag(const TagPair & tagPair, bool inLine, bool showDialog) = 0;
  
  /**
   * 
   * @return the current active @see EditorSource
   */
  virtual EditorSource * activeEditorSource() const = 0;
  
  /**
   * Find the nick name of a DTEP associated with a name.
   * @param name the name (id) of the DTEP
   * @return the nickname of the DTEP or an empty string if no such DTEP was loaded
   */
  virtual QString getDTEPNickName(const QString &name) const = 0;
signals:
  /**
   * emitted when the current document is about to get parsed
   */
  void startParsing();
  
  /**
   * emitted when the current document was parsed
   * 
   * @param parseResult the result of the parsing or 0 if the current doc was closed
   */
  void finishedParsing(const ParseResult *parseResult);

  /**
   * emitted when some groups in the current document were parsed
   * 
   * @param parseResult the result of the parsing
   */
  void groupsParsed(const ParseResult *parseResult);

  /**
   * emitted when the cursor position has changed and the idle timer expired
   *
   * @param qp qp.x() = line and qp.y() = column
   */
  void newCursorPosition(const QPoint & qp);
  
public slots:
  
  /** insert <img> tag for images or <a> for other
   *  
   * @param url the url to insert, will be converted to relative
   * @param dirInfo optional information for pre and post text
   */
  virtual void slotInsertTag(const KUrl & url, Helper::DirInfo * dirInfo = 0) = 0;

};

#endif
