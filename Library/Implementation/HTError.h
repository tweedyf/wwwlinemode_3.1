/*                                                            Error message module for libwww
                       REPORTING ERRORS AND MESSAGES TO THE CLIENT
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module maintaines a list of error messages that might occur during load of a
   requested URL. The error list is put out to standard output by a simple function that
   easily can be overwritten by a smart server or client.
   
   Note:  At the moment, HTErrorMsg() is called, if the flag HTRequest->error_block is set
   to YES, then a stream has been put up or taken down in the library and therefore it is
   VERY unsafe to put anything more to the stream.
   
   This module is implemented by HTError.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTERROR_H
#define HTERROR_H

#include "HTAccess.h"/*

Data Structures

   The basic data structure is HTErrorInfo, but in addition the following types are used:
   
  ERROR NUMBERS
  
   Note:  All non-HTTP error codes have index numbers > HTERR_HTTP_CODES, and they will
   not be shown in the error-message generated.
   
 */
typedef enum _HTErrorElement {
        HTERR_OK = 0,                                           /* 200 */
        HTERR_CREATED,                                          /* 201 */
        HTERR_ACCEPTED,                                         /* 202 */
        HTERR_PARTIAL,                                          /* 203 */
        HTERR_NO_CONTENT,                                       /* 204 */
        HTERR_MOVED,                                            /* 301 */
        HTERR_FOUND,                                            /* 302 */
        HTERR_METHOD,                                           /* 303 */
        HTERR_NOT_MODIFIED,                                     /* 304 */
        HTERR_BAD_REQUEST,                                      /* 400 */
        HTERR_UNAUTHORIZED,                                     /* 401 */
        HTERR_PAYMENT_REQUIRED,                                 /* 402 */
        HTERR_FORBIDDEN,                                        /* 403 */
        HTERR_NOT_FOUND,                                        /* 404 */
        HTERR_NOT_ALLOWED,                                      /* 405 */
        HTERR_NONE_ACCEPTABLE,                                  /* 406 */
        HTERR_PROXY,                                            /* 407 */
        HTERR_TIMEOUT,                                          /* 408 */
        HTERR_INTERNAL,                                         /* 500 */
        HTERR_NOT_IMPLEMENTED,                                  /* 501 */
        HTERR_BAD_GATE,                                         /* 502 */
        HTERR_DOWN,                                             /* 503 */
        HTERR_GATE_TIMEOUT,                                     /* 504 */
        HTERR_HTTP_CODES_END,    /* Put all non-HTTP status codes after this */
        HTERR_NO_REMOTE_HOST,
        HTERR_NO_HOST,
        HTERR_FTP_SERVER,
        HTERR_FTP_NO_RESPONSE,
        HTERR_TIME_OUT,
        HTERR_GOPHER_SERVER,
        HTERR_INTERRUPTED,
        HTERR_CON_INTR,
        HTERR_CSO_SERVER,
        HTERR_HTTP09,
        HTERR_BAD_REPLY,
        HTERR_UNKNOWN_AA,
        HTERR_NEWS_SERVER,
        HTERR_FILE_TO_FTP,
        HTERR_MAX_REDIRECT,
        HTERR_EOF,
        HTERR_WAIS_OVERFLOW,
        HTERR_WAIS_MODULE,
        HTERR_WAIS_NO_CONNECT,
        HTERR_SYSTEM,
        HTERR_ELEMENTS                      /* This MUST be the last element */
} HTErrorElement;

typedef enum _HTErrSeverity {
    ERR_FATAL             = 0x1,
    ERR_NON_FATAL         = 0x2,
    ERR_WARN              = 0x4,
    ERR_INFO              = 0x8
} HTErrSeverity;

