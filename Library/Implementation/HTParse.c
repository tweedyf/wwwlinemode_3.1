#include <stdio.h>
/*								      HTParse.c
**	URI MANAGEMENT
**
**	(c) COPYRIGHT MIT 1995.
**	Please first read the full copyright statement in the file COPYRIGH.
**
** history:
**	May 12 94	TAB added as legal char in HTCleanTelnetString
**
*/

/* Library include files */
#include "tcp.h"
#include "HTUtils.h"
#include "HTParse.h"
#include "HTString.h"
#include "HTTCP.h"

struct struct_parts {
	char * access;		/* Now known as "scheme" */
	char * host;
	char * absolute;
	char * relative;
/*	char * search;		no - treated as part of path */
	char * anchor;
};

/*	Scan a filename for its consituents
**	-----------------------------------
**
** On entry,
**	name	points to a document name which may be incomplete.
** On exit,
**      absolute or relative may be nonzero (but not both).
**	host, anchor and access may be nonzero if they were specified.
**	Any which are nonzero point to zero terminated strings.
*/
PRIVATE void scan ARGS2(char *, name, struct struct_parts *, parts)
{
    char * after_access;
    char * p;
    int length = strlen(name);
    
    parts->access = 0;
    parts->host = 0;
    parts->absolute = 0;
    parts->relative = 0;
    parts->anchor = 0;
    
    after_access = name;
    for(p=name; *p; p++) {
	if (*p==':') {
		*p = 0;
		parts->access = after_access; /* Scheme has been specified */

/* The combination of gcc, the "-O" flag and the HP platform is
   unhealthy. The following three lines is a quick & dirty fix, but is
   not recommended. Rather, turn off "-O". */

/*		after_access = p;*/
/*		while (*after_access == 0)*/
/*		    after_access++;*/

		after_access = p+1;

		if (0==strcasecomp("URL", parts->access)) {
		    parts->access = NULL;  /* Ignore IETF's URL: pre-prefix */
		} else break;
	}
	if (*p=='/') break;		/* Access has not been specified */
	if (*p=='#') break;
    }
    
    for(p=name+length-1; p>=name; p--) {
	if (*p =='#') {
	    parts->anchor=p+1;
	    *p=0;				/* terminate the rest */
	}
    }
    p = after_access;
    if (*p=='/'){
	if (p[1]=='/') {
	    parts->host = p+2;		/* host has been specified 	*/
	    *p=0;			/* Terminate access 		*/
	    p=strchr(parts->host,'/');	/* look for end of host name if any */
	    if(p) {
	        *p=0;			/* Terminate host */
	        parts->absolute = p+1;		/* Root has been found */
	    }
	} else {
	    parts->absolute = p+1;		/* Root found but no host */
	}	    
    } else {
        parts->relative = (*after_access) ? after_access : 0; /* zero for "" */
    }

#ifdef OLD_CODE
    /* Access specified but no host: the anchor was not really one
       e.g. news:j462#36487@foo.bar -- JFG 10/jul/92, from bug report */
    /* This kludge doesn't work for example when coming across
       file:/usr/local/www/fred#123
       which loses its anchor. Correct approach in news is to
       escape weird characters not allowed in URL.  TBL 21/dec/93
    */
    if (parts->access && ! parts->host && parts->anchor) {
      *(parts->anchor - 1) = '#';  /* Restore the '#' in the address */
      parts->anchor = 0;
    }
#endif

#ifdef NOT_DEFINED	/* search is just treated as part of path */
    {
        char *p = relative ? relative : absolute;
	if (p) {
	    char * q = strchr(p, '?');	/* Any search string? */
	    if (q) {
	    	*q = 0;			/* If so, chop that off. */
		parts->search = q+1;
	    }
	}
    }
#endif
} /*scan */    


