
/***************************************************************************
                          simpleio.c  - use console io only
                             -------------------
    begin                : Sun Dec 23 2001
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

#include "kxsldbgpart/libxsldbg/xsldbg.h"

#include <libxslt/xsltutils.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#ifdef HAVE_HISTORY
#include <readline/history.h>
#endif
#endif

#include <string.h>

#include "kxsldbgpart/libxsldbg/xsldbgmsg.h"
#include "kxsldbgpart/libxsldbg/xsldbgio.h"
#include "kxsldbgpart/libxsldbg/options.h"


int notifyXsldbgApp(XsldbgMessageEnum type, const void *data)
{
  return 1;
}

int notifyStateXsldbgApp(XsldbgMessageEnum type, int commandId,
			   XsldbgCommandStateEnum commandState, const char *text)
{
  return 1;
}

int notifyTextXsldbgApp(XsldbgMessageEnum type, const char *text)
{
  return 1;
}


/* use this function instead of the one that was in debugXSL.c */
/**
 * xslShellReadline:
 * @prompt:  the prompt value
 *
 * Read a string
 *
 * Returns a copy of the text inputed or NULL if EOF in stdin found.
 *    The caller is expected to free the returned string.
 */
xmlChar *
xslDbgShellReadline(xmlChar * prompt)
{

  static char last_read[DEBUG_BUFFER_SIZE] = { '\0' };

#ifdef HAVE_READLINE

      xmlChar *line_read;

      if (optionsGetIntOption(OPTIONS_STDOUT) == 0){
	/* Get a line from the user. */
	line_read = (xmlChar *) readline((char *) prompt);

	/* If the line has any text in it, save it on the history. */
	if (line_read && *line_read) {
	  char *temp = (char*)line_read;
	  add_history((char *) line_read);
	  strncpy((char*)last_read, (char*)line_read, DEBUG_BUFFER_SIZE - 1);
	  /* we must ensure that the data is free properly */
	  line_read = xmlStrdup((xmlChar*)line_read);
	  free(temp);
	} else {
	  free(line_read);
	  /* if only <Enter>is pressed then try last saved command line */
	  line_read = xmlStrdup((xmlChar*)last_read);
	}
      }else{
	/* readline library will/may  echo its output which is not wanted
	   when running in gdb mode.*/
	char line_buffer[DEBUG_BUFFER_SIZE];

	if (prompt != NULL)
	  xsltGenericError(xsltGenericErrorContext, "%s", prompt);
	if (!fgets(line_buffer, sizeof(line_buffer) - 1, stdin)){
	  line_read = NULL;
	}else{
	  line_buffer[DEBUG_BUFFER_SIZE - 1] = 0;
	  if ((strlen(line_buffer) == 0) || (line_buffer[0] == '\n')){
	    line_read = xmlStrdup((xmlChar*)last_read);
	  }else{
	    add_history((char *) line_buffer);
	    line_read = xmlStrdup((xmlChar*)line_buffer);
	    strncpy((char*)last_read, (char*)line_read, sizeof(last_read) - 1);	  	  }
	}

      }
      return (line_read);

#else
      char line_read[DEBUG_BUFFER_SIZE];

      if (prompt != NULL)
        xsltGenericError(xsltGenericErrorContext, "%s", prompt);
	  fflush(stderr);
      if (!fgets(line_read, DEBUG_BUFFER_SIZE - 1, stdin))
        return (NULL);
      line_read[DEBUG_BUFFER_SIZE - 1] = 0;
      /* Repeat of last command when gdb mode is disabled */
      if (optionsGetIntOption(OPTIONS_GDB) == 0){
	/* if only <Enter>is pressed then try last saved command line */
	if ((strlen(line_read) == 0) || (line_read[0] == '\n')) {
	  strncpy(line_read, last_read, sizeof(line_read) - 1);
	} else {
	  strcpy(last_read, line_read);
	}
      }
      return xmlStrdup((xmlChar*)line_read);
#endif

    }

void xsldbgThreadCleanup(void);

/* thread has died so cleanup after it not called directly but via
 notifyXsldbgApp*/
void
xsldbgThreadCleanup(void)
{

}
