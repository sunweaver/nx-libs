#ifndef _Xproxy_h
#define _Xproxy_h

#include <X11/Xlib.h>

#ifndef _XPROXY_STRUCT_ONLY

# include   <X11/Xfuncproto.h>
# include   <X11/Xfuncs.h>

_XFUNCPROTOBEGIN

Display *XOpenDisplayWithProxySupport(
	_Xconst char *display
);
int XCloseDisplayWithProxySupport (
	Display *dpy
);
Bool XCheckIfEventNoFlush(
	Display*		/* display */,
	XEvent*			/* event_return */,
	Bool (*) (
		Display*	/* display */,
		XEvent*		/* event */,
		XPointer	/* arg */
	)			/* predicate */,
	XPointer		/* arg */
);

_XFUNCPROTOEND

#endif /* _XPROXY_STRUCT_ONLY */

#endif /* _Xproxy_h */
