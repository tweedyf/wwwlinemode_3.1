/*                                                                   Anchor object for libwww
                                HYPERTEXT "ANCHOR" OBJECT
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   An anchor represents a region of a hypertext document which is linked to another anchor
   in the same or a different document. As always we must emulate the fancy features of
   C++ by hand :-(. In this module you find:
   
      Definition of generic anchor class
      
      Definition of parent anchor class
      
      Definition of child anchor class
      
      Creation and deletion methods
      
      Manipulation of Links
      
      Access Methods for information
      
   This module is implemented by HTAnchor.c, and it is a part of the W3C Reference
   Library.
   
History

      Version 0 (TBL) written in Objective-C for the NeXT browser
      
      Version 1 of 24-Oct-1991 (JFG), written in C, browser-independant
      
      Version 2 written by Henrik Frystyk
      
 */
#ifndef HTANCHOR_H
#define HTANCHOR_H

#include "HTList.h"
#include "HTAtom.h"
#include "HTMethod.h"/*

Types for Atoms

   This is a set of videly used type definitions used through out the Library:
   
 */
typedef HTAtom * HTFormat;
typedef HTAtom * HTLevel;                      /* Used to specify HTML level */
typedef HTAtom * HTEncoding;                             /* Content Encoding */
typedef HTAtom * HTCte;                         /* Content transfer encoding */
typedef HTAtom * HTCharset;
typedef HTAtom * HTLanguage;/*

Anchor data structures

   We have to do this in order to make forward references.
   
 */
typedef struct _HyperDoc HyperDoc;  /* Ready for forward references */
typedef struct _HTAnchor HTAnchor;
typedef struct _HTParentAnchor HTParentAnchor;
typedef struct _HTChildAnchor HTChildAnchor;/*

   Must be AFTER definition of HTAnchor:
   
 */
typedef HTAtom HTLinkType;

typedef struct {
    HTAnchor *  dest;           /* The anchor to which this leads */
    HTLinkType *type;           /* Semantics of this link */
    HTMethod    method;         /* Method for this link, for example PUT */
    int         result;         /* Result of any attempt to get this link */
} HTLink;/*

  GENERIC ANCHOR TYPE
  
   This is the super class of anchors. We often use this as an argument to the functions
   that both accept parent anchors and child anchors. We separate the first link from the
   others to avoid too many small mallocs involved by a list creation. Most anchors only
   point to one place.
   
 */
struct _HTAnchor {
  HTLink        mainLink;       /* Main (or default) destination of this */
  HTList *      links;          /* List of extra links from this, if any */
  HTParentAnchor * parent;      /* Parent of this anchor (self for adults) */
};/*

  ANCHOR FOR A PARENT OBJECT
  
   These anchors points to the whole contents of a graphic object (document). The parent
   anchor of a parent anchor is itself. The parent anchor now contains all meta
   information about the object. This is largely the entity headers in the HTTP
   specification.
   
 */
struct _HTParentAnchor {
  /* Common part from the generic anchor structure */
  HTLink        mainLink;       /* Main (or default) destination of this */
  HTList *      links;          /* List of extra links from this, if any */
  HTParentAnchor * parent;      /* Parent of this anchor (self) */

  /* ParentAnchor-specific information */
  HTList *      children;       /* Subanchors of this, if any */
  HTList *      sources;        /* List of anchors pointing to this, if any */
  HyperDoc *    document;       /* The document within this is an anchor */
  void *        protocol;       /* Protocol object */
  char *        physical;       /* Physical address */
  HTList *      cacheItems ;    /* Cache hits (see HTFWriter ) for this URL */
  char *        address;        /* Absolute address of this node */
  BOOL          isIndex;        /* Acceptance of a keyword search */

  /* Entity header fields */
  BOOL          header_parsed;  /* Are we done parsing? */

  char *        title;
  int           methods;        /* Allowed methods (bit-flag) */

