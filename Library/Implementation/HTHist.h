/*                                                                  History module for libwww
                                     HISTORY MANAGER
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This is a simple history module for a WWW client.  It keeps a linear history, with
   destructive backtrack, and list sequencing (previous, next) operations.
   
   If you are building a client, you don't have to use this: just don't call it.  This
   module is not used by any other modules in the libwww, so if you don't refer to it you
   don't get it in your linked application.
   
   This module is implemented by HTHist.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTHISTORY_H
#define HTHISTORY_H

#include "HTFormat.h"
#include "HTAnchor.h"

/*                              Navigation
**                              ==========
*/

/*              Record the jump to an anchor
**              ----------------------------
*/

extern void HTHistory_record
  PARAMS(
    (HTAnchor * destination)
  );

/*              Go back in history (find the last visited node)
**              ------------------
*/

extern HTAnchor * HTHistory_back        PARAMS((HTAnchor * cur_anch));

extern HTAnchor * HTHistory_forward     PARAMS((HTAnchor * cur_anch));

/* FIXME: Should we add a `sticky' option ? */
extern HTAnchor * HTHistory_backtrack   NOPARAMS;

extern BOOL HTHistory_canBacktrack      NOPARAMS;

/*              Browse through references in the same parent node
**              -------------------------------------------------
**
**      Take the n-th child's link after or before the one we took to get here.
**      Positive offset means go towards most recently added children.
*/

extern HTAnchor * HTHistory_moveBy
  PARAMS(
     (int offset)
     );

extern BOOL HTHistory_canMoveBy
  PARAMS(
     (int offset)
     );

#define HTHistory_next (HTHistory_moveBy (+1))
#define HTHistory_canNext (HTHistory_canMoveBy (+1))
#define HTHistory_previous (HTHistory_moveBy (-1))
#define HTHistory_canPrevious (HTHistory_canMoveBy (-1))


/*                              Retrieval
**                              =========
*/

/*              Read numbered visited anchor (1 is the oldest)
**              ----------------------------
*/

extern HTAnchor * HTHistory_read
  PARAMS(
    (int number)
  );

/*              Recall numbered visited anchor (1 is the oldest)
**              ------------------------------
**      This reads the anchor and stores it again in the list, except if last.
*/

extern HTAnchor * HTHistory_recall
  PARAMS(
    (int number)
  );

/*              Number of Anchors stored
**              ------------------------
**
**      This is needed in order to check the validity of certain commands
**      for menus, etc.
(not needed for now. Use canBacktrack, etc.)
extern int HTHistory_count NOPARAMS;
*/

/*              Change last history entry
**              -------------------------
**
**      Sometimes we load a node by one anchor but leave by a different
**      one, and it is the one we left from which we want to remember.
*/
extern void HTHistory_leavingFrom
  PARAMS(
    (HTAnchor * anchor)
  );

#endif /* HTHISTORY_H */
/*

    */

