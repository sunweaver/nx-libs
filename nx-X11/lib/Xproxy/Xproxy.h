#ifndef _Xproxy_h
#define _Xproxy_h

#include <X11/Xlib.h>

#ifndef _XPROXY_STRUCT_ONLY

#include <X11/Xfuncproto.h>
#include <X11/Xfuncs.h>

_XFUNCPROTOBEGIN

Display *XproxyOpenDisplay (
	_Xconst char *display
);
int XproxyCloseDisplay (
	Display *dpy
);
Bool XproxyCheckIfEventNoFlush(
	Display*		/* display */,
	XEvent*			/* event_return */,
	Bool (*) (
		Display*	/* display */,
		XEvent*		/* event */,
		XPointer	/* arg */
	)			/* predicate */,
	XPointer		/* arg */
);

void _XproxyFreeDisplayStructure(Display *dpy);

_XFUNCPROTOEND

#endif /* _XPROXY_STRUCT_ONLY */

#endif /* _Xproxy_h */
