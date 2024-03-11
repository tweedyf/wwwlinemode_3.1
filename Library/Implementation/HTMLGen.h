/*                                                                             HTML Generator
                                      HTML GENERATOR
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module converts structed stream into stream.  That is, given a stream to write to,
   it will give you a structured stream to.
   
   This module is implemented by HTMLGen.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTMLGEN_H
#define HTMLGEN_H

#include "HTStruct.h"

extern HTStructured * HTMLGenerator PARAMS((HTStream * output));

#ifndef pyramid
extern HTConverter HTPlainToHTML;
#endif

#endif/*

   End of declaration  */

