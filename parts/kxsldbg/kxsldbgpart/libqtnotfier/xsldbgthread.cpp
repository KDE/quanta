/***************************************************************************
                          xsldbgthread.cpp  -  description
                             -------------------
    begin                : Thu Dec 20 2001
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


#include "config.h"
#include <pthread.h>            /* need to create/work with process thread */
#include <errno.h>              /* need for EAGAIN */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include <libxml/xmlerror.h>

#include "../libxsldbg/breakpoint.h"
#include "../libxsldbg/xsldbgmsg.h"
#include "../libxsldbg/xsldbgthread.h"

#ifdef HAVE_READLINE
#include <readline/readline.h>
#ifdef HAVE_HISTORY
#include <readline/history.h>
#endif
#endif

#define DEBUG_BUFFER_SIZE 500

static char inputBuffer[DEBUG_BUFFER_SIZE];
static char outputBuffer[DEBUG_BUFFER_SIZE];

/*the major structure to hold information about the process thread */
pthread_t mythread;

/* The reader for stdout */
pthread_t stdoutReaderThread;

FILE *stdoutIO = NULL;


/* -----------------------------------------------
   private functions
 ---------------------------------------------------*/
extern "C" {

/**
 * xsldbgGenericErrorFunc:
 * @ctx:  Is Valid
 * @msg:  Is valid
 * @...:  other parameters to use
 *
 * Handles print output from xsldbg and passes it to the application
 */
void
xsldbgGenericErrorFunc(void *ctx, const char *msg, ...);

}

/* -----------------------------------------------
   end functions
 ---------------------------------------------------*/

/* setup all application wide items */
int
xsldbgThreadInit(void)
{
    int result = 0;
    fprintf(stderr, "mainInit()\n");
    xsltSetGenericErrorFunc(0, xsldbgGenericErrorFunc);
    setThreadStatus(XSLDBG_MSG_THREAD_INIT);

    /* create the thread */
    if (pthread_create(&mythread, NULL, xsldbgThreadMain, NULL) != EAGAIN) {
      int counter;
      for (counter = 0; counter < 11; counter++){
	if (getThreadStatus() != XSLDBG_MSG_THREAD_INIT)
	  break;
	usleep(250000); /*guess that it will take at most 2.5 seconds to startup */
      }
      /* xsldbg should have started by now if it can */
      if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN){
        fprintf(stderr, "Created thread\n");
	result++;
      }else
         fprintf(stderr, "Thread did not start\n");
    } else {
      fprintf(stderr, "Failed to create thread\n");
    }

    return result;
}


/* tell the thread to stop and free that memory !*/
void
xsldbgThreadFree(void)
{
  fprintf(stderr, "xsldbgThreadFree()\n");
    if (getThreadStatus() != XSLDBG_MSG_THREAD_DEAD)
    {
      int counter;
      fprintf(stderr, "Killing xsldbg thread\n");
      setThreadStatus(XSLDBG_MSG_THREAD_STOP);
      for (counter = 0; counter < 11; counter++){
	if (getThreadStatus() == XSLDBG_MSG_THREAD_DEAD)
	  break;
	usleep(250000); /*guess that it will take at most 2.5 seconds to stop */
      }
    }

}

const char *getFakeInput()
{
	return inputBuffer;
}


/* put text into standard input just like we had typed it */
int
fakeInput(const char *text)
{
    int result = 0;

    if (!text || (getInputReady() == 1) || (getThreadStatus() != XSLDBG_MSG_THREAD_RUN))
        return result;

    //    fprintf(stderr, "\nFaking input of \"%s\"\n", text);
    strncpy(inputBuffer, text, sizeof(inputBuffer));
    setInputReady(1);
    result++;
    return result;
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

  const char *inputReadBuff;

  static char last_read[DEBUG_BUFFER_SIZE] = { '\0' };

  if (getThreadStatus() != XSLDBG_MSG_THREAD_RUN)
    {
#ifdef HAVE_READLINE
      xmlChar *line_read;

      /* Get a line from the user. */
      line_read = (xmlChar *) readline((char *) prompt);

      /* If the line has any text in it, save it on the history. */
      if (line_read && *line_read) {
        add_history((char *) line_read);
        strncpy((char*)last_read, (char*)line_read, DEBUG_BUFFER_SIZE - 1);
      } else {
        /* if only <Enter>is pressed then try last saved command line */
        line_read = (xmlChar *) xmlMemStrdup(last_read);
      }
      return (line_read);
#else
      char line_read[DEBUG_BUFFER_SIZE];

      if (prompt != NULL)
        xsltGenericError(xsltGenericErrorContext, "%s", prompt);
      if (!fgets(line_read, DEBUG_BUFFER_SIZE - 1, stdin))
        return (NULL);
      line_read[DEBUG_BUFFER_SIZE - 1] = 0;
      /* if only <Enter>is pressed then try last saved command line */
      if ((strlen(line_read) == 0) || (line_read[0] == '\n')) {
        strcpy(line_read, last_read);
      } else {
        strcpy(last_read, line_read);
      }
      return (xmlChar *) xmlMemStrdup(line_read);
#endif

    }
  else{

    setInputStatus(XSLDBG_MSG_AWAITING_INPUT);
    notifyXsldbgApp(XSLDBG_MSG_AWAITING_INPUT, NULL);

    while (getInputReady() == 0){
      usleep(10000);
      /* have we been told to die */
      if (getThreadStatus() ==  XSLDBG_MSG_THREAD_STOP){
	fprintf(stderr, "About to stop thread\n");
	xslDebugStatus = DEBUG_QUIT;
	return NULL;
      }
    }

    setInputStatus(XSLDBG_MSG_READ_INPUT);
    inputReadBuff =  getFakeInput();
    if(inputReadBuff){
      notifyXsldbgApp(XSLDBG_MSG_READ_INPUT, inputReadBuff);
      return (xmlChar*)xmlMemStrdup(inputReadBuff);
    }else{
      return NULL;
    }
  }
}


