/*                                                     Utility macros for the W3 code library
                                  MACROS FOR GENERAL USE
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module is a part of the W3C Reference Library. See also the system dependent file
   tcp module for system specific information.
   
 */
#ifndef HTUTILS_H
#define HTUTILS_H

#if defined(__STDC__) || defined(__cplusplus) || defined(_WINDOWS)
#define _STANDARD_CODE_
#endif/*

Debug message control

   This is the global flag for setting the TRACE options.
   
 */
#ifndef DEBUG
#define DEBUG   /* No one ever turns this off as trace is too important */
#endif          /* Keep option for really small memory applications too */

extern int WWW_TraceFlag;                    /* Global flag for all W3 trace *//*

   The verbose mode is no longer a simple boolean but a bit field so that it is possible
   to see parts of the output messages. The TRACE define still outputs all messages if
   verbose mode is active, but in addition the following TRACE defines have been made:
   
 */
#ifdef NO_STDIO
extern FILE *WWWTrace;
#ifndef TRACE_FILE
#define TRACE_FILE "WWWTRACE.TXT"
#endif
#define TDEST WWWTrace
#else /* got stdio */
#define TDEST stderr
#endif

typedef enum _HTTraceFlags {
    SHOW_CACHE_TRACE    = 0x2,                          /*                10 */
    SHOW_SGML_TRACE     = 0x8,                          /*              1000 */
    SHOW_BIND_TRACE     = 0x10,                         /*            1.0000 */
    SHOW_THREAD_TRACE   = 0x20,                         /*           10.0000 */
    SHOW_STREAM_TRACE   = 0x40,                         /*          100.0000 */
    SHOW_PROTOCOL_TRACE = 0x80,                         /*         1000.0000 */
    SHOW_URI_TRACE      = 0x200,                        /*      10.0000.0000 */
    SHOW_ANCHOR_TRACE   = 0x800,                        /*   10.00.0000.0000 */
    SHOW_ALL_TRACE      = 0xFFF                         /*   11.11.1111.1111 */
} HTTraceFlags;/*

   The flags are made so that they can serve as a group flag for correlated trace
   messages, e.g. showing messages for SGML and HTML at the same time.
   
 */
#ifdef DEBUG
#define TRACE           (WWW_TraceFlag)
#define CACHE_TRACE     (WWW_TraceFlag & SHOW_CACHE_TRACE)
#define SGML_TRACE      (WWW_TraceFlag & SHOW_SGML_TRACE)
#define BIND_TRACE      (WWW_TraceFlag & SHOW_BIND_TRACE)
#define THD_TRACE       (WWW_TraceFlag & SHOW_THREAD_TRACE)
#define STREAM_TRACE    (WWW_TraceFlag & SHOW_STREAM_TRACE)
#define PROT_TRACE      (WWW_TraceFlag & SHOW_PROTOCOL_TRACE)
#define URI_TRACE       (WWW_TraceFlag & SHOW_URI_TRACE)
#define ANCH_TRACE      (WWW_TraceFlag & SHOW_ANCHOR_TRACE)
#else
#define TRACE           0
#define CACHE_TRACE     0
#define SGML_TRACE      0
#define BIND_TRACE      0
#define THD_TRACE       0
#define STREAM_TRACE    0
#define PROT_TRACE      0
#define URI_TRACE       0
#define ANCH_TRACE      0
#endif/*

   Note:  The CTRACE flag might interfere with other if () else constructions in the code.
   IT SHOULD NOT BE USED BUT REPLACED BY TRACE
   
 */
#define CTRACE if(TRACE) fprintf/*

Error type

   THIS IS NOW OBSOLETE AND WILL BE REMOVED IN FUTURE RELEASES
   
   This is passed back when streams are aborted. It might be nice to have some structure
   of error messages, numbers, and recursive pointers to reasons. Curently this is a
   placeholder for something more sophisticated.
   
 */
typedef void * HTError;                 /* Unused at present -- best definition? *//*

Standard C library for malloc() etc

   Replace memory allocation and free C RTL functions with VAXC$xxx_OPT altrenatives for
   VAXC (but not DECC) on VMS. This makes a big performance difference. (Foteos Macrides).
   
 */
#ifdef vax
#ifdef unix
#define ultrix  /* Assume vax+unix=ultrix */
#endif
#endif

