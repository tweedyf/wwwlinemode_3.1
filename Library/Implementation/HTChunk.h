/*                                                HTChunk: Flexible array handling for libwww
                             CHUNK HANDLING: FLEXIBLE ARRAYS
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module implements a flexible array. It is a general utility module.  A chunk is a
   structure which may be extended.  These routines create and append data to chunks,
   automatically reallocating them as necessary.  It is garanteed that the array is '\0'
   terminated at all times, so the terminating function, HTChunkTerminate is only
   necessary to adjust the size in the chunk structure (the '\0' counts as a character
   when counting the size of the chunk.
   
   This module is implemented by HTChunk.c, and it is a part of the W3C Reference Library.
   
 */
#ifndef HTCHUNK_H
#define HTCHUNK_H
/*

 */
typedef struct {
        int     size;           /* In bytes                     */
        int     growby;         /* Allocation unit in bytes     */
        int     allocated;      /* Current size of *data        */
        char *  data;           /* Pointer to malloced area or 0 */
} HTChunk;
/*

Create new chunk

  ON ENTRY,
  
  growby                  The number of bytes to allocate at a time when the chunk is
                         later extended. Arbitrary but normally a trade-off time vs.
                         memory
                         
  ON EXIT,
  
  returns                 A chunk pointer to the new chunk,
                         
 */
extern HTChunk * HTChunkCreate PARAMS((int growby));

/*

Free a chunk

  ON ENTRY,
  
  ch                      A valid chunk pointer made by HTChunkCreate()
                         
  ON EXIT,
  
  ch                      is invalid and may not be used.
                         
 */
extern void HTChunkFree PARAMS((HTChunk * ch));

/*

Clear a chunk

  ON ENTRY,
  
  ch                      A valid chunk pointer made by HTChunkCreate()
                         
  ON EXIT,
  
  *ch                     The size of the chunk is zero.
                         
 */
extern void HTChunkClear PARAMS((HTChunk * ch));

/*

Ensure a chunk has a certain space in

  ON ENTRY,
  
  ch                      A valid chunk pointer made by HTChunkCreate()
                         
  s                       The size required
                         
  ON EXIT,
  
  *ch                     Has size at least s
                         
 */
extern void HTChunkEnsure PARAMS((HTChunk * ch, int s));

/*

Append a character to a  chunk

  ON ENTRY,
  
  ch                      A valid chunk pointer made by HTChunkCreate()
                         
  c                       The character to be appended
                         
  ON EXIT,
  
  *ch                     Is one character bigger
                         
 */
extern void HTChunkPutc PARAMS((HTChunk * ch, char c));
/*

Append a string to a  chunk

  ON ENTRY,
  
  ch                      A valid chunk pointer made by HTChunkCreate()
                         
  str                     Tpoints to a zero-terminated string to be appended
                         
  ON EXIT,
  
  *ch                     Is bigger by strlen(str)
                         
 */
extern void HTChunkPuts PARAMS((HTChunk * ch, const char *str));

/*

Append a zero character to a  chunk

 */
/*

  ON ENTRY,
  
  ch                      A valid chunk pointer made by HTChunkCreate()
                         
  ON EXIT,
  
  *ch                     Is one character bigger
                         
 */
extern void HTChunkTerminate PARAMS((HTChunk * ch));

#endif
/*

   end */

