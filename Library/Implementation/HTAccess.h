/*                                                                 Access manager  for libwww
                                      ACCESS MANAGER
                                             
 */
/*
**      (c) COPYRIGHT MIT 1995.
**      Please first read the full copyright statement in the file COPYRIGH.
*//*

   This module keeps a list of valid protocol (naming scheme) specifiers with associated
   access code.  It allows documents to be loaded given various combinations of
   parameters.  New access protocols may be registered at any time.
   
   This module is implemented by HTAccess.c, and it is a part of the W3C Reference
   Library.
   
   The module contains a lot of stuff but the main topics are:
   
      Initializing and Terminating the Library
      
      Default Directories and URLs
      
      Global Flags
      
      Management of the HTRequest Object
      
      Handling HTTP headers
      
      Functions for loading a document
      
      Help functions for clients to get started
      
      Functions for posting a document
      
      Internal Data Structures
      
 */
#ifndef HTACCESS_H
#define HTACCESS_H

#include "HTList.h"
#include "HTChunk.h"
#include "HTAnchor.h"
#include "HTStream.h"
#include "HTAssoc.h"
#include "HTMethod.h"/*

Initializing and Terminating the Library

   [IMAGE]These two functions initiates memory and settings for the Library and cleans up
   memory kept by the Library when about to exit the application. It is highly recommended
   that they are used!
   
 */
extern BOOL HTLibInit NOPARAMS;
extern BOOL HTLibTerminate NOPARAMS;/*

Default WWW Addresses

   These control the home page selection. To mess with these for normal browses is asking
   for user confusion.
   
 */
#define LOGICAL_DEFAULT "WWW_HOME"            /* Defined to be the home page */

#ifndef PERSONAL_DEFAULT
#define PERSONAL_DEFAULT "WWW/default.html"             /* in home directory */
#endif

/* If the home page isn't found, use this file: */
#ifndef LAST_RESORT
#define LAST_RESORT     "http://www.w3.org/"
#endif

/* If one telnets to an access point it will look in this file for home page */
#ifndef REMOTE_POINTER
#define REMOTE_POINTER  "/etc/www-remote.url"               /* can't be file */
#endif

/* and if that fails it will use this. */
#ifndef REMOTE_ADDRESS
#define REMOTE_ADDRESS  "http://www.w3.org/"                /* can't be file */
#endif

/* Default log file name */
#ifndef DEFAULT_LOGFILE
#define DEFAULT_LOGFILE         "www-log"
#endif

#ifndef LOCAL_DEFAULT_FILE
#define LOCAL_DEFAULT_FILE "/usr/local/lib/WWW/default.html"
#endif

/* This is the default cache directory: */
#ifndef HT_CACHE_ROOT
#define HT_CACHE_ROOT           "/tmp"
#endif

/* The default directory for "save locally" and "save and execute" files: */
#ifndef HT_TMP_ROOT
#define HT_TMP_ROOT             "/tmp"
#endif/*

Global Flags

   Flags and variables which may be set to control the Library
   
  MAXIMUM NUMBER OF REDIRECTIONS
  
   The maximum number of redirections is pr. default 10. This prevents the library from
   going into an infinite loop which is kind of nice :-) It is normally not necessary to
   change the default value.
   
 */
extern int HTMaxRedirections;/*

  ALLOW ACCSS TO LOCAL FILE SYSTEM
  
   This flag can be used to deny an application to get access to the local file system
   (through cache, file URLs etc.)
   
 */
extern BOOL HTSecure;                   /* Disable security holes? *//*

  NAME OF REMOTE LOGIN HOST
  
   If an application is used for remote access (allowing telnet access, like for example
   the Line Mode Browser), then set this variable to the name of the remote host.
   
 */
extern char * HTClientHost;             /* Name or number of telnetting host *//*

  SERVER SPECIFIC FLAGS
  
   These two flags are set by a proxy and a server application respectfully. They tell the
   Library to skip some of the client application specific things.
   
 */
