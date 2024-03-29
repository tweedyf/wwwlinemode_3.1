/*                                                  Event Functions for Multithreaded Library
                        EVENT FUNCTIONS FOR MULTITHREADED LIBRARY
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module is the application interface to the multi-threaded functionality in the
   Library. It contains a set of functions that the application can either use as are or
   they can be overwritten by the application. If the current implementation is kept, then
   the mode is called Active Mode of the multi-threaded model. If they are overwritten
   then it is called the Passive Mode implementation.
   
   This module is implemented by HTEvntrg.c, and it is a part of the W3C Reference
   Library.
   
   Note: No matter of the mode used (active or passive) the module uses a set of call-back
   functions. The definition of these modules are for the application to provide.
   
   The basic elements in the model are:
   
      An eventloop containing a select function
      
      Three sets of pending socket descriptors (READ, WRITE, INTERRUPTED)
      
      A set of call-back functions to e defined by the application
      
      The existent set of load functions in the HTAccess module.
      
   In order to use the code in the simplest way (active mode) the application must do the
   following:
   
      Implement a set (at least one) call-back function(s) of type HTEventHandler
      
      Register the eventhandlers using HTEventRegister
      
      Implement the HTEventRequestTerminate call-back function to react to the result of
      the request.
      
      Call the HTEventLoop with parameters to load the home document.
      
 */
#ifndef HTEVNTRG_H
#define HTEVNTRG_H

#include "tcp.h"
#include "HTUtils.h"
#include "HTAccess.h"
#include "HTThread.h"/*

Event Handlers

   Note:  application implementation
   
   The appplication registers a set of event handlers to be used on a specified set of
   sockets. The eventhandlers must be defined as follows:
   
 */
typedef u_long SockOps ;
typedef u_long Priority ;

#define FD_ALL (FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE )

#define FD_UNREGISTER ( ((FD_ALL) << 1) &   (~(FD_ALL)))


typedef enum _HTEventState {
    EVENT_OK = 0,
    EVENT_INTR,                               /* Interrupt the active thread */
    EVENT_INTR_ALL,                      /* Interrupt all registered threads */
    EVENT_TERM,                 /* Force a call to HTEventRequestTerminate() */
    EVENT_QUIT                                             /* QUIT eventloop */
} HTEventState;

typedef HTEventState (*HTEventHandler)  PARAMS((HTRequest ** request));

typedef int (*HTEventCallBack) PARAMS((SOCKET, HTRequest *, SockOps)) ;

#if 0
typedef struct _HTEventCallBack {
    int                 sockfd;
    HTEventHandler      callback;
} HTEventCallBack;
#endif/*

  REGISTER A USER EVENT HANDLER
  
   An event handler can be registered using the following function:
   
 */
#if 0
extern BOOL HTEventRegister     PARAMS((HTEventCallBack * user_socket));
#endif/*

   and unregistered using this function
   
 */
#if 0
extern BOOL HTEventUnregister   PARAMS((HTEventCallBack * user_socket));
#endif/*

  CLEANUP MEMORY
  
   The following functions deletes the list of registered eventhandlers (not the structure
   elements themselves - only the list!
   
 */
#if 0
extern void HTEventCleanup      NOPARAMS;
#endif/*

   The following functions are provided as a means of supporting  client implemented event
   loops. This requires that the library have some way of registering and unregistering
   its own sockets with the client such that the client can actually implement the select
   call (or  equivalent. These functions are provided as part of the library; they  are
   separate from the HTEvent.c module in that the  must be replaced if the client
   implements its own event loop  mechanism.
   
 */
#if 0
void HTEventThread_new PARAMS (( void * )) ;
#endif /*

   As implemented in the library, this function just calls HTThread_new with its argument
   case appropriately.
   
 */
#if 0
int HTEventThread_clear PARAMS(( void * )) ;
#endif /*

   As implemented, this function just calls HTThead_clearwith its argument cast
   appropriately.
   
 */
#if 0
void HTEventThreadState PARAMS(( SOCKFD, HTThreadAction ));
#endif /*

   This function just registers the specified SOCKET for the  action specified. As
   implemented, this just calls the  HTThreadState function. When select returns one of
   the sockets registered for user events, for example STDIN, as ready for READ then a
   function of type HTEventHandler is called to figure out what to do with the event. As
   mentioned, it is for the client to implement these functions, but the return codes must
   be as defined by HTEventState.
   
   The client can use the load functions directly described in HTAccess Module from within
   any of the functions registered.
   
   When the library calls the application the request field always contains the current
   request structure. The library reacts to the return value of the eventhandler.
   
Terminate a thread

   Note:  client implementation
   
   This function is a callback function to the client to tell the result of a terminated
   request. The functionality is the same as for one of the EventHandler functions. The
   status code passed to the application is equivalent to the set of codes defined for the
   load functions in HTAccess.
   
 */
#if 0
extern HTEventState HTEventRequestTerminate PARAMS((HTRequest * request,
                                                    int         status));
#endif /*

   This function can be used to update the history list, hotlist etc.
   
Eventloop

   This is the internal socket eventloop that is executed when the client doesn't want to
   have the eventloop. The client can interrupt this loop by sending an event on one of
   the sockets registered for user events. The arguments are used to load the first
   document (the home page) and are supposed to make life easier for the client.
   
 */
extern int HTEventLoop                  PARAMS((HTRequest *     homerequest,
                                        HTParentAnchor *        homeanchor,
                                        CONST char *            homekeywords));

#endif /* HTEvent_H *//*

   End of interface definition module
   
 */
extern int HTEvent_Register PARAMS(( SOCKET,
                HTRequest *, SockOps, HTEventCallBack, Priority));

extern int HTEvent_RegisterTTY PARAMS(( SOCKET, HTRequest *,
        SockOps, HTEventCallBack, Priority));

extern int HTEvent_UnRegister PARAMS(( SOCKET, SockOps)) ;
extern int HTEvent_UnRegisterTTY PARAMS(( SOCKET, SockOps)) ;


extern int HTEvent_Loop PARAMS(( HTRequest * )) ;

extern int HTEvent_UnregisterAll PARAMS(( void )) ;

extern HTEventState HTEventRequestTerminate PARAMS(( HTRequest *, int)) ;

extern BOOL HTEventCheckState PARAMS(( HTRequest *, HTEventState ));

extern int HTEvent_Count PARAMS(( void )) ;/*

    */

