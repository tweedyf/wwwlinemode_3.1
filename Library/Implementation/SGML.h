/*                                                                      SGML Parse Definition
                                  SGML PARSE DEFINITION
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   The SGML parser is a state machine.  It is called for every character of the input
   stream. The DTD data structure contains pointers to functions which are called to
   implement the actual effect of the text read. When these functions are called, the
   attribute structures pointed to by the DTD are valid, and the function is parsed a
   pointer to the curent tag structure, and an "element stack" which represents the state
   of nesting within SGML elements. See also the the generic Stream definition
   
   The following aspects are from Dan Connolly's suggestions: Binary search, Strcutured
   object scheme basically, SGML content enum type.
   
   This module is implemented by SGML.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef SGML_H
#define SGML_H

#include "HTStream.h"
#include "HTStruct.h"/*

SGML content types

 */
typedef enum _SGMLContent{
  SGML_EMPTY,    /* no content */
  SGML_LITERAL, /* character data. Recognized exact close tag only.
                    Old www server compatibility only! Not SGML */
  SGML_CDATA,    /* character data. recognize </ only */
  SGML_RCDATA,   /* replaceable character data. recognize </ and &ref; */
  SGML_MIXED,    /* elements and parsed character data. recognize all markup */
  SGML_ELEMENT   /* any data found will be returned as an error*/
  } SGMLContent;


typedef struct {
    char *      name;           /* The (constant) name of the attribute */
                                /* Could put type info in here */
} attr;


/*              A tag structure describes an SGML element.
**              -----------------------------------------
**
**
**      name            is the string which comes after the tag opener "<".
**
**      attributes      points to a zero-terminated array
**                      of attribute names.
**
**      litteral        determines how the SGML engine parses the charaters
**                      within the element. If set, tag openers are ignored
**                      except for that which opens a matching closing tag.
**
*/
typedef struct _tag HTTag;
struct _tag{
    char *      name;                   /* The name of the tag */
    attr *      attributes;             /* The list of acceptable attributes */
    int         number_of_attributes;   /* Number of possible attributes */
    SGMLContent contents;               /* End only on end tag @@ */
};

/*              DTD Information
**              ---------------
**
** Not the whole DTD, but all this parser usues of it.
*/
typedef struct {
    HTTag *             tags;           /* Must be in strcmp order by name */
    int                 number_of_tags;
    CONST char **       entity_names;   /* Must be in strcmp order by name */
    int                 number_of_entities;
} SGML_dtd;

#define MAX_ATTRIBUTES 20            /* Max number of attributes per element */

/*      SGML context passed to parsers */

typedef struct _HTSGMLContext *HTSGMLContext;   /* Hidden *//*

Find a Tag by Name

   Returns a pointer to the tag within the DTD.
   
 */
extern HTTag * SGMLFindTag PARAMS((CONST SGML_dtd* dtd, CONST char * string));/*

Find a Attribute by Name

   Returns the number of the atribute or -1 if failure.
   
 */
extern int SGMLFindAttribute PARAMS((HTTag* tag, CONST char * string));/*

Create an SGML parser

 */
/*
** On entry,
**      dtd             must point to a DTD structure as defined above
**      callbacks       must point to user routines.
**      callData        is returned in callbacks transparently.
** On exit,
**              The default tag starter has been processed.
*/


extern HTStream* SGML_new PARAMS((
        CONST SGML_dtd *                dtd,
        HTStructured *          target));

extern CONST HTStreamClass SGMLParser;

#endif  /* SGML_H */
/*

    */