extern char * HTImServer;               /* If I'm cern_httpd */
extern BOOL HTImProxy;                  /* If I'm cern_httpd as a proxy *//*

Functions to Manipulate a HTRequest Structure

   Just to make things easier especially for clients, here are some functions to
   manipulate the request structure:
   
 */
typedef struct _HTRequest HTRequest;/*

  CREATE BLANK REQUEST
  
   This request has defaults in -- in most cases it will need some information added
   before being passed to HTAccess, but it will work as is for a simple request.
   
 */
extern HTRequest * HTRequest_new NOPARAMS;/*

  DELETE REQUEST STRUCTURE
  
   Frees also conversion list hanging from req->conversions.
   
 */
extern void HTRequest_delete PARAMS((HTRequest * req));/*

  POST WEB MANAGEMENT
  
 */
extern BOOL HTRequest_addDestination            PARAMS((HTRequest * src,
                                                        HTRequest * dest));
extern BOOL HTRequest_removeDestination         PARAMS((HTRequest * dest));

extern BOOL HTRequest_linkDestination           PARAMS((HTRequest * dest));
extern BOOL HTRequest_unlinkDestination         PARAMS((HTRequest * dest));

extern BOOL HTRequest_removePostWeb             PARAMS((HTRequest * me));
extern BOOL HTRequest_killPostWeb               PARAMS((HTRequest * me));

#define HTRequest_mainDestination(me) \
        ((me) && (me)->source ? (me)->source->mainDestination : NULL)
#define HTRequest_isDestination(me) \
        ((me) && (me)->source && (me) != (me)->source)
#define HTRequest_isMainDestination(me) \
        ((me) && (me)->source && \
        (me) == (me)->source->mainDestination)
#define HTRequest_isSource(me) \
        ((me) && (me)->source && (me) == (me)->source)
#define HTRequest_isPostWeb(me) \
        ((me) && (me)->source)/*

HTTP/MIME Header Methods

   These enumerations set up the headers that are used in a HTTP request OR a HTTP
   response.
   
  GENERAL HTTP HEADER MASK
  
   There are a few header fields which have general applicability for both request and
   response mesages, but which do not apply to the communication parties or theentity
   being transferred. This mask enables and disables these headers. If the bit is not
   turned on they are not sent. All headers are optional and the default value is NO
   GENERAL HEADERS
   
 */
typedef enum _GenHeaderEnum {
    HT_DATE             = 0x1,
    HT_FORWARDED        = 0x2,
    HT_MESSAGE_ID       = 0x4,
    HT_MIME             = 0x8
} GenHeaderEnum;

#define DEFAULT_GENERAL_HEADERS 0/*

  REQUEST HEADERS
  
   The request header fields allow the client to pass additional information about the
   request (and about the client itself) to the server. All headers are optional but the
   default value is all request headers if present except From and Pragma.
   
 */
typedef enum _ReqHeaderEnum {
    HT_ACCEPT_TYPE      = 0x1,
    HT_ACCEPT_CHAR      = 0x2,
    HT_ACCEPT_ENC       = 0x4,
    HT_ACCEPT_LAN       = 0x8,
    HT_FROM             = 0x10,
    HT_PRAGMA           = 0x20,
    HT_REFERER          = 0x40,
    HT_USER_AGENT       = 0x80
} ReqHeaderEnum;

#define DEFAULT_REQUEST_HEADERS \
HT_ACCEPT_TYPE+HT_ACCEPT_CHAR+HT_ACCEPT_ENC+HT_ACCEPT_LAN+HT_REFERER+HT_USER_AGENT/*

  ENTITY HEADER MASK
  
   The entity headers contain information about the object sent in the HTTP transaction.
   See the Anchor module, for the storage of entity headers. This flag defines which
   headers are to be sent in a request together with an entity body (the Ostands for
   object). All headers are optional but the default value is ALL ENTITY HEADERS IF
   PRESENT
   
 */
