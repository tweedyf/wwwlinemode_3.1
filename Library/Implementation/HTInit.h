/*                                                          Initialization routines in libwww
                                  INITIALIZATION MODULE
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module resisters all the plug & play software modules which will be used in the
   program on client side (The server has it's own initialization module).
   
   The initialization consists of definiting four bindings:
   
      Between a source media type and a dest media type (conversion)
      
      Between media types and external viewers/presenters
      
      Between file extensions and media types
      
      Between an access method and a protocol module
      
   To override the default behavior, you can do two things:
   
      Use the HT_NO_INIT preprocessor define
      
      Copy it and link in your version before you link with the library.
      
   This module is implemented by HTInit.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTINIT_H
#define HTINIT_H

#include "HTList.h"/*

Media Type Conversions

   The Converters are used to convert a media type to another media type, or to present it
   on screen. This is a part of the stream stack algorithm. The Presenters are also used
   in the stream stack, but are initialized separately.
   
 */
extern void HTConverterInit PARAMS((HTList * conversions));/*

Presenters

   The Presenters are used to present a media type to the use by calling an external
   program, for example a post script viewer. This is a part of the stream stack
   algorithm. The Converters are also used in the stream stack, but are initialized
   separately.
   
 */
extern void HTPresenterInit PARAMS((HTList * conversions));/*

Converters and Presenters

   This function is only defined in order to preserve backward compatibility.
   
 */
extern void HTFormatInit PARAMS((HTList * conversions));/*

Protocol Modules

   Set up default bindings between access schemes and the set of protocol modules in the
   Library.
   
 */
#ifndef HT_NO_INIT
extern void HTAccessInit NOPARAMS;
#endif/*

File Suffix Setup

   This functions defines a basic set of file suffixes and the corresponding media types.
   
 */
#ifndef HT_NO_INIT
extern void HTFileInit NOPARAMS;
#endif

#endif/*

   End of HTInit Module. */

