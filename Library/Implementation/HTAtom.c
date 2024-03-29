/*								       HTAtom.c
**	ATOMS: STRINSGS TO NUMBERS
**
**	(c) COPYRIGHT MIT 1995.
**	Please first read the full copyright statement in the file COPYRIGH.
**
**	Atoms are names which are given representative pointer values
**	so that they can be stored more efficiently, and comparisons
**	for equality done more efficiently.
**
**	Atoms are kept in a hash table consisting of an array of linked lists.
**
** Authors:
**	TBL	Tim Berners-Lee, WorldWideWeb project, CERN
**
*/

/* Library include files */
#include "tcp.h"
#include "HTUtils.h"
#include "HTString.h"
#include "HTList.h"
#include "HTAtom.h"

#define HASH_SIZE	101		/* Tunable */

PRIVATE HTAtom * hash_table[HASH_SIZE];
PRIVATE BOOL initialised = NO;

/*
**	Finds an atom representation for a string. The atom doesn't have to be
**	a new one but can be an already existing atom.
*/
PUBLIC HTAtom * HTAtom_for ARGS1(CONST char *, string)
{
    int hash;
    CONST char * p;
    HTAtom * a;
    
    /*		First time around, clear hash table
    */
    if (!initialised) {
        memset(hash_table, '\0', sizeof(HTAtom *) * HASH_SIZE);
	initialised = YES;
    }
    
    /*		Generate hash function
    */
    for(p=string, hash=0; *p; p++) {
        hash = (hash * 3 +(*(unsigned char *) p)) % HASH_SIZE;
    }
    
    /*		Search for the string in the list
    */
    for (a=hash_table[hash]; a; a=a->next) {
	if (0==strcmp(a->name, string)) {
    	    /* if (TRACE) fprintf(TDEST,
	    	"HTAtom: Old atom %p for `%s'\n", a, string); */
	    return a;				/* Found: return it */
	}
    }
    
    /*		Generate a new entry
    */
    a = (HTAtom *)malloc(sizeof(*a));
    if (a == NULL) outofmem(__FILE__, "HTAtom_for");
    a->name = (char *)malloc(strlen(string)+1);
    if (a->name == NULL) outofmem(__FILE__, "HTAtom_for");
    strcpy(a->name, string);
    a->next = hash_table[hash];		/* Put onto the head of list */
    hash_table[hash] = a;
/*    if (TRACE) fprintf(TDEST, "HTAtom: New atom %p for `%s'\n", a, string); */
    return a;
}


/*
**	CASE INSENSITIVE VERSION OF HTAtom_for()
**	Finds an atom representation for a string. The atom doesn't have to be
**	a new one but can be an already existing atom.
*/
PUBLIC HTAtom * HTAtom_caseFor ARGS1(CONST char *, string)
{
    int hash;
    CONST char * p;
    HTAtom * a;
    
    /*		First time around, clear hash table
    */
    if (!initialised) {
        memset(hash_table, '\0', sizeof(HTAtom *) * HASH_SIZE);
	initialised = YES;
    }
    
    /*		Generate hash function
    */
    for(p=string, hash=0; *p; p++) {
        hash = (hash * 3 + *p) % HASH_SIZE;
    }
    
    /*		Search for the string in the list
    */
    for (a=hash_table[hash]; a; a=a->next) {
	if (!strcasecomp(a->name, string)) {
	    return a;					/* Found: return it */
	}
    }
    
    /*		Generate a new entry
    */
    a = (HTAtom *)malloc(sizeof(*a));
    if (a == NULL) outofmem(__FILE__, "HTAtom_for");
    a->name = (char *)malloc(strlen(string)+1);
    if (a->name == NULL) outofmem(__FILE__, "HTAtom_for");
    strcpy(a->name, string);
    a->next = hash_table[hash];		/* Put onto the head of list */
    hash_table[hash] = a;
    return a;
}


/*
**	This function cleans up the memory used by atoms.
**	Written by Eric Sink, eric@spyglass.com
*/
PUBLIC void HTAtom_deleteAll NOARGS
{
    int i;
    HTAtom *cur;
    HTAtom *next;
    
    for (i=0; i<HASH_SIZE; i++) {
	if (hash_table[i]) {
	    cur = hash_table[i];
	    while (cur) {
		next = cur->next;
		free(cur->name);
		free(cur);
		cur = next;	
	    }	
	}
    }
    initialised = NO;
}


PRIVATE BOOL mime_match ARGS2(CONST char *, name,
			      CONST char *, templ)
{
    if (name && templ) {
	static char *n1 = NULL;
	static char *t1 = NULL;
	char *n2;
	char *t2;

	StrAllocCopy(n1, name);		/* These also free the ones	*/
	StrAllocCopy(t1, templ);	/* from previous call.		*/

	if (!(n2 = strchr(n1, '/'))  ||  !(t2 = strchr(t1, '/')))
	    return NO;

	*(n2++) = (char)0;
	*(t2++) = (char)0;

	if ((0==strcmp(t1, "*") || 0==strcmp(t1, n1)) &&
	    (0==strcmp(t2, "*") || 0==strcmp(t2, n2)))
	    return YES;
    }
    return NO;
}
	

PUBLIC HTList *HTAtom_templateMatches ARGS1(CONST char *, templ)
{
    HTList *matches = HTList_new();

    if (initialised && templ) {
	int i;
	HTAtom *cur;

	for (i=0; i<HASH_SIZE; i++) {
	    for (cur = hash_table[i];  cur;  cur=cur->next) {
		if (mime_match(cur->name, templ))
		    HTList_addObject(matches, (void*)cur);
	    }
	}
    }
    return matches;
}