typedef enum _EntityHeaderEnum {
    HT_ALLOW            = 0x1,
    HT_CONTENT_ENCODING = 0x2,
    HT_CONTENT_LANGUAGE = 0x4,
    HT_CONTENT_LENGTH   = 0x8,
    HT_CTE              = 0x10,                 /* Content-Transfer-Encoding */
    HT_CONTENT_TYPE     = 0x20,
    HT_DERIVED_FROM     = 0x40,
    HT_EXPIRES          = 0x80,
    HT_LAST_MODIFIED    = 0x200,
    HT_LINK             = 0x400,
    HT_TITLE            = 0x800,
    HT_URI              = 0x1000,
    HT_VERSION          = 0x2000
} EntityHeaderEnum;

#define DEFAULT_ENTITY_HEADERS 0xFFFF/*

  USER DEFINED HEADERS
  
   Extra header can be generated when initializing the ExtraHeaders field.
   
Functions for Loading a Document

   There are several different ways of loading a document. However, the major difference
   between them is whether the document is referenced by
   
      Relative URI
      
      Absolute URI
      
      Anchor element or
      
      Contains keywords for searching an relative URI
      
      Contains keywords for searching an absolute URI
      
   NOTE: From release 3.0 of the Library, the return codes from the loading functions are
   no mode BOOL, that is YES or NO. Insted they have been replaced with the following set
   of return codes defined in the Utility module:
   
  HT_WOULD_BLOCK         An I/O operation would block
                         
  HT_ERROR               Error has occured
                         
  HT_LOADED              Success
                         
  HT_NO_DATA             Success, but no document loaded. This might be the situation when
                         a  telnet sesssion is started etc.
                         
  HT_RETRY               The remote server is down but will serve documents from the
                         calendar time indicated in HTRequest->retry_after.
                         
   However, a general rule about the return codes is that ERRORShave a negative value
   whereas SUCCESS has a positive value.
   
   There are also some functions to help the client getting started with the first URI.
   
  LOAD A DOCUMENT FROM RELATIVE URL
  
 */
extern int HTLoadRelative       PARAMS((CONST char *    relative_name,
                                        HTParentAnchor* here,
                                        HTRequest *     request));/*

  LOAD A DOCUMENT FROM ABSOLUTE URL
  
 */
extern int HTLoadAbsolute       PARAMS((CONST char *    addr,
                                        HTRequest *     request));/*

  LOAD A DOCUMENT FROM ABSOLUTE NAME TO A STREAM
  
 */
extern int HTLoadToStream       PARAMS((CONST char *    addr,
                                        BOOL            filter,
                                        HTRequest *     request));/*

  LOAD A DOCUMENT FROM ANCHOR
  
   The anchor parameter may be a child anchor. The anchor in the request is set to the
   parent anchor. The recursive function keeps the error stack in the request structure so
   that no information is lost having more than one call. See also HTBindAnchor().
   
 */
extern int HTLoadAnchor         PARAMS((HTAnchor  *     a,
                                        HTRequest *     request));
extern int HTLoadAnchorRecursive PARAMS((HTAnchor *     a,
                                        HTRequest *     request));/*

  LOAD A DOCUMENT
  
   These are two internal routines for loading a document which has an address AND a
   matching anchor.  (The public routines are called with one OR the other.)  This is
   recursively called from file load module to try ftp (though this will be obsolete in
   the next major release).
   
   If keep_error_stack is YES then the error (or info) stack is not cleared from the
   previous call.
   
 */
extern int HTLoad               PARAMS((HTRequest * request,
                                        BOOL keep_error_stack));/*

 */
extern BOOL HTLoadTerminate     PARAMS((HTRequest * request, int status));/*

  SEARCH USING RELATIVE URL
  
   Performs a search on word given by the user. Adds the search words to the end of the
   current address and attempts to open the new address.
   
 */
extern int HTSearch             PARAMS((CONST char *    keywords,
                                        HTParentAnchor* here,
                                        HTRequest *     request));/*

  SEARCH USING ABSOLUTE URL
  
   Performs a keyword search on word given by the user. Adds the keyword to the end of the
   current address and attempts to open the new address.
   
 */
