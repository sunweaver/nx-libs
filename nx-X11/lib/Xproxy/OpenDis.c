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

#define NEED_REPLIES
#define NEED_EVENTS
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/Xlib.h>
#include <nx-X11/Xproxy.h>

#ifdef NX_TRANS_SOCKET
/* extern void *_X11TransSocketProxyConnInfo(XtransConnInfo); */
#endif

#ifdef X_NOT_POSIX
#define Size_t unsigned int
#else
#define Size_t size_t
#endif

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
	return XOpenDisplay(display);
}
