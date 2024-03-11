/*                                                      The Format Manager in the WWW Library
                                    THE FORMAT MANAGER
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   Here we describe the functions of the HTFormat module which handles conversion between
   different data representations. (In MIME parlance, a representation is known as a
   content-type. In WWW the term format is often used as it is shorter). The content of
   this module is:
   
      Converters
      
      User Preferences (media type, language, charset etc.)
      
      Content Negotiation
      
      The Stream Stack
      
   This module is implemented by HTFormat.c, and it is a part of the W3C Reference
   Library.
   
 */
#ifndef HTFORMAT_H
#define HTFORMAT_H

#include "HTUtils.h"
#include "HTStream.h"
#include "HTAtom.h"
#include "HTList.h"
#include "HTAnchor.h"
#include "HTAccess.h"/*

Stream Converters

   A converter is a stream with a special set of parameters and which is registered as
   capable of converting from a MIME type to something else (maybe another MIME-type). A
   converter is defined to be a function returning a stream and accepting the following
   parameters. The content type elements are atoms for which we have defined a prototype.
   
 */
typedef HTStream * HTConverter  PARAMS((HTRequest *     request,
                                        void *          param,
                                        HTFormat        input_format,
                                        HTFormat        output_format,
                                        HTStream *      output_stream));/*

User Preferences

   The Library contains functionality for letting the application (or user) express the
   preferences for the rendition of a given data object when issuing a request. The
   categories supported are:
   
      Content type (media type)
      
      Encoding
      
      Language
      
      Charset
      
   There are two places where these preferences can be registered: in a global list valid
   for all requests and a local list valid for a particular request only. The elements are
   all atoms representations. This allows faster manipulation (comparison and copying)
   that if we stayed with strings.
   
  REGISTRATION OF ACCEPTED CONTENT TYPES
  
   A presenter is a module (possibly an external program) which can present a graphic
   object of a certain MIME type to the user. That is, presenters are normally used to
   present objects that the converters are not able to handle. Data is transferred to the
   external program using for example the HTSaveAndExecute stream which writes to a local
   file. Both presenters and converters are of the type HTConverter.
   
 */
typedef struct _HTPresentation {
    HTFormat    rep;                         /* representation name atomized */
    HTFormat    rep_out;                         /* resulting representation */
    HTConverter *converter;           /* The routine to gen the stream stack */
    char *      command;                               /* MIME-format string */
    char *      test_command;                          /* MIME-format string */
    double      quality;                     /* Between 0 (bad) and 1 (good) */
    double      secs;
    double      secs_per_byte;
} HTPresentation;

extern HTList * HTConversions;   /* Global list of converters and presenters *//*

    Predefined Content Types
    
   These macros (which used to be constants) define some basic internally referenced
   representations. The www/xxx ones are of course not MIME standard. star/star is an
   output format which leaves the input untouched. It is useful for diagnostics, and for
   users who want to see the original, whatever it is.
   
 */
#define WWW_SOURCE      HTAtom_for("*/*")      /* Whatever it was originally *//*

   www/present represents the user's perception of the document.  If you convert to
   www/present, you present the material to the user.
   
 */
#define WWW_PRESENT     HTAtom_for("www/present")   /* The user's perception *//*

   The message/rfc822 format means a MIME message or a plain text message with no MIME
   header. This is what is returned by an HTTP server.
   
 */
#define WWW_MIME        HTAtom_for("www/mime")             /* A MIME message *//*

   www/unknown is a really unknown type.  Some default action is appropriate.
   
 */
#define WWW_UNKNOWN     HTAtom_for("www/unknown")/*

   These are regular MIME types defined. Others can be added!
   
 */
#define WWW_PLAINTEXT   HTAtom_for("text/plain")
#define WWW_POSTSCRIPT  HTAtom_for("application/postscript")
#define WWW_RICHTEXT    HTAtom_for("application/rtf")
#define WWW_AUDIO       HTAtom_for("audio/basic")
#define WWW_HTML        HTAtom_for("text/html")
#define WWW_BINARY      HTAtom_for("application/octet-stream")
#define WWW_VIDEO       HTAtom_for("video/mpeg")
#define WWW_GIF         HTAtom_for("image/gif")/*

    Register a Presenter
    
  conversions            The list of conveters and presenters
                         
  representation         the MIME-style format name
                         
  command                the MAILCAP-style command template
                         
  quality                A degradation faction [0..1]
                         
  maxbytes               A limit on the length acceptable as input (0 infinite)
                         
  maxsecs                A limit on the time user will wait (0 for infinity)
                         
 */
