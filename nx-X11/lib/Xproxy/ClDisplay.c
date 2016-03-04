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
#include "Xproxyintconn.h"

/*
 * XCloseDisplay - XSync the connection to the X Server, close the connection,
 * and free all associated storage.  Extension close procs should only free
 * memory and must be careful about the types of requests they generate.
 */

int
XproxyCloseDisplay (
	Display *dpy)
{

	char **display_name = NULL;	 /* pointer to display name */

	*display_name = (char *)Xmalloc(strlen(dpy->display_name));
	strcpy(*display_name, dpy->display_name);

#if defined(NX_TRANS_SOCKET) && defined(NX_TRANS_TEST)
        fprintf(stderr, "\nXproxyCloseDisplay: Called with display [%s].\n", *display_name);
#endif

	if (!strncasecmp(*display_name, "nx/", 3) || !strcasecmp(*display_name, "nx") ||
	    !strncasecmp(*display_name, "nx:", 3) || !strncasecmp(*display_name, "nx,", 3))
	{

		/* Handle connection closure via libXcomp */
#if defined(NX_TRANS_SOCKET) && defined(NX_TRANS_TEST)
		fprintf(stderr, "XproxyCloseDisplay: Obviously a session to be handled via libXcomp.\n");
#endif

		register _XExtension *ext;
		register int i;

		if (!(dpy->flags & XlibDisplayClosing))
		{
			dpy->flags |= XlibDisplayClosing;
			for (i = 0; i < dpy->nscreens; i++) {
				register Screen *sp = &dpy->screens[i];
				XFreeGC (dpy, sp->default_gc);
			}
			if (dpy->cursor_font != None) {
				XUnloadFont (dpy, dpy->cursor_font);
			}
			XSync(dpy, 1);  /* throw away pending events, catch errors */
			/* call out to any extensions interested */
			for (ext = dpy->ext_procs; ext; ext = ext->next) {
				if (ext->close_display)
					(*ext->close_display)(dpy, &ext->codes);
			}
			/* if the closes generated more protocol, sync them up */
			if (dpy->request != dpy->last_request_read)
				XSync(dpy, 1);
		}
		_XDisconnectDisplay(dpy->trans_conn);
		_XproxyFreeDisplayStructure (dpy);
		return 0;

	} else {

		return XCloseDisplay(dpy);
	}
}