typedef struct _HTErrorInfo {
    int                 handle;         /* Unique number in this stack */
    HTErrorElement      element;        /* Index number into HTErrorMsgInfo */
    HTErrSeverity       severity;       /* A la VMS */
    BOOL                ignore;         /* YES if msg should not go to user */
    void *              par;            /* Explanation, e.g. filename  */
    unsigned int        par_length;     /* For copying by generic routine */
    char *              where;          /* Which function */
} HTErrorInfo;/*

Controling Globals

   This variable dictates which errors should be put out when generating the message to
   the user. The first four enumerations make it possible to see `everything as bad or
   worse than' this level, e.g. HT_ERR_SHOW_NON_FATAL shows messages of type
   HT_ERR_SHOW_NON_FATAL and HT_ERR_SHOW_FATAL.
   
   Note: The default value is made so that it only puts a message to stderr if a `real'
   error has occurred. If a separate widget is available for information and error
   messages then probably HT_ERR_SHOW_DETAILED would be more appropriate.
   
 */
typedef enum _HTErrorShow {
    HT_ERR_SHOW_FATAL     = 0x1,
    HT_ERR_SHOW_NON_FATAL = 0x3,
    HT_ERR_SHOW_WARNING   = 0x7,
    HT_ERR_SHOW_INFO      = 0xF,
    HT_ERR_SHOW_PARS      = 0x10,
    HT_ERR_SHOW_LOCATION  = 0x20,
    HT_ERR_SHOW_IGNORE    = 0x40,
    HT_ERR_SHOW_FIRST     = 0x80,
    HT_ERR_SHOW_LINKS     = 0x100,
    HT_ERR_SHOW_DEFAULT   = 0x13,
    HT_ERR_SHOW_DETAILED  = 0x1F,
    HT_ERR_SHOW_DEBUG     = 0x7F
} HTErrorShow;

extern unsigned int HTErrorShowMask;/*

   This is the table containing the actual error-messages and links for more information:
   
 */
typedef struct _HTErrorMsgInfo {
    int         code;                   /* Error number */
    char *      msg;                    /* Short explanation */
    char *      url;                    /* Explaning URL */
} HTErrorMsgInfo;

extern HTErrorMsgInfo error_info[];/*

Public Error Functions

  ADD AN ERROR MESSAGE
  
   This function adds an error message to the error_stack list in the HTRequest structure.
   It always returns a negative value.
   
 */
extern int HTErrorAdd PARAMS((  HTRequest *     request,
                                HTErrSeverity   severity,
                                BOOL            ignore,
                                int             element,
                                void *          par,
                                unsigned int    par_length,
                                char *          where));/*

  ADD A SYSTEM ERROR MESSAGE
  
   This function adds an error from a system call that initializes errno or equivalent and
   adds it to the error_stack list in the HTRequest structure. It always returns a
   negative value.
   
 */
extern int HTErrorSysAdd PARAMS(( HTRequest *   request,
                                  HTErrSeverity severity,
                                  int           errornumber,
                                  BOOL          ignore,
                                  char *        syscall));/*

  IGNORING AN ERROR MESSAGE
  
   If an error message is not to be send to the user, e.g., output to the stream, then the
   ignore flag must be turn on. Theese functions turns it on for the latest error appended
   to the list or an arbitrary error refered to by its handle.
   
 */
extern void HTErrorIgnore PARAMS((HTRequest * request, int handle));
extern void HTErrorIgnoreLast PARAMS((HTRequest * request));/*

  HANDLING PREFIX FOR LINKS IN ERROR MESSAGE
  
   Two functions handle the URL prefix of the links inside the error message. They are
   quite obvious:
   
 */
extern void HTErrorSetPrefix PARAMS((char *path));
extern CONST char *HTErrorGetPrefix NOPARAMS;/*

  FREEING AN ERROR LIST
  
   This is normally done when the HTRequest structure is freed but it might be  done at
   any other time in order to ignore a whole series of errors.
   
 */
extern void HTErrorFree   PARAMS((HTRequest * request));/*

 */
#endif/*

   end */

