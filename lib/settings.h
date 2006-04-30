/***************************************************************************
    begin                : Sun Jul 24 2005
    copyright            : (C) 2005 by Jens Herden <jens@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License.               *
 *                                                                         *
 ***************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H

#include "settingsbase.h"
#include "quantaexport.h"


/** @short This class is a wrapper around the class settingsbase which gets generated from the config compiler.
  Here we add some methods to create a second instance for project settings.
  To get the global settings simply use this class, if you want the settings for the
  project call prjSettings()

    @author Jens Herden <jens@kdewebdev.org>
*/
class LIBQUANTA_EXPORT Settings : public SettingsBase
{
public:
  /**
   * @return global settings class
   */
  static Settings *self();

  ~Settings();

  /** this method should be used in places where project settings can exist independend
   *  from the global settings.
   *  @see createPrjSettings
   *
   *  @return the project settings or the global settings if no project settings where set
   */
  static Settings * prjSettings() {return (m_prjSettings ? m_prjSettings : self());}


  /** create a new project settings class.
   */
  static void createPrjSettings();

  /** deletes the current project settings class.
   */
  static void deletePrjSettings();

private:
  Settings(bool readOnly = false);

  static Settings * m_prjSettings;

  static Settings *m_Self;
};

#endif

// kate: space-indent on; indent-width 2; mixedindent off; indent-mode cstyle;