xsldbgErrorMsg msg;
xsldbgErrorMsgPtr  msgPtr = &msg;
xmlChar *msgText = NULL;

int notifyStateXsldbgApp(XsldbgMessageEnum type, int commandId,
			  XsldbgCommandStateEnum commandState, const char *text)
{
  int result = 0;
  msg.type = type;
  msg.commandId = commandId;
  msg.commandState = commandState;
  if (text != NULL)
    {
    msg.text = (xmlChar*)xmlMemStrdup(text);
    if (msg.text == NULL)
      return result; /* out of memory */
    }
   else
     msg.text = NULL;

  notifyXsldbgApp(XSLDBG_MSG_PROCESSING_RESULT, msgPtr);
  if (msg.text != NULL)
      {
	xmlFree(msg.text);
	msg.text = NULL;
      }

  result++; /* */
  return result;
}


int notifyTextXsldbgApp(XsldbgMessageEnum type, const char *text)
{
  return notifyStateXsldbgApp(type, -1, XSLDBG_COMMAND_NOTUSED, text);
}

char mainBuffer[DEBUG_BUFFER_SIZE];

/* this is where the thread get to do all its work */
void *
xsldbgThreadMain(void *data)
{
  int defaultArgc = 2;
  char *defaultArgv[2];
  int i;

  if (getThreadStatus() != XSLDBG_MSG_THREAD_INIT){
    fprintf(stderr, "xsldbg thread is not ready to be started. Or one is already running.\n");
    return NULL; /* we can't start more than one thread of xsldbg */
  }

  defaultArgv[0] = xmlMemStrdup("xsldbg");
  defaultArgv[1] = xmlMemStrdup("--shell");
  /*
  defaultArgv[2] = xmlMemStrdup("xsldoc.xsl");
  defaultArgv[3] = xmlMemStrdup("xsldoc.xml");
  */
  for (i = 0; i < defaultArgc; i++){
    if (defaultArgv[i] == NULL){
      fprintf(stderr, "Start thread failed. Unable to create xsldbg arguments\n");
      return NULL;
    }
  }

    setThreadStatus(XSLDBG_MSG_THREAD_RUN);
    setInputStatus(XSLDBG_MSG_AWAITING_INPUT);
    fprintf(stderr, "Starting thread\n");

    /* call the "main of xsldbg" found in debugXSL.c */
    xsldbgMain(defaultArgc, defaultArgv);
    fprintf(stderr, "Stopping thread\n");

  for (i = 0; i < defaultArgc; i++){
    xmlFree(defaultArgv[i]);
  }

    setThreadStatus(XSLDBG_MSG_THREAD_DEAD);
    setInputStatus(XSLDBG_MSG_PROCESSING_INPUT);
    notifyXsldbgApp(XSLDBG_MSG_THREAD_DEAD, NULL);
    return NULL;
}



/* thread has died so cleanup after it not called directly but via
 notifyXsldbgApp*/
void
xsldbgThreadCleanup(void)
{
    fprintf(stderr, "Thread has finished\n");
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN)
      {
	xsldbgThreadFree();
      }
    /* its safe to modify threadStatus as the thread is now dead */
    setThreadStatus(XSLDBG_MSG_THREAD_DEAD);
}



void *
xsldbgThreadStdoutReader(void *data)
{
  if (!stdoutIO)
    return data;

  while (getThreadStatus() == XSLDBG_MSG_THREAD_RUN){
    if (fgets(outputBuffer, sizeof(outputBuffer -1), stdoutIO)){
      usleep(10000);
      strcat(outputBuffer, "\n");
      notifyTextXsldbgApp(XSLDBG_MSG_TEXTOUT, outputBuffer);
    }else{
      fprintf(stderr, "Unable to read from stdout from xsldbg\n");
      break;
    }
  }
  return data;
}
