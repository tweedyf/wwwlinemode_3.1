/*                                                                      File access in libwww
                                       FILE ACCESS
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   These are routines for local file access used by WWW browsers and servers.
   
   If the file is not a local file, then we pass it on to HTFTP in case it can be reached
   by FTP. However, as this is very time consuming when requesting a local file that
   actually doesn't exist, this redirection will be disabled in the next major release,
   www-bug@w3.org June 1994.
   
   Note: All functions that deal with directory listings etc. have been moved to HTDirBrw
   Module.
   
   This module is implemented by HTFile.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTFILE_H
#define HTFILE_H

#include "HTProt.h"

GLOBALREF HTProtocol HTFile;
/*

Make a WWW name from a full local path name

 */
extern char * WWW_nameOfFile    PARAMS((const char * name));/*

Determine write access to a file

 */
extern BOOL HTEditable          PARAMS((CONST char * filename,
                                        struct stat *stat_info));/*

 */
#endif /* HTFILE_H *//*

   End of declaration of HTFile module  */