/*	Parse a Name relative to another name
**	-------------------------------------
**
**	This returns those parts of a name which are given (and requested)
**	substituting bits from the related name where necessary.
**
** On entry,
**	aName		A filename given
**      relatedName     A name relative to which aName is to be parsed. Give
**                      it an empty string if aName is absolute.
**      wanted          A mask for the bits which are wanted.
**
** On exit,
**	returns		A pointer to a malloc'd string which MUST BE FREED
*/
char * HTParse ARGS3(CONST char *, aName, CONST char *, relatedName,
		     int, wanted)
{
    char * result = 0;
    char * return_value = 0;
    int len;
    char * name = 0;
    char * rel = 0;
    char * p;
    char * access;
    struct struct_parts given, related;
    
    if (!relatedName)        /* HWL 23/8/94: dont dump due to NULL */
        relatedName = "";
    
    /* Make working copies of input strings to cut up:
    */
    len = strlen(aName)+strlen(relatedName)+10;
    result=(char *)malloc(len);		/* Lots of space: more than enough */
    if (result == NULL) outofmem(__FILE__, "HTParse");
    
    StrAllocCopy(name, aName);
    StrAllocCopy(rel, relatedName);
    
    scan(name, &given);
    scan(rel,  &related); 
    result[0]=0;		/* Clear string  */
    access = given.access ? given.access : related.access;
    if (wanted & PARSE_ACCESS)
        if (access) {
	    strcat(result, access);
	    if(wanted & PARSE_PUNCTUATION) strcat(result, ":");
	}
	
    if (given.access && related.access)	/* If different, inherit nothing. */
        if (strcmp(given.access, related.access)!=0) {
	    related.host=0;
	    related.absolute=0;
	    related.relative=0;
	    related.anchor=0;
	}
	
    if (wanted & PARSE_HOST)
        if(given.host || related.host) {
	    if(wanted & PARSE_PUNCTUATION) strcat(result, "//");
	    strcat(result, given.host ? given.host : related.host);
	}
	
    if (given.host && related.host)  /* If different hosts, inherit no path. */
        if (strcmp(given.host, related.host)!=0) {
	    related.absolute=0;
	    related.relative=0;
	    related.anchor=0;
	}
	
    if (wanted & PARSE_PATH) {
        if(given.absolute) {				/* All is given */
	    if(wanted & PARSE_PUNCTUATION) strcat(result, "/");
	    strcat(result, given.absolute);
	} else if(related.absolute) {	/* Adopt path not name */
	    strcat(result, "/");
	    strcat(result, related.absolute);
	    if (given.relative) {
		p = strchr(result, '?');	/* Search part? */
		if (!p) p=result+strlen(result)-1;
		for (; *p!='/'; p--);	/* last / */
		p[1]=0;					/* Remove filename */
		strcat(result, given.relative);		/* Add given one */
		result = HTSimplify (&result);
	    }
	} else if(given.relative) {
	    strcat(result, given.relative);		/* what we've got */
	} else if(related.relative) {
	    strcat(result, related.relative);
	} else {  /* No inheritance */
	    strcat(result, "/");
	}
    }
		
    if (wanted & PARSE_ANCHOR)
        if(given.anchor || related.anchor) {
	    if(wanted & PARSE_PUNCTUATION) strcat(result, "#");
	    strcat(result, given.anchor ? given.anchor : related.anchor);
	}
    free(rel);
    free(name);
    
    StrAllocCopy(return_value, result);
    free(result);
    return return_value;		/* exactly the right length */
}


