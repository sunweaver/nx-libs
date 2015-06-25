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

#include "NXLibRender_Xrenderint.h"

#define NX_RENDER_CLEANUP

#ifdef NX_RENDER_CLEANUP
#include <stdio.h>
#define ROUNDUP(nbits, pad) ((((nbits) + ((pad)-1)) / (pad)) * ((pad)>>3))
#endif /* NX_RENDER_CLEANUP */

#ifdef NX_RENDER_CLEANUP
void
NXRenderCleanGlyphs(xGlyphInfo  *gi,
                    int         nglyphs,
                    CARD8       *images,
                    int         depth,
                    Display     *dpy)
{

  int widthInBits;
  int bytesPerLine;
  int bytesToClean;
  int bitsToClean;
  int widthInBytes;
  int height = gi -> height;
  register int i;
  int j;

  #ifdef NXAGENT_DEBUG
  fprintf(stderr, "NXRenderCleanGlyphs: Found a Glyph with Depth %d, width %d, pad %d.\n",
          depth, gi -> width, dpy -> bitmap_pad);
  #endif

  while (nglyphs > 0)
  {
    if (depth == 24)
    {
      widthInBits = gi -> width * 32;

      bytesPerLine = ROUNDUP(widthInBits, dpy -> bitmap_pad);

      bytesToClean = bytesPerLine * height;

      #ifdef DUBUG
      fprintf(stderr, "NXRenderCleanGlyphs: Found glyph with depth 24, bytes to clean is %d"
              "width in bits is %d bytes per line [%d] height [%d].\n", bytesToClean,
                      widthInBits, bytesPerLine, height);
      #endif

      if (dpy -> byte_order == LSBFirst)
      {
        for (i = 3; i < bytesToClean; i += 4)
        {
          images[i] = 0x00;
        }
      }
      else
      {
        for (i = 0; i < bytesToClean; i += 4)
        {
          images[i] = 0x00;
        }
      }

      #ifdef NXAGENT_DUMP
      fprintf(stderr, "NXRenderCleanGlyphs: depth %d, bytesToClean %d, scanline: ", depth, bytesToClean);
      for (i = 0; i < bytesPerLine; i++)
      {
        fprintf(stderr, "[%d]", images[i]);
      }
      fprintf(stderr,"\n");
      #endif

      images += bytesToClean;

      gi++;

      nglyphs--;
    }
    else if (depth == 1)
    {
      widthInBits = gi -> width;

      bytesPerLine = ROUNDUP(widthInBits, dpy -> bitmap_pad);

      bitsToClean = (bytesPerLine << 3) - (gi -> width);

      #ifdef NXAGENT_DEBUG
      fprintf(stderr, "NXRenderCleanGlyphs: Found glyph with depth 1, width [%d], height [%d], bitsToClean [%d]," 
              " bytesPerLine [%d].\n", gi -> width, height, bitsToClean, bytesPerLine);
      #endif

      bytesToClean = bitsToClean >> 3;

      bitsToClean &= 7;

      #ifdef NXAGENT_DEBUG
      fprintf(stderr, "NXRenderCleanGlyphs: bitsToClean &=7 is %d, bytesToCLean is %d."
              " byte_order is %d, bitmap_bit_order is %d.\n", bitsToClean, bytesToClean,
              dpy -> byte_order, dpy -> bitmap_bit_order);
      #endif

      for (i = 1; i <= height; i++)
      {
        if (dpy -> byte_order == dpy -> bitmap_bit_order)
        {
          for (j = 1; j <= bytesToClean; j++)
          {
            images[i * bytesPerLine - j] = 0x00;

            #ifdef NXAGENT_DEBUG
            fprintf(stderr, "NXRenderCleanGlyphs: byte_order = bitmap_bit_orde, cleaning %d, i=%d, j=%d.\n"
                    , (i * bytesPerLine - j), i, j);
            #endif

          }
        }
        else
        {
          for (j = bytesToClean; j >= 1; j--)
          {
            images[i * bytesPerLine - j] = 0x00;

            #ifdef NXAGENT_DEBUG
            fprintf(stderr, "NXRenderCleanGlyphs: byte_order %d, bitmap_bit_order %d, cleaning %d, i=%d, j=%d.\n"
                    , dpy -> byte_order, dpy -> bitmap_bit_order, (i * bytesPerLine - j), i, j);
            #endif

          }
        }

        if (dpy -> bitmap_bit_order == MSBFirst)
        {
          images[i * bytesPerLine - j] &= 0xff << bitsToClean;

          #ifdef NXAGENT_DEBUG
          fprintf(stderr, "NXRenderCleanGlyphs: byte_order MSBFirst, cleaning %d, i=%d, j=%d.\n"
                  , (i * bytesPerLine - j), i, j);
          #endif
        }
        else
        {
          images[i * bytesPerLine - j] &= 0xff >> bitsToClean;

          #ifdef NXAGENT_DEBUG
          fprintf(stderr, "NXRenderCleanGlyphs: byte_order LSBFirst, cleaning %d, i=%d, j=%d.\n"
                  , (i * bytesPerLine - j), i, j);
          #endif
        }
      }
  
      #ifdef NXAGENT_DUMP
      fprintf(stderr, "NXRenderCleanGlyphs: depth %d, bytesToClean %d, scanline: ", depth, bytesToClean);
      for (i = 0; i < bytesPerLine; i++)
      {
        fprintf(stderr, "[%d]", images[i]);
      }
      fprintf(stderr,"\n");
      #endif

      images += bytesPerLine * height;

      gi++;

      nglyphs--;
    }
    else if ((depth == 8) || (depth == 16) )
    {
      widthInBits = gi -> width * depth;

      bytesPerLine = ROUNDUP(widthInBits, dpy -> bitmap_pad);

      widthInBytes = (widthInBits >> 3);

      bytesToClean = bytesPerLine - widthInBytes;

      #ifdef NXAGENT_DEBUG
      fprintf(stderr, "NXRenderCleanGlyphs: nglyphs is %d, width of glyph in bits is %d, in bytes is %d.\n",
              nglyphs, widthInBits, widthInBytes);

      fprintf(stderr, "NXRenderCleanGlyphs: bytesPerLine is %d bytes, there are %d scanlines.\n", bytesPerLine, height);

      fprintf(stderr, "NXRenderCleanGlyphs: Bytes to clean for each scanline are %d.\n", bytesToClean);
      #endif

      if (bytesToClean > 0)
      {
        while (height > 0)
        {
          i = bytesToClean;

          while (i > 0)
          {
            *(images + (bytesPerLine - i)) = 0;

            #ifdef NXAGENT_DEBUG
            fprintf(stderr, "NXRenderCleanGlyphs: cleaned a byte.\n");
            #endif

            i--;
          }

          #ifdef NXAGENT_DUMP
          fprintf(stderr, "NXRenderCleanGlyphs: depth %d, bytesToClean %d, scanline: ", depth, bytesToClean);
          for (i = 0; i < bytesPerLine; i++)
          {
            fprintf(stderr, "[%d]", images[i]);
          }
          fprintf(stderr,"\n");
          #endif

          images += bytesPerLine;

          height--;
        }
      }

      gi++;

      nglyphs--;

      #ifdef NXAGENT_DEBUG
      fprintf(stderr, "NXRenderCleanGlyphs: Breaking Out.\n");
      #endif
    }
    else if (depth == 32)
    {
      #ifdef NXAGENT_DEBUG
      fprintf(stderr, "NXRenderCleanGlyphs: Found glyph with depth 32.\n");
      #endif

      gi++;

      nglyphs--;
    }
    else
    {
      #ifdef WARNING
      fprintf(stderr, "NXRenderCleanGlyphs: Unrecognized glyph, depth is not 8/16/24/32, it appears to be %d.\n",
              depth);
      #endif

      gi++;

      nglyphs--;
    }
  }
}
#endif /* NX_RENDER_CLEANUP */

