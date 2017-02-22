/**************************************************************************/
/*                                                                        */
/* Copyright (c) 2001, 2011 NoMachine (http://www.nomachine.com)          */
/* Copyright (c) 2008-2014 Oleksandr Shneyder <o.shneyder@phoca-gmbh.de>  */
/* Copyright (c) 2011-2016 Mike Gabriel <mike.gabriel@das-netzwerkteam.de>*/
/* Copyright (c) 2014-2016 Mihai Moldovan <ionic@ionic.de>                */
/* Copyright (c) 2014-2016 Ulrich Sibiller <uli42@gmx.de>                 */
/* Copyright (c) 2015-2016 Qindel Group (http://www.qindel.com)           */
/*                                                                        */
/* NXAGENT, NX protocol compression and NX extensions to this software    */
/* are copyright of the aforementioned persons and companies.             */
/*                                                                        */
/* Redistribution and use of the present software is allowed according    */
/* to terms specified in the file LICENSE which comes in the source       */
/* distribution.                                                          */
/*                                                                        */
/* All rights reserved.                                                   */
/*                                                                        */
/* NOTE: This software has received contributions from various other      */
/* contributors, only the core maintainers and supporters are listed as   */
/* copyright holders. Please contact us, if you feel you should be listed */
/* as copyright holder, as well.                                          */
/*                                                                        */
/**************************************************************************/

/* $XFree86: xc/programs/Xserver/GL/glx/glxext.c,v 1.9 2003/09/28 20:15:43 alanh Exp $
** The contents of this file are subject to the GLX Public License Version 1.0
** (the "License"). You may not use this file except in compliance with the
** License. You may obtain a copy of the License at Silicon Graphics, Inc.,
** attn: Legal Services, 2011 N. Shoreline Blvd., Mountain View, CA 94043
** or at http://www.sgi.com/software/opensource/glx/license.html.
**
** Software distributed under the License is distributed on an "AS IS"
** basis. ALL WARRANTIES ARE DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY
** IMPLIED WARRANTIES OF MERCHANTABILITY, OF FITNESS FOR A PARTICULAR
** PURPOSE OR OF NON- INFRINGEMENT. See the License for the specific
** language governing rights and limitations under the License.
**
** The Original Software is GLX version 1.2 source code, released February,
** 1999. The developer of the Original Software is Silicon Graphics, Inc.
** Those portions of the Subject Software created by Silicon Graphics, Inc.
** are Copyright (c) 1991-9 Silicon Graphics, Inc. All Rights Reserved.
**
*/

#include "../../GL/glx/glxext.c"

#include "Trap.h"

#define PANIC
#define WARNING
#undef  TEST
#undef  DEBUG

/*
** Top level dispatcher; all commands are executed from here down.
*/
static int __glXDispatch(ClientPtr client)
{
    REQUEST(xGLXSingleReq);
    CARD8 opcode;
    __GLXdispatchSingleProcPtr proc;
    __GLXclientState *cl;
    int retval;

    opcode = stuff->glxCode;
    cl = glxGetClient(client);
    /* Mark it in use so we suspend it on VT switch. */
    cl->inUse = TRUE;

    /*
    ** If we're expecting a glXRenderLarge request, this better be one.
    */
    if ((cl->largeCmdRequestsSoFar != 0) && (opcode != X_GLXRenderLarge)) {
    client->errorValue = stuff->glxCode;
    return __glXError(GLXBadLargeRequest);
    }

    /* If we're currently blocking GLX clients, just put this guy to
     * sleep, reset the request and return. */
    if (glxBlockClients) {
    ResetCurrentRequest(client);
    client->sequence--;
    IgnoreClient(client);
    return(client->noClientException);
    }

    /*
    ** Use the opcode to index into the procedure table.
    */
    proc = (__GLXdispatchSingleProcPtr) __glXGetProtocolDecodeFunction(& Single_dispatch_info,
				       opcode,
				       client->swapped);
    if (proc != NULL) {
    GLboolean rendering = opcode <= X_GLXRenderLarge;
    __glXleaveServer(rendering);

    __pGlxClient = client;


    /*
     * Report upstream that we are
     * dispatching a GLX operation.
     */

    nxagentGlxTrap = 1;

    #ifdef TEST
    fprintf(stderr, "__glXDispatch: Going to dispatch GLX operation [%d] for client [%d].\n", 
                opcode, client -> index);
    #endif

    retval = (*proc)(cl, (GLbyte *) stuff);

    nxagentGlxTrap = 0;

    #ifdef TEST
    fprintf(stderr, "__glXDispatch: Dispatched GLX operation [%d] for client [%d].\n", 
                opcode, client -> index);
    #endif

    __glXenterServer(rendering);
    }
    else {
    retval = BadRequest;
    }

    return retval;

}
