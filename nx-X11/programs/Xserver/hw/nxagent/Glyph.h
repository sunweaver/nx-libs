/**************************************************************************/
/*                                                                        */
/* Copyright (c) 2001, 2011 NoMachine, http://www.nomachine.com/          */
/* Copyright (C) 2016 Qindel http://qindel.com and QVD http://theqvd.com  */
/* Copyright (C) 2016 Mike Gabriel <mike.gabriel@das-netzwerkteam.de>     */
/*                                                                        */
/* NXAGENT, NX protocol compression and NX extensions to this software    */
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

/*
 *
 * Copyright © 2000 SuSE, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of SuSE not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  SuSE makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * SuSE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL SuSE
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Keith Packard, SuSE, Inc.
 */

#ifndef __Glyph_H__
#define __Glyph_H__

#include "X11/include/Xrender_nxagent.h"

#include "Agent.h"

#ifdef NX_RENDER_CLEANUP
void
nxagentRenderCleanGlyphs(xGlyphInfo  *gi,
                         int         nglyphs,
                         CARD8       *images,
                         int         depth,
                         Display     *dpy);
#endif /* NX_RENDER_CLEANUP */

void
nxagentRenderCompositeText8 (Display		    *dpy,
		       int			    op,
		       Picture			    src,
		       Picture			    dst,
		       _Xconst XRenderPictFormat    *maskFormat,
		       int			    xSrc,
		       int			    ySrc,
		       int			    xDst,
		       int			    yDst,
		       _Xconst XGlyphElt8	    *elts,
		       int			    nelt);

void
nxagentRenderCompositeText16 (Display		    *dpy,
			int			    op,
			Picture			    src,
			Picture			    dst,
			_Xconst XRenderPictFormat   *maskFormat,
			int			    xSrc,
			int			    ySrc,
			int			    xDst,
			int			    yDst,
			_Xconst XGlyphElt16	    *elts,
			int			    nelt);

void
nxagentRenderCompositeText32 (Display			    *dpy,
			int			    op,
			Picture			    src,
			Picture			    dst,
			_Xconst XRenderPictFormat   *maskFormat,
			int			    xSrc,
			int			    ySrc,
			int			    xDst,
			int			    yDst,
			_Xconst XGlyphElt32	    *elts,
			int			    nelt);

#endif /* __Glyph_H__ */