void
NXRenderCompositeText8 (Display			    *dpy,
		        int			    op,
		        Picture			    src,
		        Picture			    dst,
		        _Xconst XRenderPictFormat    *maskFormat,
		        int			    xSrc,
		        int			    ySrc,
		        int			    xDst,
		        int			    yDst,
		        _Xconst XGlyphElt8	    *elts,
		        int			    nelt)
{
    XRenderExtDisplayInfo	*info = XRenderFindDisplay (dpy);
    xRenderCompositeGlyphs8Req	*req;
    GlyphSet			glyphset;
    long			len;
    long			elen;
    xGlyphElt			*elt;
    int				i;
    _Xconst char		*chars;
    int				nchars;
#ifdef NX_RENDER_CLEANUP
    char			tmpChar[4];
    int				bytes_to_clean;
    int				bytes_to_write;
#endif

    if (!nelt)
	return;

    RenderSimpleCheckExtension (dpy, info);
    LockDisplay(dpy);

    GetReq(RenderCompositeGlyphs8, req);
    req->reqType = info->codes->major_opcode;
    req->renderReqType = X_RenderCompositeGlyphs8;
    req->op = op;
    req->src = src;
    req->dst = dst;
    req->maskFormat = maskFormat ? maskFormat->id : None;
    req->glyphset = elts[0].glyphset;
    req->xSrc = xSrc;
    req->ySrc = ySrc;

    /*
     * Compute the space necessary
     */
    len = 0;

#define MAX_8 252

    glyphset = elts[0].glyphset;
    for (i = 0; i < nelt; i++)
    {
	/*
	 * Check for glyphset change
	 */
	if (elts[i].glyphset != glyphset)
	{
	    glyphset = elts[i].glyphset;
	    len += (SIZEOF (xGlyphElt) + 4) >> 2;
	}
	nchars = elts[i].nchars;
	/*
	 * xGlyphElt must be aligned on a 32-bit boundary; this is
	 * easily done by filling no more than 252 glyphs in each
	 * bucket
	 */
	elen = SIZEOF(xGlyphElt) * ((nchars + MAX_8-1) / MAX_8) + nchars;
	len += (elen + 3) >> 2;
    }

    req->length += len;

    /*
     * Send the glyphs
     */
    glyphset = elts[0].glyphset;
    for (i = 0; i < nelt; i++)
    {
	/*
	 * Switch glyphsets
	 */
	if (elts[i].glyphset != glyphset)
	{
	    glyphset = elts[i].glyphset;
	    BufAlloc (xGlyphElt *, elt, SIZEOF (xGlyphElt));

#ifdef NX_RENDER_CLEANUP
	    elt->pad1 = 0;
	    elt->pad2 = 0;
#endif /* NX_RENDER_CLEANUP */

	    elt->len = 0xff;
	    elt->deltax = 0;
	    elt->deltay = 0;
	    Data32(dpy, &glyphset, 4);
	}
	nchars = elts[i].nchars;
	xDst = elts[i].xOff;
	yDst = elts[i].yOff;
	chars = elts[i].chars;
	while (nchars)
	{
	    int this_chars = nchars > MAX_8 ? MAX_8 : nchars;

	    BufAlloc (xGlyphElt *, elt, SIZEOF(xGlyphElt))

#ifdef NX_RENDER_CLEANUP
	    elt->pad1 = 0;
	    elt->pad2 = 0;
#endif /* NX_RENDER_CLEANUP */

	    elt->len = this_chars;
	    elt->deltax = xDst;
	    elt->deltay = yDst;
	    xDst = 0;
	    yDst = 0;

#ifdef NX_RENDER_CLEANUP
	    bytes_to_write = this_chars & ~3;

	    bytes_to_clean = ((this_chars + 3) & ~3) - this_chars;

	    #ifdef NXAGENT_DEBUG
	    fprintf(stderr, "NXRenderCompositeText8: bytes_to_write %d, bytes_to_clean are %d,"
	                    " this_chars %d.\n", bytes_to_write, bytes_to_clean, this_chars);
	    #endif

	    if (bytes_to_clean > 0)
	    {
		if (bytes_to_write > 0)
		{
		    #ifdef NXAGENT_DEBUG
		    fprintf(stderr, "NXRenderCompositeText8: found %d clean bytes, bytes_to_clean are %d,"
		                    " this_chars %d.\n", bytes_to_write, bytes_to_clean, this_chars);
		    #endif

		    Data (dpy, chars, bytes_to_write);
		    chars += bytes_to_write;
		}
	
		bytes_to_write = this_chars % 4;
		memcpy (tmpChar, chars, bytes_to_write);
		chars += bytes_to_write;

		#ifdef NXAGENT_DEBUG
		fprintf(stderr, "NXRenderCompositeText8: last 32 bit, bytes_to_write are %d,"
		                " bytes_to_clean are %d, this_chars are %d.\n", bytes_to_write, bytes_to_clean, this_chars);
		#endif

		#ifdef NXAGENT_DUMP
		fprintf(stderr, "NXRenderCompositeText8: bytes_to_clean %d, ", bytes_to_clean);
		#endif
	
		while (bytes_to_clean > 0)
		{
		    tmpChar[4 - bytes_to_clean] = 0;
		    bytes_to_clean--;

		    #ifdef NXAGENT_DEBUG
		    fprintf(stderr, "NXRenderCompositeText8: Cleaned %d byte.\n", 4 - bytes_to_clean);
		    #endif
		}

		Data (dpy, tmpChar, 4);
		nchars -= this_chars;

		#ifdef NXAGENT_DUMP
		fprintf(stderr, "Data: ");
		for (i = 0; i < 4; i++)
		{
		    fprintf(stderr, "[%d]", tmpChar[i]);
		}
		fprintf(stderr,"\n");
		#endif

		#ifdef NXAGENT_DEBUG
		fprintf(stderr, "NXRenderCompositeText8: nchars now is %d.\n", nchars);
		#endif

		continue;
	    }
#endif /* NX_RENDER_CLEANUP */

	    Data (dpy, chars, this_chars);
	    nchars -= this_chars;
	    chars += this_chars;
	}
    }
#undef MAX_8

    UnlockDisplay(dpy);
    SyncHandle();
}

