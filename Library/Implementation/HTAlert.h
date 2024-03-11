/*                                                      Displaying and Promting User Messages
                          DISPLAYING AND PROMTING USER MESSAGES
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module may be overridden for GUI clients. It allows progress indications and
   warning messages to be communicated to the user in a portable way using stdio. Yes, I
   know that not all platforms have stdio :-(. It contain two parts:
   
      A User Interactive part
      
      A part for sending messages to the user
      
      May 92 Created By C.T. Barker
      
      Feb 93 Portablized etc TBL
      
      Mar 95 Updated by Henrik
      
   This module is implemented by HTAlert.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTALERT_H
#define HTALERT_H

#include "HTAccess.h"/*

User Interactive Functions

   These functions require the user to interact in some way
   
  FLAGS FOR THIS MODULE
  
   If you really don't want the library to prompt for anything at all then enable this
   constant. The default value is OFF. All functions returning a string return a dynamic
   string which must be freed by the caller.
   
 */
extern BOOL HTInteractive;                  /* Any prompts from the Library? *//*

  DISPLAY A MESSAGE, THEN WAIT FOR 'YES' OR 'NO'
  
   This function prompts the user for a confirmation on the message passed as a parameter.
   If the user reacts in the affirmative, returns TRUE, returns FALSE otherwise.
   
 */
extern BOOL HTConfirm   PARAMS((CONST char * Msg));/*

  PROMPT THE USER A QUESTION
  
   Prompt for answer and get text back. Reply text is either NULL on error or a dynamic
   string which the caller must free.
   
 */
                
extern char * HTPrompt PARAMS((CONST char * Msg, CONST char * deflt));/*

  PROMPT FOR A PASSWORD
  
   Prompt for password without echoing the reply. Reply text is weither NULL on error or a
   dynamic string which the caller must free.
   
   NOTE: The current version uses getpass which on many systems returns a string of 8 or
   16 bytes.
   
 */
extern char * HTPromptPassword PARAMS((CONST char * Msg));/*

  PROMPT FOR A USERID AND A PASSWORD
  
   This is just a composite function using HTPrompt and HTPromptPassword. The strings
   returned must be freed by caller.
   
 */
extern void HTPromptUsernameAndPassword PARAMS((CONST char *    Msg,
                                                char **         username,
                                                char **         password));/*

Messages, Warnings, and Errors

   These functions are used to inform the user of an event which requires no response form
   the user.
   
  DISPLAY A MESSAGE
  
   This function simply puts out the message passed.
   
 */
extern void HTAlert PARAMS((CONST char * Msg));/*

  DISPLAY A STATUS MESSAGE ON WHAT'S GOING ON
  
   This function can be used to indicate the current status of a certain action.
   
 */
extern void HTProgress PARAMS((CONST char * Msg));/*

  GENERATING AN ERROR MESSAGE OF A REQUEST
  
   This function outputs the content of the error_stack to standard output (used in Line
   Mode Browser), but smart clients and servers might overwrite this function so that the
   error messages can be handled to the user in a nice(r) way. That is the reason for
   putting the actual implementation in HTAlert.c.
   
   Note: If a stream has been put up (and maybe taken down again) inside the Library, then
   request->error_block has been set to YES. This indicates that it is NOT possible any
   more to use the stream as output for the message.
   
 */
extern void HTErrorMsg    PARAMS((HTRequest * request));

#endif/*

    */