  HTEncoding    content_encoding;
  HTLanguage    content_language;       /* @@@ SHOULD BE LIST @@@ */
  long int      content_length;
  HTCte         cte;            /* Content-Transfer-Encoding */
  HTFormat      content_type;
  HTCharset     charset;        /* Parameter to content-type */
  HTLevel       level;          /* Parameter to content-type `text/html' */

  time_t        date;           /* When was the request issued */
  time_t        expires;        /* When does the copy expire */
  time_t        last_modified;

  char *        derived_from;   /* Opaque string */
  char *        version;        /* Opaque string */

  /* List of unknown headers coming in from the network. Use the field in the
     request structure for sending test headers. */
  HTList *      extra_headers;
};/*

  ANCHOR FOR A CHILD OBJECT
  
   A child anchor is a anchor object that points to a subpart of a graphic object
   (document)
   
 */
struct _HTChildAnchor {
  /* Common part from the generic anchor structure */
  HTLink        mainLink;       /* Main (or default) destination of this */
  HTList *      links;          /* List of extra links from this, if any */
  HTParentAnchor * parent;      /* Parent of this anchor */

  /* ChildAnchor-specific information */
  char *        tag;            /* Address of this anchor relative to parent */
};/*

Creation and Deletion Methods

   After we have defined the data structures we must define the methods that can be used
   on them. All anchors are kept in an internal hash table so that they are easier to find
   again.
   
  FIND/CREATE A PARENT ANCHOR
  
   This one is for a reference (link) which is found in a document, and might not be
   already loaded. The parent anchor returned can either be created on the spot or is
   already in the hash table.
   
 */
extern HTAnchor * HTAnchor_findAddress          PARAMS((CONST char * address));/*

  FIND/CREATE A CHILD ANCHOR
  
   This one is for a new child anchor being edited into an existing document. The parent
   anchor must already exist but the child returned can either be created on the spot or
   is already in the hash table. The tag is the part that's after the '#' sign in a URI.
   
 */
extern HTChildAnchor * HTAnchor_findChild       PARAMS((HTParentAnchor *parent,
                                                        CONST char *    tag));/*

  FIND/CREATE A CHILD ANCHOR AND LINK TO ANOTHER PARENT
  
   Find a child anchor anchor with a given parent and possibly a tag, and (if passed) link
   this child to the URI given in the href. As we really want typed links to the caller
   should also indicate what the type of the link is (see HTTP spec for more information).
   The link is relative to the address of the parent anchor.
   
 */
extern HTChildAnchor * HTAnchor_findChildAndLink
                PARAMS((HTParentAnchor * parent,        /* May not be 0 */
                CONST char * tag,                       /* May be "" or 0 */
                CONST char * href,                      /* May be "" or 0 */
                HTLinkType * ltype));                   /* May be 0 *//*

  DELETE AN ANCHOR
  
   All outgoing links from parent and children are deleted, and this anchor is removed
   from the sources list of all its targets. We also delete the targets. If this anchor's
   source list is empty, we delete it and its children.
   
 */
extern BOOL HTAnchor_delete     PARAMS((HTParentAnchor *me));/*

  DELETE ALL ANCHORS
  
   Deletes all anchors and return a list of all the HyperDocs found while doing it. The
   application may keep its own list of HyperDocs, but this function returns it anyway.
   It is always for the application to delete any HyperDocs. Return YES if OK, else NO.
   
   Note: This function is different from cleaning up the history list!
   
 */
extern BOOL HTAnchor_deleteAll  PARAMS((HTList * objects));/*

Manipulation of Links

   We really should be using typed links all of us :-)
   
  LINK THIS ANCHOR TO ANOTHER GIVEN ONE
  
   A single anchor may have many outgoing links of which the default is the main link. If
   one already exists then this new link is simply added to the list.
   
 */
extern BOOL HTAnchor_link       PARAMS((HTAnchor *      source,
                                        HTAnchor *      destination,
                                        HTLinkType *    type,
                                        HTMethod        method));/*

  FIND DESTINATION OF MAIN LINK
  
   Find the anchor which is the main target of this anchor.
   
 */
extern HTAnchor * HTAnchor_followMainLink       PARAMS((HTAnchor *me));/*

  FIND METHOD OF MAIN LINK
  
 */
