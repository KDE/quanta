
/***************************************************************************
                          help.h  -  describe the help support functions
                             -------------------
    begin                : Sun Sep 16 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef XSLDBG_HELP_H
#define XSLDBG_HELP_H

#ifdef USE_KDE_DOCS

/**
 * Provide a help system for user 
 *
 * @short help system support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
#endif

#ifdef __cplusplus
extern "C" {
#endif


#ifdef USE_GNOME_DOCS

/**
 * helpTop:
 * @args : Is valid command or empty string
 *
 * Display help about the command in @arg
 *
 * This is a platform specific interface
 *
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Display help about the command in @p arg
 *
 *
 *  This is a platform specific interface
 *
 * @param args Is valid or empty string
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int helpTop(const xmlChar * args);


#ifdef __cplusplus
}
#endif
#endif