void
NXRenderCompositeText16 (Display		    *dpy,
			 int			    op,
			 Picture		    src,
			 Picture		    dst,
			 _Xconst XRenderPictFormat  *maskFormat,
			 int			    xSrc,
			 int			    ySrc,
			 int			    xDst,
			 int			    yDst,
			 _Xconst XGlyphElt16	    *elts,
			 int			    nelt)
{
    XRenderExtDisplayInfo		*info = XRenderFindDisplay (dpy);
    xRenderCompositeGlyphs16Req	*req;
    GlyphSet			glyphset;
    long			len;
    long			elen;
    xGlyphElt			*elt;
    int				i;
    _Xconst unsigned short    	*chars;
    int				nchars;
#ifdef NX_RENDER_CLEANUP
    int				bytes_to_write;
    int				bytes_to_clean;
    char			tmpChar[4];
#endif /* NX_RENDER_CLEANUP */

    if (!nelt)
	return;

    RenderSimpleCheckExtension (dpy, info);
    LockDisplay(dpy);

    GetReq(RenderCompositeGlyphs16, req);
    req->reqType = info->codes->major_opcode;
    req->renderReqType = X_RenderCompositeGlyphs16;
    req->op = op;
    req->src = src;
    req->dst = dst;
    req->maskFormat = maskFormat ? maskFormat->id : None;
    req->glyphset = elts[0].glyphset;
    req->xSrc = xSrc;
    req->ySrc = ySrc;

    /*
     * Compute the space necessary
     */
    len = 0;

#define MAX_16	254

    glyphset = elts[0].glyphset;
    for (i = 0; i < nelt; i++)
    {
	/*
	 * Check for glyphset change
	 */
	if (elts[i].glyphset != glyphset)
	{
	    glyphset = elts[i].glyphset;
	    len += (SIZEOF (xGlyphElt) + 4) >> 2;
	}
	nchars = elts[i].nchars;
	/*
	 * xGlyphElt must be aligned on a 32-bit boundary; this is
	 * easily done by filling no more than 254 glyphs in each
	 * bucket
	 */
	elen = SIZEOF(xGlyphElt) * ((nchars + MAX_16-1) / MAX_16) + nchars * 2;
	len += (elen + 3) >> 2;
    }

    req->length += len;

    glyphset = elts[0].glyphset;
    for (i = 0; i < nelt; i++)
    {
	/*
	 * Switch glyphsets
	 */
	if (elts[i].glyphset != glyphset)
	{
	    glyphset = elts[i].glyphset;
	    BufAlloc (xGlyphElt *, elt, SIZEOF (xGlyphElt));
#ifdef NX_RENDER_CLEANUP
	    elt->pad1 = 0;
	    elt->pad2 = 0;
#endif /* NX_RENDER_CLEANUP */
	    elt->len = 0xff;
	    elt->deltax = 0;
	    elt->deltay = 0;
	    Data32(dpy, &glyphset, 4);
	}
	nchars = elts[i].nchars;
	xDst = elts[i].xOff;
	yDst = elts[i].yOff;
	chars = elts[i].chars;
	while (nchars)
	{
	    int this_chars = nchars > MAX_16 ? MAX_16 : nchars;
	    int this_bytes = this_chars * 2;

	    BufAlloc (xGlyphElt *, elt, SIZEOF(xGlyphElt))
#ifdef NX_RENDER_CLEANUP
	    elt->pad1 = 0;
	    elt->pad2 = 0;
#endif /* NX_RENDER_CLEANUP */
	    elt->len = this_chars;
	    elt->deltax = xDst;
	    elt->deltay = yDst;
	    xDst = 0;
	    yDst = 0;

#ifdef NX_RENDER_CLEANUP
	    bytes_to_write = this_bytes & ~3;
	    bytes_to_clean = ((this_bytes + 3) & ~3) - this_bytes;

	    #ifdef NXAGENT_DEBUG
	    fprintf(stderr, "NXRenderCompositeText16: this_chars %d, this_bytes %d.\n"
		            "bytes_to_write %d, bytes_to_clean are %d.\n", this_chars, this_bytes,
		             bytes_to_write, bytes_to_clean);
	    #endif

	    if (bytes_to_clean > 0)
	    {
		if (bytes_to_write > 0)
		{
		    Data16 (dpy, chars, bytes_to_write);

		    /*
		     * Cast chars to avoid errors with pointer arithmetic.
		     */

		    chars = (unsigned short *) ((char *) chars + bytes_to_write);
		}

		bytes_to_write = this_bytes % 4;
		memcpy (tmpChar, (char *) chars, bytes_to_write);
		chars = (unsigned short *) ((char *) chars + bytes_to_write);

		#ifdef NXAGENT_DEBUG
		fprintf(stderr, "NXRenderCompositeText16: last 32 bit, bytes_to_write are %d,"
		                " bytes_to_clean are %d.\n", bytes_to_write, bytes_to_clean);
		#endif

		while (bytes_to_clean > 0)
		{
		    tmpChar[4 - bytes_to_clean] = 0;
		    bytes_to_clean--;

		    #ifdef NXAGENT_DEBUG
		    fprintf(stderr, "NXRenderCompositeText16: Cleaned %d byte.\n", 4 - bytes_to_clean);
		    #endif
		}

		Data16 (dpy, tmpChar, 4);
		nchars -= this_chars;

		#ifdef NXAGENT_DEBUG
		fprintf(stderr, "NXRenderCompositeText16: nchars now is %d.\n", nchars);
		#endif

		continue;
	    }
#endif /* NX_RENDER_CLEANUP */

	    Data16 (dpy, chars, this_bytes);
	    nchars -= this_chars;
	    chars += this_chars;
	}
    }
#undef MAX_16

    UnlockDisplay(dpy);
    SyncHandle();
}