#ifndef VMS
#ifndef ultrix
#ifdef NeXT
#include <libc.h>       /* NeXT */
#endif

#ifndef Mips
#ifndef MACH /* Vincent.Cate@furmint.nectar.cs.cmu.edu */
#include <stdlib.h>     /* ANSI */
#endif
#endif

#else /* ultrix */
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>   /* ANSI */   /* BSN */
#endif

#else   /* VMS */
#include <stdio.h>
#include <stdlib.h>
#include <unixlib.h>
#include <ctype.h>
#if defined(VAXC) && !defined(__DECC)
#define malloc  VAXC$MALLOC_OPT
#define calloc  VAXC$CALLOC_OPT
#define free    VAXC$FREE_OPT
#define cfree   VAXC$CFREE_OPT
#define realloc VAXC$REALLOC_OPT
#endif /* VAXC but not DECC */
#define unlink remove
#define gmtime localtime
#include <stat.h>
#define S_ISDIR(m)      (((m)&S_IFMT) == S_IFDIR)
#define S_ISREG(m)      (((m)&S_IFMT) == S_IFREG)
#define putenv HTVMS_putenv
#endif/*

  SAFE CLEANING MEMORY
  
 */
#define FREE(x) if (x) {free(x); x=NULL;}/*

Macros for declarations

 */
#define PUBLIC                  /* Accessible outside this module     */
#define PRIVATE static          /* Accessible only within this module */

#ifdef _STANDARD_CODE_

#ifndef sco                     /* The sco CC compiler does not know const */
#define CONST const             /* "const" only exists in STDC */
#else
#define CONST
#endif

#define NOPARAMS (void)
#define PARAMS(parameter_list) parameter_list
#define NOARGS (void)
#define ARGS1(t,a) \
                (t a)
#define ARGS2(t,a,u,b) \
                (t a, u b)
#define ARGS3(t,a,u,b,v,c) \
                (t a, u b, v c)
#define ARGS4(t,a,u,b,v,c,w,d) \
                (t a, u b, v c, w d)
#define ARGS5(t,a,u,b,v,c,w,d,x,e) \
                (t a, u b, v c, w d, x e)
#define ARGS6(t,a,u,b,v,c,w,d,x,e,y,f) \
                (t a, u b, v c, w d, x e, y f)
#define ARGS7(t,a,u,b,v,c,w,d,x,e,y,f,z,g) \
                (t a, u b, v c, w d, x e, y f, z g)
#define ARGS8(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h) \
                (t a, u b, v c, w d, x e, y f, z g, s h)
#define ARGS9(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h,r,i) \
                (t a, u b, v c, w d, x e, y f, z g, s h, r i)
#define ARGS10(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h,r,i,q,j) \
                (t a, u b, v c, w d, x e, y f, z g, s h, r i, q j)

#else  /* not ANSI */

#define CONST

#define NOPARAMS ()
#define PARAMS(parameter_list) ()
#define NOARGS ()
#define ARGS1(t,a) (a) \
                t a;
#define ARGS2(t,a,u,b) (a,b) \
                t a; u b;
#define ARGS3(t,a,u,b,v,c) (a,b,c) \
                t a; u b; v c;
#define ARGS4(t,a,u,b,v,c,w,d) (a,b,c,d) \
                t a; u b; v c; w d;
#define ARGS5(t,a,u,b,v,c,w,d,x,e) (a,b,c,d,e) \
                t a; u b; v c; w d; x e;
#define ARGS6(t,a,u,b,v,c,w,d,x,e,y,f) (a,b,c,d,e,f) \
                t a; u b; v c; w d; x e; y f;
#define ARGS7(t,a,u,b,v,c,w,d,x,e,y,f,z,g) (a,b,c,d,e,f,g) \
                t a; u b; v c; w d; x e; y f; z g;
#define ARGS8(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h) (a,b,c,d,e,f,g,h) \
                t a; u b; v c; w d; x e; y f; z g; s h;
#define ARGS9(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h,r,i) (a,b,c,d,e,f,g,h,i) \
                t a; u b; v c; w d; x e; y f; z g; s h; r i;
#define ARGS10(t,a,u,b,v,c,w,d,x,e,y,f,z,g,s,h,r,i,q,j) (a,b,c,d,e,f,g,h,i,j) \
                t a; u b; v c; w d; x e; y f; z g; s h; r i; q j;
                
        
