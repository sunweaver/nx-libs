/* $Xorg: OpenDis.c,v 1.4 2001/02/09 02:03:34 xorgcvs Exp $ */
/*

Copyright 1985, 1986, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.

*/

/**************************************************************************/
/*                                                                        */
/* Copyright (c) 2001, 2011 NoMachine, http://www.nomachine.com/.         */
/*                                                                        */
/* NX-X11, NX protocol compression and NX extensions to this software     */
/* are copyright of NoMachine. Redistribution and use of the present      */
/* software is allowed according to terms specified in the file LICENSE   */
/* which comes in the source distribution.                                */
/*                                                                        */
/* Check http://www.nomachine.com/licensing.html for applicability.       */
/*                                                                        */
/* NX and NoMachine are trademarks of Medialogic S.p.A.                   */
/*                                                                        */
/* All rights reserved.                                                   */
/*                                                                        */
/**************************************************************************/

/* $XFree86: xc/lib/X11/OpenDis.c,v 3.16 2003/07/04 16:24:23 eich Exp $ */

#include <stdlib.h>
#include <stdio.h>

#define NEED_REPLIES
#define NEED_EVENTS
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include "Xproxy.h"
#include "Xproxyintconn.h"

#ifdef NX_TRANS_SOCKET
/* extern void *_X11TransSocketProxyConnInfo(XtransConnInfo); */
#endif

#ifdef X_NOT_POSIX
#define Size_t unsigned int
#else
#define Size_t size_t
#endif

/* #define NX_TRANS_TEST */

/*
 * Connects to a server, creates a Display object and returns a pointer to
 * the newly created Display back to the caller.
 *
 * If the server is reachable directly, we use Xlib's XOpenDisplay() call.
 *
 * If the server is reachable over a proxy, we set up the connection ourselves.
 * in the NX_TRANS_SOCKET fashion.
 */
Display *
XOpenDisplayWithProxySupport (
	register _Xconst char *display)
{

	register Display *dpy;          /* New Display object being created. */
	char *display_name;
	char *fullname = NULL;          /* expanded name of display */
	int idisplay;                   /* display number */
	int iscreen;                    /* screen number */
	char *conn_auth_name, *conn_auth_data;
	int conn_auth_namelen, conn_auth_datalen;

	/*
	 * If the display specifier string supplied as an argument to this 
	 * routine is NULL or a pointer to NULL, read the DISPLAY variable.
	 */
	if (display == NULL || *display == '\0') {
		if ((display_name = getenv("DISPLAY")) == NULL) {
			/* Oops! No DISPLAY environment variable - error. */
			return(NULL);
		}
	}
	else {
		/* Display is non-NULL, copy the pointer */
		display_name = strdup(display);
		fprintf(stderr, "\n%s", display_name);
	}

#if defined(NX_TRANS_SOCKET) && defined(NX_TRANS_TEST)
	fprintf(stderr, "\nXOpenDisplayWithProxySupport: Called with display [%s].\n", display_name);
#endif

	if (!strncasecmp(display_name, "nx/", 3) || !strcasecmp(display_name, "nx") ||
	    !strncasecmp(display_name, "nx:", 3) || !strncasecmp(display_name, "nx,", 3))
	{

		/* Handle opening display via libXcomp */

#if defined(NX_TRANS_SOCKET) && defined(NX_TRANS_TEST)
		fprintf(stderr, "XOpenDisplayWithProxySupport: Obviously a session to be handled via libXcomp.\n");
#endif

		/*
		 * Set the default error handlers.  This allows the global variables to
		 * default to NULL for use with shared libraries.
		 */
		if (_XErrorFunction == NULL) (void) XSetErrorHandler(NULL);
		if (_XIOErrorFunction == NULL) (void) XSetIOErrorHandler(NULL);

		if ((dpy = (Display *)Xcalloc(1, sizeof(Display))) == NULL) {
            		return(NULL);
		}

		/*
		 * Call the Connect routine to get the transport connection object.
		 * If NULL is returned, the connection failed. The connect routine
		 * will set fullname to point to the expanded name.
		 */

		if ((dpy->trans_conn = _X11TransConnectDisplay(
		                                 display_name, &fullname, &idisplay,
		                                 &iscreen, &conn_auth_name,
		                                 &conn_auth_namelen, &conn_auth_data,
		                                 &conn_auth_datalen)) == NULL) {
			Xfree((char *) dpy);
			return(NULL);
		}

		dpy->fd = _X11TransGetConnectionNumber (dpy->trans_conn);

#if defined(NX_TRANS_SOCKET) && defined(NX_TRANS_TEST) 
		fprintf(stderr, "\nXOpenDisplayWithProxySupport: Connected display with dpy->fd = [%d].\n", dpy->fd);
#endif

		return(NULL);

	} else {

		/* Obviously a normal TCP / Unix Socket based X11 session, handing
		 * it over to libX11 from X.Org.
		 */

		return XOpenDisplay(display);
	}
}
