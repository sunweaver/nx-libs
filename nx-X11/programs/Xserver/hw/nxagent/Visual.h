/**************************************************************************/
/*                                                                        */
/* Copyright (c) 2001, 2011 NoMachine, http://www.nomachine.com/.         */
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

Copyright 1993 by Davor Matic

Permission to use, copy, modify, distribute, and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation.  Davor Matic makes no representations about
the suitability of this software for any purpose.  It is provided "as
is" without express or implied warranty.

*/

#ifndef __Visual_H__
#define __Visual_H__

Visual *nxagentVisual(VisualPtr pVisual);
Visual *nxagentVisualFromID(ScreenPtr pScreen, VisualID visual);
Visual *nxagentVisualFromDepth(ScreenPtr pScreen, int depth);

Colormap nxagentDefaultVisualColormap(Visual *visual);

#define nxagentDefaultVisual(pScreen) \
    nxagentVisualFromID((pScreen), (pScreen) -> rootVisual)

/*
 * Visual generated by Xorg and Xfree86 at
 * 16-bit depth differs on the bits_per_rgb
 * value, so we avoid checking it.
 */

/*
 * Some Solaris X servers uses the color
 * masks inverted, so that the red and
 * the blue mask are switched. To reconnect
 * the session on this displays, we do a
 * double check, as workaround.
 */

#define nxagentCompareVisuals(v1, v2)           \
    ((v1).depth         == (v2).depth &&        \
     /*(v1).bits_per_rgb  == (v2).bits_per_rgb &&*/ \
     (v1).class         == (v2).class &&        \
     ((v1).red_mask      == (v2).red_mask ||    \
      (v1).red_mask      == (v2).blue_mask) &&  \
     (v1).green_mask    == (v2).green_mask &&   \
     ((v1).blue_mask     == (v2).blue_mask ||   \
      (v1).blue_mask     == (v2).red_mask) &&   \
     (v1).colormap_size == (v2).colormap_size)

Visual nxagentAlphaVisual;

void nxagentInitAlphaVisual();

#endif /* __Visual_H__ */