extern HTMethod HTAnchor_mainLinkMethod         PARAMS((HTAnchor *me));/*

  FIND DESTINATION WITH GIVEN RELATIONSHIP
  
   Return the anchor with a given typed link.
   
 */
extern HTAnchor * HTAnchor_followTypedLink      PARAMS((HTAnchor *me,
                                                        HTLinkType *type));/*

  IS THIS ANCHOR A DESTINATION LINK?
  
   If the destination anchor is a link from the source anchor then return YES, else NO.
   
 */
extern BOOL HTAnchor_isLink             PARAMS((HTAnchor *src, HTAnchor *dest));/*

  MAKE A PARTICULAR LINK THE MAIN LINK
  
   Any outgoing link can at any time be the main destination.
   
 */
extern BOOL HTAnchor_makeMainLink       PARAMS((HTAnchor *me, HTLink *movingLink));/*

  MOVE LINK INFORMATION
  
   Move all link information form one anchor to another. This is useful when we get a
   redirection on a request and want to inherit the link information to the new anchor and
   change the link information in the old one to "redirect".
   
 */
extern BOOL HTAnchor_moveLinks                  PARAMS((HTAnchor *src,
                                                        HTAnchor *dest));/*

  MOVE A CHILD ANCHOR TO THE HEAD OF THE LIST OF ITS SIBLINGS
  
   This is to ensure that an anchor which might have already existed is put in the correct
   order as we load the document.
   
 */
extern void HTAnchor_makeLastChild              PARAMS((HTChildAnchor *me));/*

Access Methods of an Anchor

   These functions should be used to access information within the anchor structures.
   
  RELATIONS TO OTHER ANCHORS
  
    Who is Parent?
    
   For parent anchors this returns the anchor itself
   
 */
extern HTParentAnchor * HTAnchor_parent         PARAMS((HTAnchor *me));/*

    Does it have any Anchors within it?
    
 */
extern BOOL HTAnchor_hasChildren                PARAMS((HTParentAnchor *me));/*

  GRAPHIC OBJECT (HYPERDOC)
  
   HyperDoc is the application defined data structure that contains a graphic object in
   memory.
   
    Assign a HyperDoc to an Anchor
    
 */
extern void HTAnchor_setDocument                PARAMS((HTParentAnchor *me,
                                                        HyperDoc *      doc));/*

    Return the HyperDoc of an anchor (if any)
    
 */
extern HyperDoc * HTAnchor_document     PARAMS((HTParentAnchor *me));/*

  URI INFORMATION OF ANCHORS
  
   There are two addresses of an anchor. The URI that was passed when the anchor was
   crated and the physical address that's used when the URI is going to be requested. The
   two addresses may be different if the request is going through a proxy or a gateway.
   
    Get URI Address
    
   Returns the full URI of the anchor, child or parent as a malloc'd string to be freed by
   the caller as when th eanchor was created.
   
 */
extern char * HTAnchor_address          PARAMS((HTAnchor *me));/*

  PHYSICAL ADDRESS
  
   Contains the physical address after we haved looked for proxies etc.
   
 */
extern char * HTAnchor_physical         PARAMS((HTParentAnchor * me));

extern void HTAnchor_setPhysical        PARAMS((HTParentAnchor * me,
                                                char * protocol));/*

  IS THE ANCHOR SEARCHABLE?
  
 */
extern void HTAnchor_clearIndex         PARAMS((HTParentAnchor *me));
extern void HTAnchor_setIndex           PARAMS((HTParentAnchor *me));
extern BOOL HTAnchor_isIndex            PARAMS((HTParentAnchor *me));/*

  WHAT PROTOCOL SHOULD BE USED
  
 */
extern void * HTAnchor_protocol         PARAMS((HTParentAnchor * me));
extern void HTAnchor_setProtocol        PARAMS((HTParentAnchor * me,
                                                void* protocol));/*

  TITLE HANDLING
  
   We keep the title in the anchor as we then can refer to it later in the history list
   etc. We can also obtain the title element if it is passed as a HTTP header in the
   response. Any title element found in an HTML document will overwrite a title given in a
   HTTP header.
   
 */
