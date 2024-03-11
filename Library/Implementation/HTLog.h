/*                                                                                Log Manager
                                       LOG MANAGER
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module maintaines logs of request to a file.
   
   This module is implemented by HTLog.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTLIBLOG_H
#define HTLIBLOG_H

#include "HTAccess.h"/*

Enable the log

   Open the log file and start doing log. The time used in the log file is either GMT or
   local dependent on local.
   
 */
extern BOOL HTLog_enable        PARAMS((CONST char * filename, BOOL local));/*

Disable the log

   Close the log file and do more log
   
 */
extern BOOL HTLog_disable NOPARAMS;/*

Log a Request

   This functions logs the result of a request.
   
 */
extern BOOL HTLog_request       PARAMS((HTRequest * request));

#endif/*

   End of declaration */

