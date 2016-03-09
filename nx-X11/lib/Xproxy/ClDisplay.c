/*

Copyright 1985, 1990, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from The Open Group.

*/

#include <stdio.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xlibint.h>

#include "Xproxy.h"

/* #define NX_TRANS_TEST */

/*
 * XCloseDisplay - XSync the connection to the X Server, close the connection,
 * and free all associated storage.  Extension close procs should only free
 * memory and must be careful about the types of requests they generate.
 */

int
XCloseDisplayWithProxySupport (
	register Display *dpy)
{

	char **display_name;		 /* pointer to display name */

	*display_name = (char *)Xmalloc(strlen(dpy->display_name));
	strcpy(*display_name, dpy->display_name);

#if defined(NX_TRANS_SOCKET) && defined(NX_TRANS_TEST)
        fprintf(stderr, "\nXCloseDisplayWithProxySupport: Called with display [%s].\n", *display_name);
#endif

	if (!strncasecmp(*display_name, "nx/", 3) || !strcasecmp(*display_name, "nx") ||
	    !strncasecmp(*display_name, "nx:", 3) || !strncasecmp(*display_name, "nx,", 3))
	{

		/* Handle connection closure via libXcomp */
#if defined(NX_TRANS_SOCKET) && defined(NX_TRANS_TEST)
		fprintf(stderr, "XCloseDisplayWithProxySupport: Obviously a session to be handled via libXcomp.\n");
#endif

		return 0;

	} else {

		return XCloseDisplay(dpy);
	}
}