extern int HTSearchAbsolute     PARAMS((CONST char *    keywords,
                                        CONST char *    indexname,
                                        HTRequest *     request));/*

Help Function for Clients to get started

   These function helps the client to load the first document. They are not mandatory to
   use - but they make life easier!
   
  BIND AN ANCHOR TO A REQUEST STRUCTURE WITHOUT LOADING
  
 */
extern BOOL HTBindAnchor PARAMS((HTAnchor *anchor, HTRequest *request));/*

  GENERATE THE ANCHOR FOR THE HOME PAGE
  
   As it involves file access, this should only be done once when the program first runs.
   This is a default algorithm using the WWW_HOME environment variable.
   
 */
extern HTParentAnchor * HTHomeAnchor NOPARAMS;/*

  FIND RELATED NAME
  
   Creates a local file URI that can be used as a relative name when calling HTParse() to
   expand a relative file name to an absolute one.
   
   The code for this routine originates from the Line Mode Browser and was moved here by
   howcome@w3.org in order for all clients to take advantage.
   
 */
extern char *  HTFindRelatedName NOPARAMS;/*

Functions for Posting a Document

   NOTE: The Posting functions are used to send a data object along with the request. The
   functions have the same set of return codes as for the Load Functions.
   
  GET A SAVE STREAM
  
    On Entry,
    
  request->anchor         is valid anchor which has previously beeing loaded
                         
    On exit,
    
  returns                 0 if error else a stream to save the object to.
                         
 */
extern HTStream * HTSaveStream  PARAMS((HTRequest * request));/*

  COPY AN ANCHOR
  
   Fetch the URL (possibly local file URL) and send it using either PUT or POST directly
   to the remote destination using HTTP, that is remote copy of object O from A to B where
   A might be the host of the application. The caller can decide the exact method used and
   which HTTP header fields to transmit by setting the user fields in the destination
   request structure.
   
 */
extern int HTCopyAnchor         PARAMS((HTAnchor *      src_anchor,
                                        HTRequest *     dest_req));/*

  UPLOAD AN ANCHOR
  
   Send the contents (in hyperdoc) of the source anchor using either PUT or POST to the
   remote destination using HTTP. The caller can decide the exact method used and which
   HTTP header fields to transmit by setting the user fields in the request structure.
   Format conversion can be made on the fly by setting the input_format field in the
   destination request structure. If the content-length is unknown (-1) then a
   content-length counter is automaticly put into the stream pipe.
   
 */
extern int HTUploadAnchor       PARAMS((HTAnchor *      src_anchor,
                                        HTParentAnchor *dest_anchor,
                                        HTRequest *     dest_req));/*

Internal Data Structures

   These are internal to the Library and should not normally be accessed directly.
   
  ACCESS AUTHENTICATION
  
   We need to define the following structures as they are used in the HTRequest structure.
   The AA module is declared in HTAAUtil and HTAABrow. The enumeration HTAAScheme
   represents the possible authentication schemes used by the WWW Access Authorization.
   
 */
typedef enum {
    HTAA_UNKNOWN,
    HTAA_NONE,
    HTAA_BASIC,
    HTAA_PUBKEY,
    HTAA_KERBEROS_V4,
    HTAA_KERBEROS_V5,
    HTAA_MAX_SCHEMES                            /* THIS MUST ALWAYS BE LAST! */
} HTAAScheme;

typedef struct _HTAARealm HTAARealm;
typedef struct _HTAASetup HTAASetup;/*

  BUFFERING FOR THE NETWORK
  
   This structure provides buffering for READ (and future WRITE) to the network. It is
   used by all the protocol modules. The size of the buffer, INPUT_BUFFER_SIZE, is a
   compromis between speed and memory.
   
 */
#define INPUT_BUFFER_SIZE 8192

typedef struct _HTInputSocket HTInputSocket;/*

  PROTOCOL SPECIFIC INFORMATION
  
   This structure contains information about socket number, input buffer for reading from
   the network etc. The structure is used through out the protocol modules and is the
   reference point for introducing multi threaded execution into the library, see
   specifications on Multiple Threads.
   
 */
