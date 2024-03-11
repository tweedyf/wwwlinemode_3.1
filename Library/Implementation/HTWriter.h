/*                                                                   Socket writer for libwww
                          UNIX FILE DESCRIPTOR OR SOCKET WRITER
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This version of the stream object just writes to a socket. The socket is assumed open
   and closed afterward.There are two versions (identical on ASCII machines) one of which
   converts to ASCII on output.
   
   This module is implemented by HTWriter.c, and it is a part of the W3C Reference
   Library.
   
 */
#ifndef HTWRITE_H
#define HTWRITE_H

#include "HTStream.h"

extern HTStream * HTWriter_new          PARAMS((SOCKFD soc, BOOL leave_open));

#if 0
extern HTStream * HTWriter_newNoClose   PARAMS((SOCKFD soc));
#endif

#ifdef NOT_ASCII
extern HTStream * HTASCIIWriter         PARAMS((SOCKFD soc));
#endif

#endif/*

   end */

