/*                                                     Style Management for Line Mode Browser
                            MAIN MODULE FOR LINE MODE BROWSER
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module defines the global variables etc. for the Line Mode Browser. The module is
   implemented by HTBrowse.c, and it is a part of the Line Mode Browser.
   
 */
#ifndef HTBROWSE_H
#define HTBROWSE_H

#include "tcp.h"

#ifdef SHORT_NAMES
#define HTScreenHeight          HTScHeig
#define HTScreenWidth           HTScWidt
#define display_anchors         HTDiAnch
#define interactive             HTIntera
#define reference_mark          HTReMark
#endif

extern  int  HTScreenWidth;             /* By default */
extern  int  HTScreenHeight;            /* Undefined */
extern  BOOL display_anchors;           /* anchor will be shown in text? */

                                        
extern char * reference_mark;           /* Format string for  [1] &c */
extern char * end_mark;                 /* Format string for  [End] */

#endif /* HTBROWSE_H */
/*

   End of HTBrowse Declaration */