typedef enum _SocAction {
    SOC_INVALID = -1,
    SOC_WRITE = 0,                              /* By default ready to write */
    SOC_READ,
    SOC_INTERRUPT
} SocAction;

typedef struct _HTNetInfo {
    SOCKFD              sockfd;                         /* Socket descripter */
    SockA               sock_addr;              /* SockA is defined in tcp.h */
    HTInputSocket *     isoc;                                /* Input buffer */
    SocAction           action;                 /* Result of the select call */
    HTStream *          target;                             /* Target stream */
    int                 addressCount;        /* Attempts if multi-homed host */
    time_t              connecttime;             /* Used on multihomed hosts */
    struct _HTRequest * request;           /* Link back to request structure */
} HTNetInfo;/*

   Note: The AddressCount varaible is used to count the number of attempt to connect to a
   multi-homed host so we know when to stop trying new IP-addresses.
   
  THE REQUEST STRUCTURE
  
   When a request is handled, all kinds of things about it need to be passed along.  These
   are all put into a HTRequest structure. This is the most essential structure in the
   library. It contains two main categories of information regarding a request:
   
      Application dependent information
      
      Library dependent information
      
   Applications using the Library should never use the internal library dependent
   information. It's only because we dont have real classes that we can't hide it.
   
   Note: If you reuse the request structure for more than one request then make sure that
   the request is re-initialized, so that no `old' data is reused, see functions to
   manipulate HTRequest Structure. The library handles its own internal information from
   request to request but the information set by the caller is untouched.
   
   The elements of the request structure are as follows:
   
 */
