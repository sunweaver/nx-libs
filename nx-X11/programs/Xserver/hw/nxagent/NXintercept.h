/* intercepting functions in Xserver/dix/extension.c */

#define ProcQueryExtension(x) nxagentProcQueryExtension(x)
#define ProcListExtensions(x) nxagentProcListExtensions(x)
