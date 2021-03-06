#ifndef TCL_WORKAROUND_H
#define TCL_WORKAROUND_H
// -*- mode:c++; c-basic-offset:4 -*-
/*<std-header orig-src='shore' incl-file-exclusion='TCL_WORKAROUND_h'>

 $Id: tcl_workaround.h,v 1.3 2010/06/23 23:44:31 nhall Exp $

SHORE -- Scalable Heterogeneous Object REpository

Copyright (c) 1994-99 Computer Sciences Department, University of
                      Wisconsin -- Madison
All Rights Reserved.

Permission to use, copy, modify and distribute this software and its
documentation is hereby granted, provided that both the copyright
notice and this permission notice appear in all copies of the
software, derivative works or modified versions, and any portions
thereof, and that both notices appear in supporting documentation.

THE AUTHORS AND THE COMPUTER SCIENCES DEPARTMENT OF THE UNIVERSITY
OF WISCONSIN - MADISON ALLOW FREE USE OF THIS SOFTWARE IN ITS
"AS IS" CONDITION, AND THEY DISCLAIM ANY LIABILITY OF ANY KIND
FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.

This software was developed with support by the Advanced Research
Project Agency, ARPA order number 018 (formerly 8230), monitored by
the U.S. Army Research Laboratory under contract DAAB07-91-C-Q518.
Further funding for this work was provided by DARPA through
Rome Research Laboratory Contract No. F30602-97-2-0247.

*/

#include "w_defines.h"

/*  -- do not edit anything above this line --   </std-header>*/

/* Tcl _vs_ Shore workarounds */

/*
 * It used to be that TCL had lots of arguments had things which
 * were logically 'const', but didn't have const decls.  That conflicted
 * with shore attempting to use 'const' to good effect.   So we
 * had the TCL_CVBUG which casted away constness for tcl, with the
 * understanding that it better darn well be const even though not
 * declared that way.
 *
 * Well, TCL eventually improved, and with the 8.4 release they added
 * 'const' declarations to a lot of things.  That is really great.
 * The only problem is that shore needs to be compatible with both old
 * and new TCLs.   So, I've made the following "compatability" suite
 * to try and classify the different const versus non-const things,
 * to allow shore to tcl interaction to be as const as possible.
 *
 * Quite likely this will break more in the future, but the new system
 * with "this particular thing" allows compatability to be adaptable...
 * so think of this as a base for the future, not a be-all and end-all
 * solution.
 */

/* A simple categorization of the macros:
   TCL_CBUG: const/non-const function returns
   TCL_CVBUG: function args which SHOULD be const but aren't
   TCL_AV:    argv in tcl command functions is/isn't const
   TCL_AV1:    ~TCL_AV for a few reverse decls in some cases (groan)
   TCL_SETV2:    SetVar 2nd (value) argument
   TCL_SETRES:    SetResult (result) argument
   TCL_SLIST:    SplitList vector
   TCL_GETX:    Return value of GetVar, GetStringResult
 */

#define    W_TCL_VER(major,minor)    ((major << 16) + (minor))

#define    W_THIS_TCL_VER    W_TCL_VER(TCL_MAJOR_VERSION, TCL_MINOR_VERSION)

// XXX this should be re-evaluated in the context of the newer TCL

#define    _TCL_CVBUG    (char *)

#if W_THIS_TCL_VER >= W_TCL_VER(8,4)
#define    TCL_CVBUG
#define    TCL_AV        const
#define    TCL_AV1
#define    TCL_SETV2
#define    TCL_SETRES    _TCL_CVBUG
#define    TCL_SLIST    const
#define    TCL_GETX    const
#else
#define TCL_CVBUG    _TCL_CVBUG
#define    TCL_AV
#define    TCL_AV1        TCL_CVBUG
#define    TCL_SETV2    TCL_CVBUG
#define    TCL_SETRES    TCL_CVBUG
#define    TCL_SLIST
#define    TCL_GETX
#endif

/*<std-footer incl-file-exclusion='TCL_WORKAROUND_H'>  -- do not edit anything below this line -- */

#endif          /*</std-footer>*/