void
NXRenderCompositeText32 (Display		    *dpy,
			 int			    op,
			 Picture		    src,
			 Picture		    dst,
			 _Xconst XRenderPictFormat  *maskFormat,
			 int			    xSrc,
			 int			    ySrc,
			 int			    xDst,
			 int			    yDst,
			 _Xconst XGlyphElt32	    *elts,
			 int			    nelt)
{
    XRenderExtDisplayInfo		*info = XRenderFindDisplay (dpy);
    xRenderCompositeGlyphs32Req	*req;
    GlyphSet			glyphset;
    long			len;
    long			elen;
    xGlyphElt			*elt;
    int				i;
    _Xconst unsigned int    	*chars;
    int				nchars;

    if (!nelt)
	return;

    RenderSimpleCheckExtension (dpy, info);
    LockDisplay(dpy);


    GetReq(RenderCompositeGlyphs32, req);
    req->reqType = info->codes->major_opcode;
    req->renderReqType = X_RenderCompositeGlyphs32;
    req->op = op;
    req->src = src;
    req->dst = dst;
    req->maskFormat = maskFormat ? maskFormat->id : None;
    req->glyphset = elts[0].glyphset;
    req->xSrc = xSrc;
    req->ySrc = ySrc;

    /*
     * Compute the space necessary
     */
    len = 0;

#define MAX_32	254

    glyphset = elts[0].glyphset;
    for (i = 0; i < nelt; i++)
    {
	/*
	 * Check for glyphset change
	 */
	if (elts[i].glyphset != glyphset)
	{
	    glyphset = elts[i].glyphset;
	    len += (SIZEOF (xGlyphElt) + 4) >> 2;
	}
	nchars = elts[i].nchars;
	elen = SIZEOF(xGlyphElt) * ((nchars + MAX_32-1) / MAX_32) + nchars *4;
	len += (elen + 3) >> 2;
    }

    req->length += len;

    glyphset = elts[0].glyphset;
    for (i = 0; i < nelt; i++)
    {
	/*
	 * Switch glyphsets
	 */
	if (elts[i].glyphset != glyphset)
	{
	    glyphset = elts[i].glyphset;
	    BufAlloc (xGlyphElt *, elt, SIZEOF (xGlyphElt));
#ifdef NX_RENDER_CLEANUP
	    elt->pad1 = 0;
	    elt->pad2 = 0;
#endif /* NX_RENDER_CLEANUP */
	    elt->len = 0xff;
	    elt->deltax = 0;
	    elt->deltay = 0;
	    Data32(dpy, &glyphset, 4);
	}
	nchars = elts[i].nchars;
	xDst = elts[i].xOff;
	yDst = elts[i].yOff;
	chars = elts[i].chars;
	while (nchars)
	{
	    int this_chars = nchars > MAX_32 ? MAX_32 : nchars;
	    int this_bytes = this_chars * 4;
	    BufAlloc (xGlyphElt *, elt, SIZEOF(xGlyphElt))
#ifdef NX_RENDER_CLEANUP
	    elt->pad1 = 0;
	    elt->pad2 = 0;
#endif /* NX_RENDER_CLEANUP */
	    elt->len = this_chars;
	    elt->deltax = xDst;
	    elt->deltay = yDst;
	    xDst = 0;
	    yDst = 0;
	    DataInt32 (dpy, chars, this_bytes);
	    nchars -= this_chars;
	    chars += this_chars;
	}
    }
#undef MAX_32

    UnlockDisplay(dpy);
    SyncHandle();
}