/*	        Simplify a URI
//		--------------
// A URI is allowed to contain the seqeunce xxx/../ which may be
// replaced by "" , and the seqeunce "/./" which may be replaced by "/".
// Simplification helps us recognize duplicate URIs. 
//
//	Thus, 	/etc/junk/../fred 	becomes	/etc/fred
//		/etc/junk/./fred	becomes	/etc/junk/fred
//
//      but we should NOT change
//		http://fred.xxx.edu/../..
//
//	or	../../albert.html
//
// In the same manner, the following prefixed are preserved:
//
//	./<etc>
//	//<etc>
//
// In order to avoid empty URLs the following URLs become:
//
//		/fred/..		becomes /fred/..
//		/fred/././..		becomes /fred/..
//		/fred/.././junk/.././	becomes /fred/..
//
// If more than one set of `://' is found (several proxies in cascade) then
// only the part after the last `://' is simplified.
//
// Returns: A string which might be the old one or a new one.
*/
PUBLIC char *HTSimplify ARGS1(char **, filename)
{
    char *path;
    char *p;

    if (!*filename) {
	if (URI_TRACE)
	    fprintf(TDEST, "HTSimplify.. Bad argument\n");
	return *filename;
    }
    if (URI_TRACE)
	fprintf(TDEST, "HTSimplify.. `%s\' ", *filename);

    if ((path = strstr(*filename, "://")) != NULL) {	   /* Find host name */
	char *newptr;
	path += 3;
	while ((newptr = strstr(path, "://")) != NULL)
	    path = newptr+3;
	path = HTCanon(filename, path);       	      /* We have a host name */
    } else if ((path = strstr(*filename, ":/")) != NULL) {
	path += 2;
    } else
	path = *filename;
    if (*path == '/' && *(path+1)=='/') {	  /* Some URLs start //<foo> */
	path += 1;
    } else if (!strncmp(path, "news:", 5)) {
	char *ptr = strchr(path+5, '@');
	if (!ptr) ptr = path+5;
	while (*ptr) {			    /* Make group or host lower case */
	    *ptr = TOLOWER(*ptr);
	    ptr++;
	}
	if (URI_TRACE)
	    fprintf(TDEST, "into\n............ `%s'\n", *filename);
	return *filename;		      /* Doesn't need to do any more */
    }
    if ((p = path)) {
	int segments = 0;

	/* Parse string first time to find number of `real' tokens */
	while (*p) {
	    if (*p=='/' || p==path) {
		if (!((*(p+1)=='/' || !*(p+1)) ||
		      (*(p+1)=='.' && (*(p+2)=='/' || !*(p+2))) ||
		      (*(p+1)=='.' && *(p+2)=='.' &&(*(p+3)=='/' || !*(p+3)))))
		    segments++;
	    }
	    p++;
	}
	
	/* Parse string second time to simplify */
	p = path;
	while(*p) {
	    if (*p=='/') {
		if (p>path && *(p+1)=='.' && (*(p+2)=='/' || !*(p+2))) {
		    char *orig=p, *dest=p+2;
		    while ((*orig++ = *dest++)); /* Remove a slash and a dot */
		    p--;
		} else if (segments>1 && *(p+1)=='.' && *(p+2)=='.' &&
		    (*(p+3)=='/' || !*(p+3))) {
		    char *q = p;
		    while (q>path && *--q!='/');	       /* prev slash */
		    if (strncmp(q, "/../", 4) && strncmp(q, "/./", 3) &&
			strncmp(q, "./", 2)) {
			char *orig=q, *dest=p+3;
			if (*q!='/') dest++;
			while ((*orig++ = *dest++));	   /* Remove /xxx/.. */
			segments--;
			p = q-1;	      /* Start again with prev slash */
		    } else
			p++;
		} else if (*(p+1)=='/') {
		    while (*(p+1)=='/') {
			char *orig=p, *dest=p+1;
			while ((*orig++ = *dest++));  /* Remove multiple /'s */
		    }
		}
	    }
	    p++;
	}  /* end while (*p) */
    }
    if (URI_TRACE)
	fprintf(TDEST, "into\n............ `%s'\n", *filename);
    return *filename;
}

