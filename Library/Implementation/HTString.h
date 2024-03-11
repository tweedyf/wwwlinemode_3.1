/*                                                                 String handling for libwww
                                    STRING MANAGEMENT
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   These functions provide functionality for case-independent string comparison and
   allocations with copies etc.
   
   This module is implemented by HTString.c, and it is a part of the W3C Reference
   Library.
   
 */
#ifndef HTSTRING_H
#define HTSTRING_H

extern CONST char * HTLibraryVersion;   /* String for help screen etc *//*

Dynamic String Manipulation

   These two functions are dynamic versions of strcpy and strcat. They use malloc for
   allocating space for the string. If StrAllocCopy is called with a non-NULL dest, then
   this is freed before the new value is assigned so that only the last string created has
   to be freed by the user. If StrAllocCat is called with a NULL pointer as destination
   then it is equivalent to StrAllocCopy.
   
 */
#define StrAllocCopy(dest, src) HTSACopy (&(dest), src)
#define StrAllocCat(dest, src)  HTSACat  (&(dest), src)
extern char * HTSACopy PARAMS ((char **dest, CONST char *src));
extern char * HTSACat  PARAMS ((char **dest, CONST char *src));/*

Case-insensitive String Comparison

   The usual routines (comp instead of cmp) had some problem.
   
 */
extern int strcasecomp  PARAMS((CONST char *a, CONST char *b));
extern int strncasecomp PARAMS((CONST char *a, CONST char *b, int n));/*

Case-insensitive strstr

   This works like strstr() but is not case-sensitive.
   
 */
extern char * strcasestr PARAMS((char * s1, char * s2));/*

Strip white space off a string

   Return value points to first non-white character, or to '/0' if none. All trailing
   white space is OVERWRITTEN with zero.
   
 */
extern char * HTStrip PARAMS((char * s));/*

Next word or quoted string

   This function returns a RFC822 word separated by space, comma, or semi-colons.
   
 */
extern char * HTNextField PARAMS ((char** pstr));/*

RFC1123 Date/Time Stamp String

   Returns a pointer to a static area!
   
 */
extern CONST char *HTDateTimeStr PARAMS((time_t *calendar, BOOL local));/*

Timezone Offset

   Calculates the offset from GMT in seconds. This is called from HTLibInit().
   
 */
extern long HTGetTimeZoneOffset NOPARAMS;/*

Parse a Date/Time String

   Converts a string representation in GMT to a local representation of localtime time_t.
   
 */
extern time_t HTParseTime PARAMS((CONST char * str));/*

Unique Message-ID String

 */
extern CONST char *HTMessageIdStr NOPARAMS;
#endif/*

   End of declaration module  */

