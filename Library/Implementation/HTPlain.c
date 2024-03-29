#include <stdio.h>
/*								      HTPlain.c
**	PLAIN TEXT OBJECT
**
**	(c) COPYRIGHT MIT 1995.
**	Please first read the full copyright statement in the file COPYRIGH.
**
**	This version of the stream object just writes to a socket.
**	The socket is assumed open and left open.
**
**	Bugs:
**		strings written must be less than buffer size.
*/

/* Library include files */
#include "tcp.h"
#include "HTUtils.h"
#include "HText.h"
#include "HTStyle.h"
#include "HTPlain.h"

#define BUFFER_SIZE 4096;	/* Tradeoff */

extern HTStyleSheet * styleSheet;



/*		HTML Object
**		-----------
*/

struct _HTStream {
	CONST HTStreamClass *	isa;

	HText * 		text;
};

/*	Write the buffer out to the socket
**	----------------------------------
*/


/*_________________________________________________________________________
**
**			A C T I O N 	R O U T I N E S
*/

/*	Character handling
**	------------------
*/

PRIVATE int HTPlain_put_character ARGS2(HTStream *, me, char, c)
{
    HText_appendCharacter(me->text, c);
    return HT_OK;
}


/*	String handling
**	---------------
**
*/
PRIVATE int HTPlain_put_string ARGS2(HTStream *, me, CONST char*, s)
{
    HText_appendText(me->text, s);
    return HT_OK;
}


PRIVATE int HTPlain_write ARGS3(HTStream *, me, CONST char*, b, int, l)
{
    while (l-- > 0)
	HText_appendCharacter(me->text, *b++);
    return HT_OK;
}



/*	Flush an Plain object
**	--------------------
*/
PRIVATE int HTPlain_flush ARGS1(HTStream *, me)
{
    return HT_OK;
}

/*	Free an HTML object
**	-------------------
**
**	Note that the SGML parsing context is freed, but the created object is not,
**	as it takes on an existence of its own unless explicitly freed.
*/
PRIVATE int HTPlain_free ARGS1(HTStream *, me)
{
    free(me);
    return HT_OK;
}

/*	End writing
*/

PRIVATE int HTPlain_abort ARGS2(HTStream *, me, HTError, e)
{
    HTPlain_free(me);
    return HT_ERROR;
}



/*		Structured Object Class
**		-----------------------
*/
PUBLIC CONST HTStreamClass HTPlain =
{		
	"PlainText",
	HTPlain_flush,
	HTPlain_free,
	HTPlain_abort,
	HTPlain_put_character, 	HTPlain_put_string, HTPlain_write,
}; 


/*		New object
**		----------
*/
PUBLIC HTStream* HTPlainPresent ARGS5(
	HTRequest *,		request,
	void *,			param,
	HTFormat,		input_format,
	HTFormat,		output_format,
	HTStream *,		output_stream)
{

    HTStream* me = (HTStream*)malloc(sizeof(*me));
    if (me == NULL) outofmem(__FILE__, "HTPlain_new");
    me->isa = &HTPlain;       

    me->text = HText_new2(request->anchor, output_stream);
    HText_beginAppend(me->text);
    HText_setStyle(me->text, HTStyleNamed(styleSheet, "Example"));

    return (HTStream*) me;
}