#ifdef OLD_CODE
    char * p = filename;
    char * q;
    
    if (p) {
	while (*p && (*p == '/' || *p == '.'))     /* Pass starting / or .'s */
	    p++;
	while(*p) {
	    if (*p=='/') {
	    if ((p[1]=='.') && (p[2]=='.') && (p[3]=='/' || !p[3] )) {
		for (q=p-1; (q>=filename) && (*q!='/'); q--); /* prev slash */
		if (q[0]=='/' && 0!=strncmp(q, "/../", 4)
			&&!(q-1>filename && q[-1]=='/')) {
	            ari_strcpy(q, p+3);		/* Remove  /xxx/..	*/
		    if (!*filename) strcpy(filename, "/");
		    p = q-1;		/* Start again with prev slash 	*/
		} else {			/*   xxx/.. leave it!	*/
#ifdef BUG_CODE
		    ari_strcpy(filename, p[3] ? p+4 : p+3); /* rm  xxx/../ */
		    p = filename;		/* Start again */
#endif
		}
	    } else if ((p[1]=='.') && (p[2]=='/' || !p[2])) {
	        ari_strcpy(p, p+2);		/* Remove a slash and a dot */
	    } else if (p[-1] != ':') {
		while (p[1] == '/') {
		    ari_strcpy(p, p+1);	/* Remove multiple slashes */
		}
	    }
	    }
	    p++;
	}  /* end while (*p) */
    } /* end if (p) */
}
#endif /* OLD_CODE */


/*		Make Relative Name
**		------------------
**
** This function creates and returns a string which gives an expression of
** one address as related to another. Where there is no relation, an absolute
** address is retured.
**
**  On entry,
**	Both names must be absolute, fully qualified names of nodes
**	(no anchor bits)
**
**  On exit,
**	The return result points to a newly allocated name which, if
**	parsed by HTParse relative to relatedName, will yield aName.
**	The caller is responsible for freeing the resulting name later.
**
*/
char * HTRelative ARGS2(CONST char *, aName, CONST char *, relatedName)
{
    char * result = 0;
    CONST char *p = aName;
    CONST char *q = relatedName;
    CONST char * after_access = 0;
    CONST char * path = 0;
    CONST char * last_slash = 0;
    int slashes = 0;
    
    for(;*p; p++, q++) {	/* Find extent of match */
    	if (*p!=*q) break;
	if (*p==':') after_access = p+1;
	if (*p=='/') {
	    last_slash = p;
	    slashes++;
	    if (slashes==3) path=p;
	}
    }
    
    /* q, p point to the first non-matching character or zero */
    
    if (!after_access) {			/* Different access */
        StrAllocCopy(result, aName);
    } else if (slashes<3){			/* Different nodes */
    	StrAllocCopy(result, after_access);
#if 0 /* Henrik */
    } else if (slashes==3){			/* Same node, different path */
        StrAllocCopy(result, path);
#endif
    } else {					/* Some path in common */
        int levels= 0;
        for(; *q && (*q!='#'); q++)  if (*q=='/') levels++;
	result = (char *)malloc(3*levels + strlen(last_slash) + 1);
      if (result == NULL) outofmem(__FILE__, "HTRelative");
	result[0]=0;
	for(;levels; levels--)strcat(result, "../");
	strcat(result, last_slash+1);
    }
    if (URI_TRACE) fprintf(TDEST,
		      "HTRelative.. `%s' expressed relative to `%s' is `%s'\n",
		       aName, relatedName, result);
    return result;
}