#endif /* _STANDARD_CODE_ (ANSI) */

#ifndef NULL
#define NULL ((void *)0)
#endif/*

Booleans

 */
#ifndef BOOLEAN_DEFINED
typedef char    BOOLEAN;                                    /* Logical value */
#endif

#ifndef CURSES
#ifndef TRUE
#define TRUE    (BOOLEAN)1
#define FALSE   (BOOLEAN)0
#endif
#endif   /*  CURSES  */

#ifndef BOOL
#define BOOL BOOLEAN
#endif
#ifndef YES
#define YES             (BOOL)1
#define NO              (BOOL)0
#endif

#ifndef HTMIN
#define HTMIN(a,b) ((a) <= (b) ? (a) : (b))
#define HTMAX(a,b) ((a) >= (b) ? (a) : (b))
#endif

#define TCP_PORT 80     /* Allocated to http by Jon Postel/ISI 24-Jan-92 */
#define OLD_TCP_PORT 2784       /* Try the old one if no answer on 80 */
#define DNP_OBJ 80      /* This one doesn't look busy, but we must check */
                        /* That one was for decnet *//*

Return Codes for Protocol Modules

   Theese are the codes returned from the protocol modules. Success (>=0) and failure (<0)
   codes
   
 */
#define HT_OK                   0       /* Generic success */
#define HT_LOADED               29999   /* Instead of a socket */
#define HT_REDIRECTION_ON_FLY   29998   /* Redo the retrieve with a new URL */

#define HT_ERROR                -1      /* Generic failure */
#define HT_NO_ACCESS            -10     /* Access not available */
#define HT_FORBIDDEN            -11     /* Access forbidden */
#define HT_INTERNAL             -12     /* Weird -- should never happen. */
#define HT_NO_DATA              -9999   /* OK but no data was loaded */
                                        /* Typically, other app started */
#define HT_RETRY                -29996  /* if service isn't available */
#define HT_WOULD_BLOCK          -29997  /* If we are in a select */
#define HT_INTERRUPTED          -29998  /* Note the negative value! */

#ifdef _STANDARD_CODE_
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#ifdef CURSES
        /* htbrowse.c; */
#ifdef ULTRIX      /* or DECSTATION */
#include <cursesX.h> /* Extended curses under X. Only decent one :lou. */
#else
#include <curses.h>
#endif /* ULTRIX */
        extern        WINDOW  *w_top, *w_text, *w_prompt;
        extern        void    user_message PARAMS((const char *fmt, ...));
        extern        void    prompt_set PARAMS((CONST char * msg));
        extern        void    prompt_count PARAMS((long kb));
#else
#define user_message printf
#endif
/*

Out Of Memory checking for malloc() return:

 */
#ifndef __FILE__
#define __FILE__ ""
#define __LINE__ ""
#endif

#define outofmem(file, func) \
 { fprintf(TDEST, "%s %s: out of memory.\nProgram aborted.\n", file, func); \
  exit(1);}
/*

Upper- and Lowercase macros

   The problem here is that toupper(x) is not defined officially unless isupper(x) is.
   These macros are CERTAINLY needed on #if defined(pyr) || define(mips) or BDSI
   platforms. For safefy, we make them mandatory.
   
 */
#include <ctype.h>

#ifndef TOLOWER
#define TOLOWER(c) tolower(c)
#define TOUPPER(c) toupper(c)
#endif/*

Max and Min values for Integers and Floating Point

 */
#ifdef FLT_EPSILON                                  /* The ANSI C way define */
#define HT_EPSILON FLT_EPSILON
#else
#define HT_EPSILON 0.00000001
#endif/*

White Characters

   Is character c white space?
   
 */
#ifdef OLD_CODE
#define WHITE(c) (((unsigned char)(TOASCII(c))) <= 32)
#else
#define WHITE(c) isspace(c)
#endif/*

The local equivalents of CR and LF

   We can check for these after net ascii text has been converted to the local
   representation. Similarly, we include them in strings to be sent as net ascii after
   translation.
   
 */
#define LF   FROMASCII('\012')  /* ASCII line feed LOCAL EQUIVALENT */
#define CR   FROMASCII('\015')  /* Will be converted to ^M for transmission */

#endif /* HTUTILS_H *//*

   end of utilities */

