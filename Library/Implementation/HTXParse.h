/*                                        ExtParse: Module to get unparsed stream from libwww
                   EXTPARSE: MODULE TO GET UNPARSED STREAM FROM LIBWWW
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This version of the stream object is a hook for clients that want an unparsed stream
   from libwww. The HTExtParse_put_* and HTExtParse_write routines copy the content of the
   incoming buffer into a buffer that is realloced whenever necessary. This buffer is
   handed over to the client in HTExtParse_free. See also HTFWriter for writing to C
   files.
   
  BUGS:
  
      strings written must be less than buffer size.
      
   This module is implemented by HTXParse.c, and it is a part of the W3C Reference
   Library.
   
 */
#ifndef HTEXTPARSE_H
#define HTEXTPARSE_H

#include "HTStream.h"
#include "HTAccess.h"

typedef struct _HTExtParseStruct HTExtParseStruct;
typedef void (*CallClient) PARAMS((struct _HTExtParseStruct *me));
struct _HTExtParseStruct {
        CallClient      call_client;
#if 0
        void            (*call_client)(struct _HTExtParseStruct *eps);
#endif
        int             used;         /* how much of the buffer is being used */
        BOOL            finished;     /* document loaded? */
        int             length;       /* how long the buffer is */
        char *          buffer;       /* buffer to store in until client takes over*/
        char *          content_type;
        HTRequest *     request;      /* the request structure */
};

/*extern HTStream * HTExtParse PARAMS((void (*CallMeArg)()));*/

/* extern void HTExtParse PARAMS((HTExtParseStruct * eps)); */

extern HTStream* HTExtParse PARAMS((
        HTRequest *             request,
        void *                  param,
        HTFormat                input_format,
        HTFormat                output_format,
        HTStream *              output_stream));


#endif

/*

   end */