/*							       HTCanon
**
**	Canonicalizes the URL in the following manner starting from the host
**	pointer:
**
**	1) The host name is converted to lowercase
**	2) Expands the host name of the URL from a local name to a full
**	   domain name. A host name is started by `://'.
**	3) Chop off port if `:80' (http), `:70' (gopher), or `:21' (ftp)
**
**	Return: OK	The position of the current path part of the URL
**			which might be the old one or a new one.
*/
PUBLIC char *HTCanon ARGS2 (char **, filename, char *, host)
{
    char *newname = NULL;
    char *port;
    char *strptr;
    char *path;
    char *access = host-3;

    while (access>*filename && *(access-1)!='/')       /* Find access method */
	access--;
    if ((path = strchr(host, '/')) == NULL)			/* Find path */
	path = host + strlen(host);
    if ((strptr = strchr(host, '@')) != NULL && strptr<path)	   /* UserId */
	host = strptr;
    if ((port = strchr(host, ':')) != NULL && port>path)      /* Port number */
	port = NULL;

    strptr = host;				    /* Convert to lower-case */
    while (strptr<path) {
	*strptr = TOLOWER(*strptr);
	strptr++;
    }
    
    /* Does the URL contain a full domain name? This also works for a
       numerical host name. The domain name is already made lower-case
       and without a trailing dot. */
    if (((strptr = strchr(host, '.')) == NULL || strptr >= path) &&
	strncasecomp(host, "localhost", 9)) {
	CONST char *domain = HTGetDomainName();
	if (domain) {
	    if ((newname = (char *) calloc(1, strlen(*filename) +
				       strlen(domain)+2)) == NULL)
		outofmem(__FILE__, "HTCanon");
	    if (port)
		strncpy(newname, *filename, (int) (port-*filename));
	    else
		strncpy(newname, *filename, (int) (path-*filename));
	    strcat(newname, ".");
	    strcat(newname, domain);
	}
    } else {					  /* Look for a trailing dot */
	char *dot = port ? port : path;
	if (dot > *filename && *--dot=='.') {
	    char *orig=dot, *dest=dot+1;
	    while((*orig++ = *dest++));
	    if (port) port--;
	    path--;
	}
    }
    /* Chop off port if `:', `:80' (http), `:70' (gopher), or `:21' (ftp) */
    if (port) {
	if (!*(port+1) || *(port+1)=='/') {
	    if (!newname) {
		char *orig=port, *dest=port+1;
		while((*orig++ = *dest++));
	    }
	} else if ((!strncmp(access, "http", 4) &&
	     (*(port+1)=='8'&&*(port+2)=='0'&&(*(port+3)=='/'||!*(port+3)))) ||
	    (!strncmp(access, "gopher", 6) &&
	     (*(port+1)=='7'&&*(port+2)=='0'&&(*(port+3)=='/'||!*(port+3)))) ||
	    (!strncmp(access, "ftp", 3) &&
	     (*(port+1)=='2'&&*(port+2)=='1'&&(*(port+3)=='/'||!*(port+3))))) {
	    if (!newname) {
		char *orig=port, *dest=port+3;
		while((*orig++ = *dest++));
	    }
	} else if (newname)
	    strncat(newname, port, (int) (path-port));
    }

    if (newname) {
	char *newpath = newname+strlen(newname);
	strcat(newname, path);
	path = newpath;
	free(*filename);				    /* Free old copy */
	*filename = newname;
    }
    return path;
}


/*							HTCleanTelnetString()
 *	Make sure that the given string doesn't contain characters that
 *	could cause security holes, such as newlines in ftp, gopher,
 *	news or telnet URLs; more specifically: allows everything between
 *	ASCII 20-7E, and also A0-FE, inclusive. Also TAB ('\t') allowed!
 *
 * On entry,
 *	str	the string that is *modified* if necessary.  The
 *		string will be truncated at the first illegal
 *		character that is encountered.
 * On exit,
 *	returns	YES, if the string was modified.
 *		NO, otherwise.
 */
PUBLIC BOOL HTCleanTelnetString ARGS1(char *, str)
{
    char * cur = str;

    if (!str) return NO;

    while (*cur) {
	int a = TOASCII(*cur);
	if (a != 0x9 && (a < 0x20 || (a > 0x7E && a < 0xA0) ||  a > 0xFE)) {
	    if (URI_TRACE)
		fprintf(TDEST, "Illegal..... character in URL: \"%s\"\n",str);
	    *cur = 0;
	    if (URI_TRACE)
		fprintf(TDEST, "Truncated... \"%s\"\n",str);
	    return YES;
	}
	cur++;
    }
    return NO;
}