struct _HTRequest {/*

    Application Dependent - Set by the caller of HTAccess
    
 */
    HTMethod    method;/*

   An enum used to specify the HTTP method used for the actual request. The default value
   is GET.
   
      HTTP Header Information
      
 */
    HTList *    conversions;/*

   NULL, or a local list of specific conversions which the format manager can do in order
   to fulfill the request.  It typically points to a list set up on initialisation time
   for example by HTInit(). There is also a global list of conversions which contains a
   generic set of possible conversions.
   
 */
    HTList *    encodings;/*

   The list of encodings acceptable in the output stream.
   
 */
    HTList *    languages;/*

   The list of (human) language values acceptable in the response. The default is all
   languages.
   
 */
    HTList *    charsets;/*

   The list of charsets accepted by the application
   
 */
    GenHeaderEnum       GenMask;
    ReqHeaderEnum       RequestMask;
    EntityHeaderEnum    EntityMask;/*

   These bitmask variables defines which headers to include in a HTTP request (or any
   other MIME-like protocol). See header masks for more information on default values.
   
 */
    HTParentAnchor *parentAnchor;/*

   If this parameter is set then a `Referer: <parent address> can be generated in the
   request to the server, see Referer field in a HTTP Request
   
 */
   char * ExtraHeaders;/*

   Extra header information can be send along with a request using this variable. The text
   is sent as is so it must be preformatted with <CRLF> line terminators.
   
      Streams From Network to Application
      
 */
    HTStream *  output_stream; /*

   The output stream is to be used to put data down to as they come in from the network
   and back to the application. The default value is NULL which means that the stream goes
   to the user (display).
   
 */
    HTAtom *    output_format;/*

   The desired format of the output stream. This can be used to get unconverted data etc.
   from the library. If NULL, then WWW_PRESENT is default value.
   
 */
    HTStream*   error_stream;/*

   All object bodies sent from the server with status codes different from 200 OK will be
   put down this stream. This can be used as a debug window etc. If the value is NULL
   (default) then the stream used is HTBlackHole.
   
 */
    HTAtom *    error_format;/*

   The desired format of the error stream. This can be used to get unconverted data etc.
   from the library. The default value if WWW_HTML as a character based only has one
   WWW_PRESENT.
   
      Streams From Application to Network
      
 */
    HTStream *  input_stream; /*

   The input stream is to be used by the PostCallBackfunction to put data out on the
   network. The user should not initialize this field.
   
 */
    HTAtom *    input_format;/*

   The desired format of the output stream. This can be used to upload converted data to a
   remote server. If NULL, then WWW_SOURCE is default value.
   
 */
    int (*PostCallBack)         PARAMS((struct _HTRequest *     request,
                                        HTStream *              target));/*

   The call back function which is called when the current request is ready for sending
   (posting) the data object. The request is the current request so that the application
   knows which post we are handling. The function must have the same return values as the
   other Load functions.
   
      Other Flags
      
 */
    BOOL BlockingIO;
    BOOL ForceReload;
    BOOL ContentNegotiation;/*

   BlockingIO can be set to override if a protocol module is registered as using
   non-blocking IO, ForceReload will cancel any cached element, and ContentNegotioation
   will force content negotiation when looking for a local file. This is the default!
   
 */
    BOOL (* callback ) PARAMS((struct _HTRequest* request,
                                                void *param));/*

   A function to be called back in the event that a file has been saved to disk by
   HTSaveAndCallBack for example.
   
 */
    void *      context;/*

   An arbitrary pointer passed to HTAccess and passed back as a parameter to the callback.
   
    Library Dependent - Set by Library
    
   None of the bits below may be looked at by a WWW application. The Library handles the
   cleanup by itself.
   
 */
    HTParentAnchor*     anchor;/*

   The anchor for the object in question.  Set immediately by HTAcesss. Used by the
   protocol and parsing modules.  Valid thoughout the access.
   
 */
    HTChildAnchor *     childAnchor;    /* For element within the object  *//*

   The anchor for the sub object if any.  The object builder should ensure that is is
   selected, highlighted, etc when the object is loaded.
   
 */
    void *      using_cache;
    BOOL        using_proxy;/*

   Pointer to cache element if cache hit anfd if using proxy
   
 */
    BOOL        error_block;            /* YES if stream has been used    */
    HTList *    error_stack;            /* List of errors                 *//*

   These two fields are used by the error reporting system to keep a stack of messages.
   
 */
    char *      redirect;               /* Location or URI */
    int         redirections;           /* Number of redirections */
    time_t      retry_after;            /* Absolut time for a retry *//*

   We keep track of the number of redirections and also register if we have to repeat a
   request (if we received "5xx Retry After")
   
 */
    HTNetInfo * net_info;               /* Information about socket etc. *//*

   This structure contains protocol specific information, socket number etc.
   
 */
    HTRequest * source;                 /* Source for request or itself */
    HTRequest * mainDestination;        /* For the typical case */
    HTList *    destinations;           /* List of related requests */
    int         destRequests;           /* Number of destination requests */
    int         destStreams;            /* Number of destination streams *//*

   A simple model to bind related request structures together, for example the source and
   destination request in a PUT.
   
 */
    char *      WWWAAScheme;            /* WWW-Authenticate scheme */
    char *      WWWAARealm;             /* WWW-Authenticate realm */
    char *      WWWprotection;          /* WWW-Protection-Template */
    char *      authorization;          /* Authorization: field */
    HTAAScheme  scheme;                 /* Authentication scheme used *//*

   Access Authentication specific information.
   
 */
    HTInputSocket *     isoc;           /* InputSocket object for reading *//*

   This header is only used by the server and will be removed at some point.
   
 */
    HTList *    valid_schemes;          /* Valid auth.schemes             */
    HTAssocList **      scheme_specifics;/* Scheme-specific parameters    */
    char *      authenticate;           /* WWW-authenticate: field */
    char *      prot_template;          /* WWW-Protection-Template: field */
    HTAASetup * setup;                  /* Doc protection info            */
    HTAARealm * realm;                  /* Password realm                 */
    char *      dialog_msg;             /* Authentication prompt (client) *//*

   These fields are used by the HTTP access authentication used by a client application.
   
    Windows Specific Information
    
 */
#ifdef _WINDOWS
        HWND            hwnd;           /* Windows handle for MSWindows   */
        unsigned long   winMsg;         /* msg number of Windows eloop    */
#endif /* _WINDOWS *//*

 */
};/*

   End of Declaration
   
 */
#endif /* HTACCESS_H *//*

   end of HTAccess */

