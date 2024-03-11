/*                                                                        HTTP Request Stream
                                   HTTP REQUEST STREAM
                                             
   The HTTP Request stream generates a HTTP request header and writes it to the target
   which is normally a HTWriter stream.
   
   This module is implemented by HTTPReq.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTTPREQ_H
#define HTTPREQ_H

#include "HTStream.h"
#include "HTAccess.h"/*

  STREAMS DEFINITION
  
   This stream makes a HTTP request header before it goes into transparent mode.
   
 */
extern HTStream * HTTPRequest_new       PARAMS((HTRequest *     request,
                                                HTStream *      target));

#endif/*

   End of HTTPReq */

