/*                                           Cache Manager and Writing to a File using ANSI C
                     CACHE MANAGER AND WRITING TO A FILE USING ANSI C
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   It is useful to have both FWriter and Writer for environments in which fdopen() doesn't
   exist for example. The module contains the following parts:
   
      A Balckhole for absorbing data
      
      A File Writer Stream
      
      Converters using the file writer stream
      
      Cache Manager
      
   This module is implemented by HTFWrite.c, and it is a part of the W3C Reference
   Library.
   
 */
#ifndef HTFWRITE_H
#define HTFWRITE_H

#include "HTStream.h"
#include "HTFormat.h"/*

Black Hole Stream

   This stream simply absorbs data without doing anything what so ever.
   
 */
extern HTStream * HTBlackHole NOPARAMS;/*

Converters

   This is the set of functions that can be registered as converters. Note that the cache
   writer stream is also a converter!
   
 */
#ifndef pyramid
extern HTConverter HTSaveAndExecute, HTSaveLocally, HTSaveAndCallBack,
       HTThroughLine, HTCacheWriter;
#endif/*

Location of Temporary Files

   HTSaveAndCallBack will save to a cache file and call the request->callback function
   with the filename as parameter. The destination for temporary files can be managed by
   the following functions:
   
 */
extern BOOL  HTTmp_setRoot              PARAMS((CONST char * tmp_root));
extern CONST char * HTTmp_getRoot       NOPARAMS;
extern void  HTTmp_freeRoot             NOPARAMS;/*

   The HTTmp_freeRoot is called by the HTLibTerminate function. The default value is
   defined in HTAccess.html
   
Set up a Stream to a File

   This function puts up a new stream given an open file descripter. If the file is not to
   be closed afterwards, then set leave_open = NO.
   
 */
extern HTStream * HTFWriter_new PARAMS((FILE *  fp,
                                             BOOL       leave_open));/*

The Cache Manager

   The cache contains details of temporary disk files which contain the contents of remote
   documents.  There is also a list of cache items for each URL in its anchor object.
   
  AN ITEM IN THE CACHE
  
   This will not be public in the next release so DON'T USE IT ;-)
   
 */
typedef struct _HTCacheItem {
    HTParentAnchor *    anchor;
    HTFormat            format;         /* May have many formats per anchor */
    char *              filename;
    time_t              load_time;
    time_t              load_delay;
    int                 reference_count;
} HTCacheItem;/*

  THE CACHE LIMIT
  
   The cache limit is the number of files which are kept. Yes, I know, the amount of disk
   space would be more relevant. So this may change. Currently it is preset to 100 but may
   be changed by the application by writing into this variable.
   
 */
extern int HTCacheLimit;/*

  ENABLE CACHE
  
   If cache_root is NULL then reuse old value or use HT_CACHE_ROOT.  An empty string will
   make '/' as cache root.
   
 */
extern BOOL HTCache_enable PARAMS((CONST char * cache_root));/*

  DISABLE CACHE
  
   Turns off the cache. Note that the cache can be disabled and enabled at any time. The
   cache root is kept and can be reused during the execution.
   
 */
extern BOOL HTCache_disable NOPARAMS;/*

  IS CACHE ENABLED
  
   Returns YES or NO. Also makes sure that we have a root value (even though it might be
   invalid)
   
 */
extern BOOL HTCache_isEnabled NOPARAMS;/*

  SET CACHE ROOT
  
   If cache_root is NULL then the current value (might be a define) Should we check if the
   cache_root is actually OK? I think not!
   
 */
extern BOOL HTCache_setRoot PARAMS((CONST char * cache_root));/*

  GET CACHE ROOT
  
 */
extern CONST char *HTCache_getRoot NOPARAMS;/*

  FOR CLEAN UP MEMORY
  
   This is done by the Library function HTLibTerminate()
   
 */
extern void HTCache_freeRoot NOPARAMS;/*

  CLEAR A CACHE
  
   Can clear a list of cache items.
   
 */
extern void HTCacheClear PARAMS((HTList * cache));/*

  TO REMOVE ALL CACHE FILES KNOWN TO THIS SESSION
  
 */
extern void HTCacheDeleteAll NOPARAMS;

#endif/*

   End of definition module */

