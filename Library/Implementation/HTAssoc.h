/*                                                                          Association Pairs
                      ASSOCIATION LIST FOR STORING NAME-VALUE PAIRS
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   Lookups from assosiation list are not case-sensitive.
   
   This module is implemented by HTAssoc.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTASSOC_H
#define HTASSOC_H

#include "HTList.h"

typedef HTList HTAssocList;

typedef struct {
    char * name;
    char * value;
} HTAssoc;


extern HTAssocList *HTAssocList_new NOPARAMS;
extern void HTAssocList_delete PARAMS((HTAssocList * alist));

extern void HTAssocList_add PARAMS((HTAssocList *       alist,
                                    CONST char *        name,
                                    CONST char *        value));

extern char *HTAssocList_lookup PARAMS((HTAssocList *   alist,
                                        CONST char *    name));

#endif /* not HTASSOC_H *//*

   End of file HTAssoc.h. */