extern CONST char * HTAnchor_title      PARAMS((HTParentAnchor *me));

extern void HTAnchor_setTitle           PARAMS((HTParentAnchor *me,
                                                CONST char *    title));

extern void HTAnchor_appendTitle        PARAMS((HTParentAnchor *me,
                                                CONST char *    title));/*

  MEDIA TYPES (CONTENT-TYPE)
  
 */
extern HTFormat HTAnchor_format         PARAMS((HTParentAnchor *me));
extern void HTAnchor_setFormat          PARAMS((HTParentAnchor *me,
                                                HTFormat        form));/*

  CHARSET PARAMETER TO CONTENT-TYPE
  
 */
extern HTCharset HTAnchor_charset       PARAMS((HTParentAnchor *me));
extern void HTAnchor_setCharset         PARAMS((HTParentAnchor *me,
                                                HTCharset       charset));/*

  LEVEL PARAMETER TO CONTENT-TYPE
  
 */
extern HTLevel HTAnchor_level           PARAMS((HTParentAnchor * me));
extern void HTAnchor_setLevel           PARAMS((HTParentAnchor * me,
                                                HTLevel         level));/*

  CONTENT LANGUAGE
  
 */
extern HTLanguage HTAnchor_language     PARAMS((HTParentAnchor *me));
extern void HTAnchor_setLanguage        PARAMS((HTParentAnchor *me,
                                                HTLanguage      language));/*

  CONTENT ENCODING
  
 */
extern HTEncoding HTAnchor_encoding     PARAMS((HTParentAnchor *me));
extern void HTAnchor_setEncoding        PARAMS((HTParentAnchor *me,
                                                HTEncoding      encoding));/*

  CONTENT TRANSFER ENCODING
  
 */
extern HTCte HTAnchor_cte               PARAMS((HTParentAnchor *me));
extern void HTAnchor_setCte             PARAMS((HTParentAnchor *me,
                                                HTCte           cte));/*

  CONTENT LENGTH
  
 */
extern long int HTAnchor_length         PARAMS((HTParentAnchor *me));
extern void HTAnchor_setLength          PARAMS((HTParentAnchor *me,
                                                long int        length));/*

  ALLOWED METHODS (ALLOW)
  
 */
extern int HTAnchor_methods             PARAMS((HTParentAnchor *me));
extern void HTAnchor_setMethods         PARAMS((HTParentAnchor *me,
                                                int             methodset));
extern void HTAnchor_appendMethods      PARAMS((HTParentAnchor *me,
                                                int             methodset));/*

  VERSION
  
 */
extern CONST char * HTAnchor_version    PARAMS((HTParentAnchor *me));
extern void HTAnchor_setVersion         PARAMS((HTParentAnchor *me,
                                                CONST char *    version));/*

  DERIVED FROM
  
 */
extern CONST char * HTAnchor_derived    PARAMS((HTParentAnchor *me));
extern void HTAnchor_setDerived         PARAMS((HTParentAnchor *me,
                                                CONST char *    derived_from));/*

  EXTRA HEADERS
  
   List of unknown headers coming in from the network. Do not use the HTAnchor_addExtra()
   function to extra headers here, but use the field in the request structure for sending
   test headers.
   
 */
extern HTList * HTAnchor_Extra          PARAMS((HTParentAnchor *me));
extern void HTAnchor_addExtra           PARAMS((HTParentAnchor *me,
                                                CONST char *    header));/*

  STATUS OF HEADER PARSING
  
   These are primarily for internal use
   
 */
extern BOOL HTAnchor_headerParsed       PARAMS((HTParentAnchor *me));
extern void HTAnchor_setHeaderParsed    PARAMS((HTParentAnchor *me));/*

  WE WANT TO CLEAR THE HEADER INFORMATION...
  
 */
extern void HTAnchor_clearHeader        PARAMS((HTParentAnchor *me));

#endif /* HTANCHOR_H *//*

    */

