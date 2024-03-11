/*                                                                     List object for libwww
                                       LIST OBJECT
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   The list object is a generic container for storing collections of things in order.  In
   principle it could be implemented in many ways, but in practice knowing that it is a
   linked list is important for speed.  See also the traverse macro for example.
   
   This module is implemented by HTList.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTLIST_H
#define HTLIST_H


typedef struct _HTList HTList;

struct _HTList {
  void * object;
  HTList * next;
};

extern HTList * HTList_new NOPARAMS;
extern void     HTList_delete PARAMS((HTList *me));/*

  ADD OBJECT TO START OF LIST
  
 */
extern BOOL     HTList_addObject PARAMS((HTList *me, void *newObject));
extern BOOL     HTList_removeObject PARAMS((HTList *me, void *oldObject));
extern void *   HTList_removeLastObject PARAMS((HTList *me));
extern void *   HTList_removeFirstObject PARAMS((HTList *me));
#define         HTList_isEmpty(me) (me ? me->next == NULL : YES)
extern int      HTList_count PARAMS((HTList *me));
extern int      HTList_indexOf PARAMS((HTList *me, void *object));
#define         HTList_lastObject(me) \
  (me && me->next ? me->next->object : NULL)
extern void *   HTList_objectAt PARAMS((HTList *me, int position));/*

  TRAVERSE LIST
  
   Fast macro to traverse the list.  Call it first with copy of list header: it returns
   the first object and increments the passed list pointer.  Call it with the same
   variable until it returns NULL.
   
 */
#define HTList_nextObject(me) \
  (me && (me = me->next) ? me->object : NULL)/*

  FREE LIST
  
 */
#define HTList_free(x)  FREE(x)

#endif /* HTLIST_H */
/*

   end */