extern void HTSetPresentation   PARAMS((HTList *        conversions,
                                        CONST char *    representation,
                                        CONST char *    command,
                                        CONST char *    test_command,
                                        double          quality,
                                        double          secs,
                                        double          secs_per_byte));/*

    Register a Converter
    
  conversions            The list of conveters and presenters
                         
  rep_in                 the MIME-style format name
                         
  rep_out                is the resulting content-type after the conversion
                         
  converter              is the routine to call which actually does the conversion
                         
  quality                A degradation faction [0..1]
                         
  maxbytes               A limit on the length acceptable as input (0 infinite)
                         
  maxsecs                A limit on the time user will wait (0 for infinity)
                         
 */
extern void HTSetConversion     PARAMS((HTList *        conversions,
                                        CONST char *    rep_in,
                                        CONST char *    rep_out,
                                        HTConverter *   converter,
                                        double          quality,
                                        double          secs,
                                        double          secs_per_byte));/*

    Set up Default Presenters and Converters
    
   The HTInit module defines a default set of converters and presenters. However, no
   automatic initialization is done in the Library, so this is for the application to do!
   
 */
extern void HTFormatInit        PARAMS((HTList * conversions));/*

  REGISTRATION OF ACCEPTED CONTENT ENCODINGS
  
   Encodings are the HTTP extension of transfer encodings. Encodings include compress,
   gzip etc. These are the data structures:
   
 */
typedef struct _HTAcceptNode {
    HTAtom *    atom;
    double      quality;
} HTAcceptNode;

extern HTList * HTEncodings;                     /* Global list of encodings *//*

    Predefined Encoding Types
    
 */
#define WWW_ENC_7BIT            HTAtom_for("7bit")
#define WWW_ENC_8BIT            HTAtom_for("8bit")
#define WWW_ENC_BINARY          HTAtom_for("binary")
#define WWW_ENC_BASE64          HTAtom_for("base64")
#define WWW_ENC_COMPRESS        HTAtom_for("compress")
#define WWW_ENC_GZIP            HTAtom_for("gzip")/*

    Register an Encoding
    
 */
extern void HTAcceptEncoding    PARAMS((HTList *        list,
                                        CONST char *    enc,
                                        double          quality));/*

  REGISTRATION OF ACCEPTED CHARSETS
  
   We use the same HTAcceptNode as above.
   
 */
extern HTList * HTCharsets;                       /* Global list of charsets *//*

    Register a Charset
    
 */
extern void HTAcceptCharset     PARAMS((HTList *        list,
                                        CONST char *    charset,
                                        double          quality));/*

  REGISTRATION OF ACCEPTED CONTENT LANGUAGES
  
   We use the same HTAcceptNode as above.
   
 */
extern HTList * HTLanguages;                     /* Global list of languages *//*

    Register a Language
    
 */
extern void HTAcceptLanguage    PARAMS((HTList *        list,
                                        CONST char *    lang,
                                        double          quality));/*

  GARBAGE COLLECTION OF USER PREFERENCES
  
   This is used for cleaning up the lists of user preferences, both the local and the
   global lists:
   
  REMOVE LOCAL PREFERENCES
  
   The function is called from HTRequest_delete.
   
 */
extern void HTFormatDelete      PARAMS((HTRequest * request));/*

  REMOVE GLOBAL PREFERENCES
  
   The function is called from HTLibTerminate.
   
 */
extern void HTDisposeConversions NOPARAMS;/*

Ranking of Accepted Formats

   This function is used when the best match among several possible documents is to be
   found as a function of the accept headers sent in the client request.
   
 */
typedef struct _HTContentDescription {
    char *      filename;
    HTAtom *    content_type;
    HTAtom *    content_language;
    HTAtom *    content_encoding;
    int         content_length;
    double      quality;
} HTContentDescription;

extern BOOL HTRank PARAMS((HTList * possibilities,
                           HTList * accepted_content_types,
                           HTList * accepted_content_languages,
                           HTList * accepted_content_encodings));/*

The Stream Stack

   This is the routine which actually sets up the conversion. It currently checks only for
   direct conversions, but multi-stage conversions are forseen.  It takes a stream into
   which the output should be sent in the final format, builds the conversion stack, and
   returns a stream into which the data in the input format should be fed. If guess is
   true and input format is www/unknown, try to guess the format by looking at the first
   few bytes of the stream.
   
 */
extern HTStream * HTStreamStack PARAMS((HTFormat        rep_in,
                                        HTFormat        rep_out,
                                        HTStream *      output_stream,
                                        HTRequest *     request,
                                        BOOL            guess));/*

Cost of a Stream Stack

   Must return the cost of the same stack which HTStreamStack would set up.
   
 */
extern double HTStackValue      PARAMS((HTList *        conversions,
                                        HTFormat        format_in,
                                        HTFormat        format_out,
                                        double          initial_value,
                                        long int        length));

#endif /* HTFORMAT *//*

   End of declaration module  */

