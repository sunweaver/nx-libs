/*

Copyright 1993, 1994, 1998  The Open Group

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

/* Copyright 1993, 1994 NCR Corporation - Dayton, Ohio, USA
 *
 * All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name NCR not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  NCR makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * NCR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN
 * NO EVENT SHALL NCR BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef X_TRANSPROXYINT_H
#define X_TRANSPROXYINT_H

# ifndef TRANS_OPEN_MAX

#  ifndef X_NOT_POSIX
#   ifdef _POSIX_SOURCE
#    include <limits.h>
#   else
#    define _POSIX_SOURCE
#    include <limits.h>
#    undef _POSIX_SOURCE
#   endif
#  endif
#  ifndef OPEN_MAX
#   if defined(_SC_OPEN_MAX) && !defined(__UNIXOS2__)
#    define OPEN_MAX (sysconf(_SC_OPEN_MAX))
#   else
#    ifdef SVR4
#     define OPEN_MAX 256
#    else
#     include <sys/param.h>
#     ifndef OPEN_MAX
#      ifdef __OSF1__
#       define OPEN_MAX 256
#      else
#       ifdef NOFILE
#        define OPEN_MAX NOFILE
#       else
#        if !defined(__UNIXOS2__) && !defined(__QNX__)
#         define OPEN_MAX NOFILES_MAX
#        else
#         define OPEN_MAX 256
#        endif
#       endif
#      endif
#     endif
#    endif
#   endif
#  endif
#  if defined(_SC_OPEN_MAX)
#   define TRANS_OPEN_MAX OPEN_MAX
#  else /* !__GNU__ */
#   if OPEN_MAX > 256
#    define TRANS_OPEN_MAX 256
#   else
#    define TRANS_OPEN_MAX OPEN_MAX
#   endif
#  endif /*__GNU__*/

# endif /* TRANS_OPEN_MAX */

#endif /* X_TRANSPROXYINT_H */

#ifdef XPROXY_t
#define X_TCP_PORT 6000
#endif