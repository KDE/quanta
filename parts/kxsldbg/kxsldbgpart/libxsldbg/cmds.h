
/***************************************************************************
                          cmds.h  -  A list of valid commands ID's for xsldbg
                             -------------------
    begin                : Thu Dec 27 2001
    copyright            : (C) 2001 by keith
    email                : keith@linux
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef CMDS_H
#define CMDS_H

/* See the top of debugXSL.c for a list of command names and thier shortcuts */
#ifndef USE_KDE_DOCS
typedef enum {                  /* id's for commands of xslDbgShell */
    DEBUG_HELP_CMD = 100,
    DEBUG_BYE_CMD,
    DEBUG_EXIT_CMD,
    DEBUG_QUIT_CMD,

    DEBUG_STEP_CMD,
    DEBUG_STEPUP_CMD,
    DEBUG_STEPDOWN_CMD,
    DEBUG_NEXT_CMD,
    DEBUG_CONT_CMD,
    DEBUG_RUN_CMD,

    DEBUG_TEMPLATES_CMD,
    DEBUG_WHERE_CMD,
    DEBUG_FRAME_CMD,
    DEBUG_STYLESHEETS_CMD,

    DEBUG_BREAK_CMD,
    DEBUG_SHOWBREAK_CMD,
    DEBUG_DELETE_CMD,
    DEBUG_ENABLE_CMD,
    DEBUG_DISABLE_CMD,

    DEBUG_LS_CMD,
    DEBUG_DIR_CMD,
    DEBUG_DU_CMD,
    DEBUG_CAT_CMD,
    DEBUG_PRINT_CMD,            /* cat alternative */
    DEBUG_PWD_CMD,
    DEBUG_DUMP_CMD,
    DEBUG_BASE_CMD,

    DEBUG_GLOBALS_CMD,
    DEBUG_LOCALS_CMD,
    /*     DEBUG_CAT_CMD, already listed */
    DEBUG_SOURCE_CMD,
    DEBUG_DATA_CMD,
    DEBUG_OUTPUT_CMD,
    DEBUG_CD_CMD,

    /* file related */
    /*    DEBUG_OUTPUT_CMD, already listed */
    DEBUG_ENTITIES_CMD,
    DEBUG_SYSTEM_CMD,
    DEBUG_PUBLIC_CMD,
    DEBUG_ENCODING_CMD,
    DEBUG_VALIDATE_CMD,
    DEBUG_LOAD_CMD,
    DEBUG_SAVE_CMD,
    DEBUG_WRITE_CMD,
    DEBUG_FREE_CMD,

    /* Operating system related */
    DEBUG_CHDIR_CMD,
    DEBUG_SHELL_EXEC_CMD,
    DEBUG_TTY_CMD,

    /* libxslt parameter and options related */
    DEBUG_ADDPARAM_CMD,
    DEBUG_DELPARAM_CMD,
    DEBUG_SHOWPARAM_CMD,
    DEBUG_SETOPTION_CMD,
    DEBUG_OPTIONS_CMD,          /* print options */

    /* extra options/commands */
    DEBUG_TRACE_CMD,
    DEBUG_WALK_CMD,
    DEBUG_ADDWATCH_CMD,
    DEBUG_DELWATCH_CMD,
    DEBUG_SHOWWATCH_CMD,

    /* searching */
    DEBUG_SEARCH_CMD,

    /* variable change */
    DEBUG_SET_CMD
        /* NULL */
} CommandsEnum;
#else

/* Keep kdoc happy*/
enum CommandsEnum {             /* id's for commands of xslDbgShell */
    DEBUG_HELP_CMD = 100,
    DEBUG_BYE_CMD,
    DEBUG_EXIT_CMD,
    DEBUG_QUIT_CMD,

    DEBUG_STEP_CMD,
    DEBUG_STEPUP_CMD,
    DEBUG_STEPDOWN_CMD,
    DEBUG_NEXT_CMD,
    DEBUG_CONT_CMD,
    DEBUG_RUN_CMD,

    DEBUG_TEMPLATES_CMD,
    DEBUG_WHERE_CMD,
    DEBUG_FRAME_CMD,
    DEBUG_STYLESHEETS_CMD,

    DEBUG_BREAK_CMD,
    DEBUG_SHOWBREAK_CMD,
    DEBUG_DELETE_CMD,
    DEBUG_ENABLE_CMD,
    DEBUG_DISABLE_CMD,

    DEBUG_LS_CMD,
    DEBUG_DIR_CMD,
    DEBUG_DU_CMD,
    DEBUG_CAT_CMD,
    DEBUG_PRINT_CMD,            /* cat alternative */
    DEBUG_PWD_CMD,
    DEBUG_DUMP_CMD,
    DEBUG_BASE_CMD,

    DEBUG_GLOBALS_CMD,
    DEBUG_LOCALS_CMD,
    /*     DEBUG_CAT_CMD, already listed */
    DEBUG_SOURCE_CMD,
    DEBUG_DATA_CMD,
    DEBUG_OUTPUT_CMD,
    DEBUG_CD_CMD,

    /* file related */
    /*    DEBUG_OUTPUT_CMD, already listed */
    DEBUG_ENTITIES_CMD,
    DEBUG_SYSTEM_CMD,
    DEBUG_PUBLIC_CMD,
    DEBUG_ENCODING_CMD,
    DEBUG_VALIDATE_CMD,
    DEBUG_LOAD_CMD,
    DEBUG_SAVE_CMD,
    DEBUG_WRITE_CMD,
    DEBUG_FREE_CMD,

    /* Operating system related */
    DEBUG_CHDIR_CMD,
    DEBUG_SHELL_EXEC_CMD,
    DEBUG_TTY_CMD,

    /* libxslt parameter and options related */
    DEBUG_ADDPARAM_CMD,
    DEBUG_DELPARAM_CMD,
    DEBUG_SHOWPARAM_CMD,
    DEBUG_SETOPTION_CMD,
    DEBUG_OPTIONS_CMD,          /* print options */

    /* extra options/commands */
    DEBUG_TRACE_CMD,
    DEBUG_WALK_CMD,
    DEBUG_ADDWATCH_CMD,
    DEBUG_DELWATCH_CMD,
    DEBUG_SHOWWATCH_CMD,

    /* searching */
    DEBUG_SEARCH_CMD,

    /* variable change */
    DEBUG_SET_CMD
        /* NULL */
};
#endif

#endif
