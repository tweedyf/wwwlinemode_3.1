/*                                                                      Access Scheme Manager
                                  ACCESS SCHEME MANAGER
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module keeps a list of valid protocol (naming scheme) specifiers with associated
   access code. New access protocols may be registered at any time.
   
   This module is implemented by HTProt.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTPROT_H
#define HTPROT_H

#include "HTAccess.h"
#include "HTAnchor.h"/*

   An access scheme is defined by an HTProtocol structure which point to the routines for
   performing the various logical operations on an object: in HTTP terms, GET, PUT, and
   POST. The access schemes supported in the Library are initiated automaticly using the
   function HTAccessInit() if not defined HT_NO_INIT
   
   Each of these routine takes as a parameter a request structure containing details of
   the request.  When the protocol class routine is called, the anchor element in the
   request is already valid (made valid by HTAccess).
   
 */
typedef enum _HTSocBlock {
    SOC_BLOCK,
    SOC_NON_BLOCK
} HTSocBlock;

typedef struct _HTProtocol {
    char *      name;
    HTSocBlock  block;
    int         (*load)         PARAMS((HTRequest *     request));
    HTStream*   (*saveStream)   PARAMS((HTRequest *     request));
    HTStream*   (*postStream)   PARAMS((HTRequest *     request,
                                        HTParentAnchor* postTo));
} HTProtocol;/*

  ADD AN ACCESS SCHEME
  
   This functions registers a protocol module and binds it to a specific access acheme.
   For example HTTP.c is bound to http URLs.
   
 */
extern BOOL HTProtocol_add              PARAMS((HTProtocol * protocol));/*

  GET AN ACCESS SCHEME
  
   This function finds the access scheme that is bound to the anchor's type of URL.
   Returns YES if found, else NO
   
 */
extern BOOL HTProtocol_get              PARAMS((HTParentAnchor * anchor));/*

  REMOVE ALL REGISTERED SCHEMES
  
   This is the garbage collection function. It is called by HTLibTerminate
   
 */
extern void HTProtocol_deleteAll        NOPARAMS;/*

  IS THE SCHEME USING BLOCKING I/O?
  
   A small function to make life easier. Returns YES or NO. If the Library is run in
   non-interactive mode then the function always returns YES;
   
 */
#define HTProtocol_isBlocking(me) \
        ((me) && ((me)->BlockingIO || \
                ((me)->anchor && (me)->anchor->protocol && \
                        ((HTProtocol*)((me)->anchor->protocol))->block==SOC_BLOCK)))/*

 */
#endif /* HTPROT_H *//*

   End of Declaration  */

