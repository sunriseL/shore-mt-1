#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(shore-sm, 6.0.2, nhall@cs.wisc.edu.)
AC_CONFIG_HEADERS([config/shore-config.h:config/shore-config-h.in])
AC_CONFIG_SRCDIR([config/shore.def])
AC_COPYRIGHT(COPYRIGHT-NOTICE)
AC_CONFIG_AUX_DIR(config)
##
## Note that AM_INIT_AUTOMAKE is required.
## Turn on all automake warnings and treat them as errors.
## 'foreign' says that we don't follow all GNU standards
## For some reason, dependency tracking is not enabled by default
## on our Solaris box.
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

## This package uses an assembler.
## Define CCAS and CCASFLAGS 
AM_PROG_AS

## Add dependencies
AC_SUBST([CONFIG_STATUS_DEPENDENCIES],['$(top_srcdir)/configure.ac'])

# Checks for programs.
## preprocessor required
AC_PROG_CXXCPP
AC_REQUIRE_CPP

using_gcc=$ac_cv_cxx_compiler_gnu
## echo using gcc $using_gcc

AC_LANG_PUSH(C++)
if [test "x$using_gcc" = xyes]; then
    # -------------------------------------------------------------------------
    #
    # What compiler is indicated by CXX
    #
    # -------------------------------------------------------------------------
    AC_MSG_CHECKING([GCC-specific checks])
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
    [[
    #if defined(__GNUC__) 
    #else
    #error "not GNU  CC"
    #endif
    ]])], 
    [AM_CONDITIONAL([COMPILER_IS_GCC], [test -n "1"]) AC_MSG_RESULT[] ],
    [AM_CONDITIONAL([COMPILER_IS_GCC], [test -z "1"])  AC_MSG_RESULT[]]
    )
    # empty msg result above gives us a newline in the output

    # here, with certainty not sunCC or unknown
    AM_CONDITIONAL([COMPILER_IS_SUNCC], [test -z "1"])
    AM_CONDITIONAL([COMPILER_IS_UNKNOWN], [test -n "1"]) 


    AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
    [[
    #if defined(__INTEL_COMPILER) 
    #else
    #error "not GNU  ICC"
    #endif
    ]])], 
    [AM_CONDITIONAL([COMPILER_IS_ICC], [test -n "1"]) AC_MSG_RESULT[] ],
    [AM_CONDITIONAL([COMPILER_IS_ICC], [test -z "1"])  AC_MSG_RESULT[]]
    )
    # empty msg result above gives us a newline in the output

    # -------------------------------------------------------------------------
    #
    # GCC_BROKEN_WARNINGS
    # Needed at config time to tell make what -W flags to put on the compiles.
    # Needed at compile time  for smsh/sm_sizeof.cpp
    #
    # -------------------------------------------------------------------------

    AH_TEMPLATE(GCC_BROKEN_WARNINGS,	[/*is gcc 2.96*/])
    AH_VERBATIM([GCC_BROKEN_WARNINGS],[/*if is gcc2.96*/
#if defined(__GNUC__) && __GNUC__==2
# if defined(__GNUC_MINOR__) && __GNUC_MINOR__==96
# define GCC_BROKEN_WARNINGS
#endif
#endif
    ])
## NOTE: AH_VERBATIM gets processed even despite the if/else/endif
## and the 2nd and subsequent lines must start in the first column or
## the sparc/CC assember/cpp chokes on these lines.S

    AC_MSG_CHECKING([whether the compiler is gcc 2.96])
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
    [[
    #if defined(__GNUC__) && (__GNUC__==2) && defined(__GNUC_MINOR__) && (__GNUC_MINOR__==96)
    #else
    #error not broken warnings
    #endif
    ]])], 
    [AM_CONDITIONAL([GCC_BROKEN_WARNINGS], [test -n "1"]) AC_MSG_RESULT([yes])],
    [AM_CONDITIONAL([GCC_BROKEN_WARNINGS], [test -z "1"]) AC_MSG_RESULT([no])]
    )

    # -------------------------------------------------------------------------
    #
    # NOPEDANTIC: determine if this is a compiler with known problems in
    # its include files that prevent -pedantic from being used.
    # Needed at config time to tell make whether to use -pedantic
    # We find this to be a problem with 3.0 through 3.2
    #
    # We find this to be a problem with different Linices. Just rip it out....
    # -------------------------------------------------------------------------

    # AH_TEMPLATE(NOPEDANTIC,	[does compiler have header-file pedantic-errors])
    # AC_MSG_CHECKING([if compiler has header-file errors precluding -pedantic])
    # AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
    # [[
    # if defined(__GNUC__) && (__GNUC__==3) && defined(__GNUC_MINOR__) && (__GNUC_MINOR__<=2)
    # #else
    # #   error 
    # #endif
    # ]])], 
    # [AM_CONDITIONAL([NOPEDANTIC], [test -n "1"]) AC_MSG_RESULT([yes])],
    # [AM_CONDITIONAL([NOPEDANTIC], [test -z "1"]) AC_MSG_RESULT([no])]
    # )
    AM_CONDITIONAL([NOPEDANTIC], [test -n "1"])


    # -------------------------------------------------------------------------
    #
    # GCC_VER_34_WARNINGS
    # Needed at config time to tell make what -W flags to put on the compiles.
    #
    # -------------------------------------------------------------------------

    AH_TEMPLATE(GCC_VER_34_WARNINGS,	[hack for gcc3.4 and later])
    AC_MSG_CHECKING([whether the compiler is gcc 3.4 or later ])
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
    [[
    #if defined(__GNUC__) && (__GNUC__==3) && defined(__GNUC_MINOR__) && (__GNUC_MINOR__>=4)
    #else
    #   if defined(__GNUC__) && (__GNUC__>3) 
    #   else
    #   error version 3.4
    #   endif
    #endif
    ]])], 
    [AM_CONDITIONAL([GCC_VER_34_WARNINGS], [test -n "1"]) AC_MSG_RESULT([yes])],
    [AM_CONDITIONAL([GCC_VER_34_WARNINGS], [test -z "1"]) AC_MSG_RESULT([no])]
    )


    # -------------------------------------------------------------------------
    #
    # GCC_VER_3_WARNINGS
    # Needed at config time to tell make what -W flags to put on the compiles.
    # Needed for smsh/sm_sizeof.cpp
    #
    # -------------------------------------------------------------------------

    AH_TEMPLATE(GCC_VER_3_WARNINGS,	[hack for gcc3.x ])
    AC_MSG_CHECKING([whether the compiler is gcc 3.x ])
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
    [[#if defined(__GNUC__) && (__GNUC__==3) 
    #else
    #error not version 3
    #endif
    ]])], 
    [ AM_CONDITIONAL([GCC_VER_3_WARNINGS],[test -n "1"]) AC_MSG_RESULT([yes])],
    [ AM_CONDITIONAL([GCC_VER_3_WARNINGS],[test -z "1"]) AC_MSG_RESULT([no])]
    )
    AH_TEMPLATE(GCC_VER_3_WARNINGS,	[/*is gcc 3.x, x < 4*/])
    AH_VERBATIM([GCC_VER_3_WARNINGS],[/*if is gcc3.x, x < 4*/
#if defined(__GNUC__) && (__GNUC__==3) 
# define GCC_VER_3_WARNINGS
#endif
    ])
## NOTE: AH_VERBATIM gets processed even despite the if/else/endif
## and the 2nd and subsequent lines must start in the first column or
## the sparc/CC assember/cpp chokes on these lines.S


    # -------------------------------------------------------------------------
    #
    # No macros defined.
    # Needed at config time to tell if the chosen compiler version is known
    #   to fail at build time.
    # NOTE: this used to fail for 32-bit 4.1.2, but with RHEL 4.1.2 is installed
    # and since the code has changed so much, I've changed the config to allow 
    # 4.1.2 to be used again; it seems to work so far. So I don't know if
    # 4.1.[01] are a problem anymore. Maybe they aren't.
    #
    # -------------------------------------------------------------------------

    AC_MSG_CHECKING([whether the gcc version is known broken: 4.1.x, x < 2 or 2.95])
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
    [[
    #if defined(__GNUC__) && (__GNUC__==4) && defined(__GNUC_MINOR__) && (__GNUC_MINOR__==1) && defined(__GNUC_PATCHLEVEL__) && (__GNUC_PATCHLEVEL__<2)
    #else

    #if defined(__GNUC__) && (__GNUC__==2) && defined(__GNUC_MINOR__) && (__GNUC_MINOR__==95) 
    #else
    #error not version 2.95 or 4.1.2
    #endif

    #endif
    ]])], 
    [ 
      AC_MSG_RESULT([yes: build will fail])
      AC_MSG_ERROR([
    *****************************************************************************
       You are using broken version of gcc. Please try another release.
    *****************************************************************************
      ])
      ],
    [ AC_MSG_RESULT([no])
    ]
    )

    AC_MSG_CHECKING([whether the gcc version has known issues: 4.0.1])
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
    [[
    #if defined(__GNUC__) && (__GNUC__==4) && defined(__GNUC_MINOR__) && (__GNUC_MINOR__==0) && defined(__GNUC_PATCHLEVEL__) && (__GNUC_PATCHLEVEL__==1)
    #else
    #error not version 4.0.1
    #endif
    ]])], 
    [ 
      AC_MSG_RESULT([yes: build will have warnings])
      AC_MSG_WARN([
    *****************************************************************************
       You are using version of gcc that generates questionale assembly. 
       Please try another release, such as 4.0.4.
    *****************************************************************************
      ])
      ],
    [ AC_MSG_RESULT([no])]
    )
else

    AC_MSG_CHECKING([if desired compiler is SunStudio])
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
    [[
    #if defined(__SUNPRO_CC) 
    #else
    #error "not SunStudio CC"

    #endif
    ]])], 
    [AM_CONDITIONAL([COMPILER_IS_SUNCC], [test -n "1"]) AC_MSG_RESULT([yes])],
    [AM_CONDITIONAL([COMPILER_IS_SUNCC], [test -z "1"]) AC_MSG_RESULT([no])]
)

    AC_MSG_CHECKING([if desired compiler is unknown])
    AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
    [[
    #if !defined(__SUNPRO_CC)  && !defined(__GNUC__)
    #error Unknown compiler
    #else
    #endif
    ]])], 
    [AM_CONDITIONAL([COMPILER_IS_UNKNOWN], [test -n "1"]) AC_MSG_RESULT([no])],
    [AM_CONDITIONAL([COMPILER_IS_UNKNOWN], [test -z "1"]) AC_MSG_RESULT([COMPILER IS UNKNOW])]
    )

# With certainty, not GCC
    AM_CONDITIONAL([COMPILER_IS_GCC], [test -z "1"])
    AM_CONDITIONAL([COMPILER_IS_ICC], [test -z "1"])
    AM_CONDITIONAL([GCC_BROKEN_WARNINGS], [test -z "1"])
    AM_CONDITIONAL([GCC_VER_34_WARNINGS], [test -z "1"])
    AM_CONDITIONAL([GCC_VER_3_WARNINGS],[test -z "1"])
    AM_CONDITIONAL([NOPEDANTIC], [test -n "1"])

fi
AC_LANG_POP(C++)

## AC_PROG_INSTALL 

AC_PROG_RANLIB 

AC_PATH_PROG([PERL], [perl], [],[]) 

## set DTRACE to the path of DTRACE or leave it undefined
AC_PATH_TOOL([DTRACE], [dtrace], [],[]) 

AC_CHECK_TOOL(RMM, rm, /bin/rm)

## HAVE_DIRENT_H : src/sthread/os_interface.h
AC_HEADER_DIRENT 

## HAVE_FCNTL_H : src/sthread/os_fcntl.h
## if not HAVE_FCNTL_H, it's assumed you have <sys/fcntl.h>
AC_CHECK_HEADERS([fcntl.h]) 

## used in os_memory.h and tests/ :
## for definitions of memset and companions
## if not found in memory.h, lookin string.h if we have it
AC_CHECK_HEADERS([memory.h]) 
AC_CHECK_HEADERS([string.h]) 

## used in lid.cpp (logical-id manager)  for volume ids
AC_CHECK_HEADERS([netdb.h]) 

## used in sthread lib, for ntohs, htohl, etc
AC_CHECK_HEADERS([netinet/in.h]) 

## AC_CHECK_HEADERS([stdlib.h]) 
## used in solaris_stats only
AC_CHECK_HEADERS([sys/ioctl.h]) 

## used in fc/w_minmax.h
AC_CHECK_HEADERS([sys/param.h]) 

## used in sthread/sfile.cpp, hpp
AC_CHECK_HEADERS([sys/socket.h]) 

## used in error log and w_form
AC_FUNC_VPRINTF 

AC_CHECK_TYPES([ptrdiff_t, uchar_t, ushort_t, ulong_t, char_t, short_t, long_t]) 
AC_TYPE_SIZE_T 
AC_TYPE_UID_T 
AC_TYPE_PID_T 
AC_TYPE_MODE_T 
AC_TYPE_OFF_T 
AC_CHECK_TYPES([ptrdiff_t]) 

##
## Check capabilities of MMAP
##  Don't bother to check for mmap, because we have no alternatives to it
##
AC_CHECK_DECLS([MAP_ANONYMOUS, MAP_ANON, MAP_ALIGN, MAP_FIXED, MAP_NORESERVE],[],[],[#include <sys/mman.h>]) 
AC_CHECK_FUNCS([memcntl]) 
AC_CHECK_FUNCS([getpagesizes]) 

## AC_CHECK_DECLS([sys_siglist]) 
AC_CHECK_MEMBERS([struct stat.st_blksize]) 
## can't build any of this if wedon't have this type.

## AM_CONDITIONAL HAVE_ATOMIC_OPS is removed in 
## favor of unconditionally making the library, which depends
## on compile-time HAVE_ATOMIC_H in the shore-config.h file 
## AH_TEMPLATE(HAVE_ATOMIC_OPS,	[operating system provides atomic_ops])
AC_CHECK_HEADERS([atomic.h]) 

# endian checks used in unit tests
AC_C_BIGENDIAN


## Architectures: These are defined even though SHORE-MT is not supported
## on most.  As it is ported to other architectures, we may use these.
## The AH_TEMPLATES are needed prior to the architecture-detecting macros 
## uses below.

AH_TEMPLATE(I860,	[architecture])
AH_TEMPLATE(I386,	[architecture])
AH_TEMPLATE(i386,	[architecture - not always defined by compiler])
AH_TEMPLATE(IA64,	[architecture])
AH_TEMPLATE(amd64,	[architecture])
AH_TEMPLATE(x86_64,	[architecture])
AH_TEMPLATE(Snake, 	[architecture])
AH_TEMPLATE(Sparc, 	[architecture])

AH_TEMPLATE(HPUX8, 	[operating system])
AH_TEMPLATE(Linux,	[operating system])
AH_TEMPLATE(SOLARIS2,	[operating system])
AH_TEMPLATE(SUNOS41,	[operating system])

AH_TEMPLATE(IS_HPUX8,	[operating system])
AH_TEMPLATE(IS_LINUX,	[operating system])
AH_TEMPLATE(IS_SOLARIS2,[operating system])
AH_TEMPLATE(IS_SUNOS41,	[operating system])

AH_TEMPLATE(IS_I386_LINUX,	[combo])
AH_TEMPLATE(IS_I386_SOLARIS2,	[combo])
AH_TEMPLATE(IS_IA64_LINUX,	[combo])
AH_TEMPLATE(IS_SNAKE_HPUX8,	[combo])
AH_TEMPLATE(IS_SPARC_SOLARIS2,	[combo])
AH_TEMPLATE(IS_SPARC_SUNOS41,	[combo])
AH_TEMPLATE(IS_X86_64,          [combo])
AH_TEMPLATE(IS_AMD64,          [combo])


##  SHORE-MT debugging config options:

AH_TEMPLATE(DEBUG,		[old debug code included])
AH_TEMPLATE(NDEBUG,		[no debug code included])
AH_TEMPLATE(W_TRACE,		[tracing debugging code included])
AH_TEMPLATE(W_DEBUG_RC,		[rc-is-checked code included])

##  SHORE-MT DIAS projects:
AH_TEMPLATE(SM_DORA,            [DORA-related code included])

## Check for atomic ops
AC_CHECK_FUNCS([membar_enter]) 
AC_CHECK_FUNCS([posix_spawn]) 

## check for suitable function for stime_t
AC_CHECK_FUNCS([clock_gettime]) 
AC_CHECK_FUNCS([gettimeofday]) 

## Checks for library functions.
## if any of these don't exist, we have a real problem.
## For now, don't bother checking, because we don't have any
## alternatives at the moment, and we're not trying to build this
## on windows..

## AC_SEARCH_LIBS and AC_CHECK_LIB do not define HAVE_xxx
## so we must also use CHECK_FUNCS for all 3
## and AC_SEARCH_LIBS to add the proper libraries for linking.
AC_SEARCH_LIBS([getaddrinfo],[socket])
AC_SEARCH_LIBS([gethostname],[nsl])
AC_SEARCH_LIBS([gethostbyname],[nsl])
AC_CHECK_FUNCS([getaddrinfo])
AC_CHECK_FUNCS([gethostname])
AC_CHECK_FUNCS([gethostbyname])

## These are needed but have no alternative if not present
## from <cstring>:
## AC_CHECK_FUNCS([memchr]) 
## AC_CHECK_FUNCS([memmove]) 
## AC_CHECK_FUNCS([memset]) 
## AC_CHECK_FUNCS([strchr]) 
## AC_CHECK_FUNCS([strrchr]) 
## AC_CHECK_FUNCS([strstr]) 
AC_CHECK_FUNCS([strerror]) 

## for smsh shell 
AC_SEARCH_LIBS([strcasecmp],[c]) 
AC_SEARCH_LIBS([strdup],[c]) 

## from <cstdlib>
## AC_CHECK_FUNCS([strtol]) 
## AC_CHECK_FUNCS([strtoul]) 
## AC_CHECK_FUNCS([strtoull]) 


##  Check for size of pthread_t
## solaris says int, linux says unsigned long
## this affects how we define tatas_lock, among other things
AC_CHECK_SIZEOF(pthread_t,[],[])


## define HAVE_LIBrt and prepend -lrt to LIBS:
AC_CHECK_LIB([rt], [clock_gettime])

## look for faster multi-threaded malloc library
AC_CHECK_LIB([mtmalloc], [malloc],,,[tmalloc])

## check for the pthread func to get the stack size and addr
AH_TEMPLATE(HAVE_PTHREAD_ATTR_GETSTACK, [have pthread_attr_getstack?] )

AC_CHECK_LIB([pthread], [pthread_attr_getstack],
[AC_DEFINE(HAVE_PTHREAD_ATTR_GETSTACK,1)],
[AC_DEFINE(HAVE_PTHREAD_ATTR_GETSTACK,0)]
)
## check for the pthread func to get the stack size
AH_TEMPLATE(HAVE_PTHREAD_ATTR_GETSTACKSIZE, [have pthread_attr_getstacksize?] )

AC_CHECK_LIB([pthread], [pthread_attr_getstacksize],
[AC_DEFINE(HAVE_PTHREAD_ATTR_GETSTACKSIZE,1)],
[AC_DEFINE(HAVE_PTHREAD_ATTR_GETSTACKSIZE,0)]
)

## check for the pthread barriers option for smsh
AH_TEMPLATE(HAVE_PTHREAD_BARRIERS, [have pthreads barriers?] )
AC_CHECK_LIB([pthread], [pthread_barrier_wait], 
[AC_DEFINE(HAVE_PTHREAD_BARRIERS,1)],
[AC_DEFINE(HAVE_PTHREAD_BARRIERS,0)]
)


## ---------------------------------------------------------------
## 
## -------------- Architecture
##
## NOTE: all these IS_xxx
## are not used in the sources and can be removed. For now, they
## are left in for developers' convenience, in event we decide
## to port to some other platform and need them.
## TODO: check that these still are not used.
##
## ---------------------------------------------------------------

AC_LANG_PUSH(C++)

AC_MSG_CHECKING([architecture])
AC_SUBST([uname_machine], [`uname -m`])

AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
[[#if defined(__SVR4) && defined(sparc) && defined(sun)
#else
#error not this architecture
#endif
]])], 
[AC_DEFINE(Sparc,,[])
AC_DEFINE(IS_SPARC_SOLARIS2,1,[is sparc solaris])
AC_DEFINE(IS_SOLARIS2,1,[is solaris])
AC_MSG_RESULT([Sparc Solaris])
],
[])

AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
[[#if defined(hpux) && defined(hp9000s800) 
#else
#error not this architecture
#endif
]])], 
[AC_DEFINE(Snake,,[])
AC_DEFINE(HPUX8,,[])
AC_DEFINE(IS_SNAKE_HPUX8,1,[hp unix])
AC_DEFINE(IS_HPUX8,1,[hp unix])
AC_MSG_RESULT([hpux  hp])
],
[])
AH_VERBATIM([hpux],[/*turn off hpux and hp9000s800*/
#if defined(hpux) && defined(hp9000s800)
#undef hpux
#undef hp9000s800
#endif
])

AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
[[#if defined(linux) 
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#else
#error not this architecture
#endif
#else
#error not this architecture
#endif
]])], 
[AC_DEFINE(x86_64,,[])
AC_DEFINE(amd64)
AC_DEFINE(Linux,,[])
AC_DEFINE(IS_X86_64_LINUX,1,[x86_64 linux])
AC_DEFINE(IS_AMD64_LINUX,1,[amd64 linux])
AC_DEFINE(IS_LINUX,1,[linux])
AC_MSG_RESULT([linux amd64,x86_64])
],
[])
AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
[[#if defined(linux) && defined(i386)
#else
#error not this architecture
#endif
]])], 
[AC_DEFINE(I386,,[])
AC_DEFINE(i386)
AC_DEFINE(Linux,,[])
AC_DEFINE(IS_I386_LINUX,1,[i386 linux])
AC_DEFINE(IS_LINUX,1,[linux])
AC_MSG_RESULT([linux i386])
],
[])

AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
[[#if defined(linux) && defined(__ia64)
#else
#error not this architecture
#endif
]])], 
[AC_DEFINE(IA64,,[])
AC_DEFINE(Linux,,[])
AC_DEFINE(IS_IA64_LINUX,1,[ia64 linux])
AC_DEFINE(IS_LINUX,1,[linux])
AC_MSG_RESULT([linux ia64])
],
[])

AC_LANG_POP(C++)

AC_LANG_PUSH(C++)
# -------------------------------------------------------------------------
#
# USE_IOS_COMPAT
# This simply causes configure/make to add the compat/ directory to
# the include list for compiles.
# -------------------------------------------------------------------------
# NOTE: must use AC_PREPROC_IFELSE here because at least one
# compiler version (4.1.2) produces bad assembly, which causes this
# test to fail. Cannot use 4.1.2
# 
# NOTE: compat library no longer used (6.0 release). Code left in place
# for historical reasons.
# -------------------------------------------------------------------------
AC_MSG_CHECKING([whether the ios headers exist without .h suffix])
AC_PREPROC_IFELSE([AC_LANG_PROGRAM(
[[#include <sstream>
]])], 
[AM_CONDITIONAL([USE_IOS_COMPAT],[test -z "1"]) AC_MSG_RESULT([yes])],
[AM_CONDITIONAL([USE_IOS_COMPAT],[test -n "1"]) AC_MSG_RESULT([no])]
)
# -------------------------------------------------------------------------
#
# W_USE_COMPAT_STRSTREAM
# Unrelated to USE_IOS_COMPAT
#
# unconditional #define W_USE_COMPAT_STRSTREAM to get in-mem stream bufs
#
# At one time we made this optional, but with the newer compilers, it
# is mandatory
#
# -------------------------------------------------------------------------
AH_VERBATIM([W_USE_COMPAT_STRSTREAM],[/*for in-memory str streams*/
#define W_USE_COMPAT_STRSTREAM
])


AC_LANG_POP(C++)



# add bogus "features" simply for the purpose of getting nicer --help 
AC_ARG_ENABLE([bogus],
[
SHORE-specific Features:], [], [])


# -------------------------------------------------------------------------
#
# SM_PAGESIZE :: --with-page-size=xxx
#  
# -------------------------------------------------------------------------

AC_MSG_CHECKING([for sm page size])
AC_ARG_WITH([page-size],
[AS_HELP_STRING([--with-page-size],[1024, 2048, 4196, 8192(default), 16384, or 32768])],
[],
[with_page_size=8192])
AC_DEFINE_UNQUOTED(SM_PAGESIZE, $with_page_size, [configured page size])
AC_MSG_RESULT([$with_page_size])
# -------------------------------------------------------------------------
#
# HAVE_HUGETLBFS
#  
#  Generate HAVE_HUGETLBFS if so configured
#
# NOTE ************************************  REMOVED FOR NOW - problematic.
# If/when reinstalled, then add back in doxygen info in src/mainpage.h
#
# -------------------------------------------------------------------------
# AC_ARG_WITH([hugetlbfs],
# [AS_HELP_STRING([--with-hugetlbfs],[Use the hugetlbfs for the buffer pool])],
# [],
# [with_hugetlbfs=no])
# AS_IF([test "x$with_hugetlbfs" = xyes],
# [AC_DEFINE_UNQUOTED(HAVE_HUGETLBFS, 1, [use hugetlbfs for buffer pool])]
# )

## invoke gethugepagesize.sh to establish the page size
## we do this regardless of the --with-hugetlbfs option; it's
## only used if we chose the option
hugepagesize=`./gethugepagesize.sh`
AC_MSG_CHECKING([for Hugepagesize])
AC_DEFINE_UNQUOTED(HUGEPAGESIZE, $hugepagesize, [huge page size])
AC_MSG_RESULT([$hugepagesize])

# -------------------------------------------------------------------------
#
# --without-mmap
#  
#  Don't bother to mmap.  --without-mmap overrides hugetlbfs use.
#  It means just allocate aligned memory and don't bother using mmap for
#  the buffer pool in any attempt to affect the #tlb entries. 
#  We will use posix_memalign if it's here; else memalign if it's here,
#  else valloc if it's here.
#
# -------------------------------------------------------------------------
# 
#
#AC_ARG_WITH([mmap],
#[AS_HELP_STRING([--without-mmap],[Do not use mmap for the buffer pool. Trumps hugetlbfs option])],
#[],
#[with_mmap=yes])
#AS_IF([test "x$with_mmap" = xno],
#[AC_DEFINE_UNQUOTED(WITHOUT_MMAP, 1, [do not mmap the buffer pool])]
#)
AC_CHECK_FUNCS([posix_memalign])
AC_CHECK_FUNCS([memalign])
AC_CHECK_FUNCS([valloc])

# -------------------------------------------------------------------------
#
# handle --enable-pthread-mutex  --disable-pthread-mutex
#
# Determines what compile-time options to use for USE_PTHREAD_MUTEX
# (src/sthread/sthread.h), to use pthread mutexen implementation of
# queue_based_lock_t
#
#
# -------------------------------------------------------------------------
AH_TEMPLATE(USE_PTHREAD_MUTEX, [enable-pthread-mutex ?] )
AC_MSG_CHECKING(whether to enable pthreads-based mutex implementation)
AC_ARG_ENABLE([pthread-mutex],
[  --enable-pthread-mutex default:yes	Compile to use pthreads-based mutexes for certain critical sections],
 [case "${enableval}" in
 yes) usepthreadmutex=ON ;;
 no)  usepthreadmutex=OFF ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-pthread-mutex]) ;;
esac]
 , 
[usepthreadmutex=ON]
)

if test "x$usepthreadmutex" = xON
then 
     AC_MSG_RESULT(yes)
     AC_DEFINE(USE_PTHREAD_MUTEX, 1)
else
     AC_MSG_RESULT(no)
     AC_DEFINE(USE_PTHREAD_MUTEX, 0)
fi

# -------------------------------------------------------------------------
#
# handle --enable-lp64  --disable-lp64
#
# Determines what compile-time options to use for target machine (-m64 (gcc)
#  or -xtarget=native64 (Sun CC)
#
#
# -------------------------------------------------------------------------
AC_ARG_ENABLE([lp64],
[  --enable-lp64		default:yes	Compile to use LP 64 data model],
 [case "${enableval}" in
 yes) lp64=ON ;;
 no)  lp64=OFF ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-lp64]) ;;
esac]
 , 
[lp64=ON]
)

AM_CONDITIONAL([ENABLE_ARCH_LP64], [test x$lp64 = xON])


# This is unnecessary for LP64  **however** we are leaving it in 
# in anticipation of porting back to LP32
# 
AH_VERBATIM([W_LARGEFILE],[#if (defined(_LARGEFILE_SOURCE) && defined(_FILE_OFFSET_BITS)) || defined(ARCH_LP64)
#define LARGEFILE_AWARE
#endif
])

# -------------------------------------------------------------------------
#
# Posix monotonic clock funcs : clock_gettime
#  on linux, need this to implement gethrtime() lib func
# -------------------------------------------------------------------------
## AC_MSG_CHECKING([for posix monotonic clock library functions])
AC_CHECK_HEADERS([time.h]) 
AC_CHECK_FUNCS([gethrtime])
AC_CHECK_FUNCS([getopt])
AC_CHECK_FUNCS([uname])

# -------------------------------------------------------------------------
#
# handle --enable-checkrc --disable-checkrc
#   --enable-checkrc causes #define W_DEBUG_RC 1
#   --disable-checkrc causes #define W_DEBUG_RC 0
#
# #if W_DEBUG_RC==1 the enforcement of w_rc_t::verify() is done.
# #if W_DEBUG_RC==0 the enforcement of w_rc_t::verify() is skipped.
# all this is in fc/w_rc.h
#
# -------------------------------------------------------------------------
# p. 198
AC_ARG_ENABLE([checkrc],
[  --enable-checkrc	default:no	Generate (expensive) code to verify return-code checking],
 [case "${enableval}" in
 yes) checkrc=1 ;;
 no)  checkrc=0 ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-checkrc]) ;;
esac]
 , 
[checkrc=0])
AC_DEFINE_UNQUOTED(W_DEBUG_RC, $checkrc,[track return-code checking])

# -------------------------------------------------------------------------
#
# handle --enable-trace --disable-trace
# --enable-trace is equiv to  #define TRACECODE 1,
# which, in turn (in shore-config.h), causes #define W_TRACE,
# which, in turn fc/w_base.h causes #define W_IFTRACE to be non-vacuous
#
# --disable-trace is equiv to 
# #undef W_TRACE and
# #define W_IFTRACE /**/
# -------------------------------------------------------------------------
# p. 198
AC_ARG_ENABLE([trace],
[  --enable-trace	default:no	Include tracing code],
 [case "${enableval}" in
 yes) trace=1 ;;
 no)  trace=0 ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-trace]) ;;
esac]
 , 
[trace=0])
AM_CONDITIONAL([TRACE], [test x$trace = x1])

AC_DEFINE_UNQUOTED(TRACECODE, $trace,[include tracing code?])
AH_VERBATIM([W_TRACE],[#if TRACECODE==1
#define W_TRACE
#endif
])

# -------------------------------------------------------------------------
#
# handle --with-debug-level1 --without-debug-level1
# handle --with-debug-level2 --without-debug-level2
# handle --with-debug-level3 --without-debug-level3
# Needed at config time to tell make what flags to put on the compiles
#
# -------------------------------------------------------------------------
# 
AC_ARG_WITH([debug-level1],
[AS_HELP_STRING([--with-debug-level1],[Include level 1 debug code, optimize])],
[],
[with_debug_level1=no])

## Make needs to give the right flags
AM_CONDITIONAL([DEBUGLEVEL1], [test x$with_debug_level1 = xyes])

AS_IF([test "x$with_debug_level1" = xyes],
[AC_DEFINE_UNQUOTED(W_DEBUG_LEVEL, 1, [use debug level 1])
AC_MSG_RESULT([Debug level 1])
]
)

AC_ARG_WITH([debug-level2],
[AS_HELP_STRING([--with-debug-level2],[Include level 2 debug code, no opt])],
[],
[with_debug_level2=no])

## Make needs to give the right flags
AM_CONDITIONAL([DEBUGLEVEL2], [test x$with_debug_level2 = xyes])
AS_IF([test "x$with_debug_level2" = xyes],
[AC_DEFINE_UNQUOTED(W_DEBUG_LEVEL, 2, [use debug level 2])
AC_MSG_RESULT([Debug level 2])
]
)


AC_ARG_WITH([debug-level3],
[AS_HELP_STRING([--with-debug-level3],[Include level 3 debug code, no opt])],
[],
[with_debug_level3=no])

# Makefile needs to know level
AM_CONDITIONAL([DEBUGLEVEL3], [test x$with_debug_level3 = xyes])

AS_IF([test "x$with_debug_level3" = xyes],
[AC_DEFINE_UNQUOTED(W_DEBUG_LEVEL, 3, [use debug level 3])
AC_MSG_RESULT([Debug level 3])
]
)

with_debug_level0=no

AS_IF(
[test "x$with_debug_level1" = xno -a "x$with_debug_level2" = xno -a "x$with_debug_level3" = xno ],
[AC_DEFINE_UNQUOTED(W_DEBUG_LEVEL, 0, [Use debug level 0, optimized])
AC_MSG_RESULT([Debug level 0 (optimized)])
with_debug_level0=yes
]
)

AM_CONDITIONAL([DEBUGLEVEL0], [test x$with_debug_level0 = xyes])

# -------------------------------------------------------------------------
#
# NOTE: THIS MUST COME AFTER THE handling of debuglevel* 
#
# handle --enable-dbgsymbols --disable-dbgsymbols
# Needed at config time to tell make what flags to put on the compiles,
#
# -------------------------------------------------------------------------
AC_ARG_ENABLE([dbgsymbols],
[  --enable-dbgsymbols	default:no	Turn on debugger symbols],
[case "${enableval}" in
 yes) dbgsymbols=true ;;
 no)  dbgsymbols=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-dbgsymbols]) ;;
esac], [dbgsymbols=false])

AS_IF(
[test "x$with_debug_level1" = xyes -o "x$with_debug_level2" = xyes -o "x$with_debug_level3" = xyes ],
[dbgsymbols=true]
)
AC_MSG_RESULT([Debugger symbols $dbgsymbols])
AM_CONDITIONAL([DEBUGGERSYMBOLS], [test x$dbgsymbols = xtrue -o x$with_debug_level1 = xyes -o x$with_debug_level2 = xyes -o x$with_debug_level3 = xyes])


# -------------------------------------------------------------------------
#
# handle --enable-explicit --disable-explicit
# Needed at config time to tell make what flags to put on the compiles.
# DISABLED: this needs work!
#
# -------------------------------------------------------------------------
# AC_ARG_ENABLE([explicit],
# [  --enable-explicit	default:no	Compile with explicit templates],
# [case "${enableval}" in
#  yes) explicit=true ;;
#  no)  explicit=false ;;
#  *) AC_MSG_ERROR([bad value ${enableval} for --enable-explicit]) ;;
# esac], [explicit=false])
AM_CONDITIONAL([EXPLICIT_TEMPLATE], [test x$explicit = xtrue])

# -------------------------------------------------------------------------
#
# handle --enable-purify, --enable-quantify, --enable-purecov, --enable-valgrind
# Needed at config time to tell make what include directories to add
# and what libs to add.
#
# -------------------------------------------------------------------------
AC_ARG_ENABLE([valgrind],
[  --enable-valgrind	default:no	Enable running valgrind run-time behavior],
[case "${enableval}" in
 yes) valgrind=true ;;
 no)  valgrind=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-valgrind]) ;;
esac], [valgrind=false])
AS_IF([test "x$valgrind" = xtrue],
[AC_CHECK_HEADERS([valgrind/valgrind.h])
AC_CHECK_HEADERS([valgrind.h])
]
)

AC_ARG_ENABLE([purify],
[  --enable-purify	default:no	Enable build of <prog>.pure],
[case "${enableval}" in
 yes) purify=true ;;
 no)  purify=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-purify]) ;;
esac], [purify=false])
AM_CONDITIONAL([PURIFY], [test x$purify = xtrue])

AC_ARG_ENABLE([quantify],
[  --enable-quantify	default:no	Enable build of <prog>.quant],
[case "${enableval}" in
 yes) quantify=true ;;
 no)  quantify=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-quantify]) ;;
esac], [quantify=false])
AM_CONDITIONAL([QUANTIFY], [test x$quantify = xtrue])

AC_ARG_ENABLE([purecov],
[  --enable-purecov	default:no	Enable build of <prog>.purecov],
[case "${enableval}" in
 yes) purecov=true ;;
 no)  purecov=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-purecov]) ;;
esac], [purecov=false])
AM_CONDITIONAL([PURECOV], [test x$purecov = xtrue])


# -------------------------------------------------------------------------
#
# Shore-MT new features
#
# There are six supported additional options:
#
# (1) --enable-dora       : defines SM_DORA
# Enables the code paths for Data-oriented Transaction Execution (DORA)
#
# -------------------------------------------------------------------------
AC_MSG_CHECKING(whether to enable DORA-support)
AC_ARG_ENABLE([dora], 
[  --enable-dora         default:no      Enable DORA],
[case "${enableval}" in
  yes) dora=true ;;
  no)  dora=false ;;
  *) dora=false ;;
esac],[dora=false])
AM_CONDITIONAL([USE_DORA], [test x$dora = xtrue])

if test "$dora" = true
then 
     AC_MSG_RESULT(yes)
     AC_DEFINE(SM_DORA)
else
     AC_MSG_RESULT(no)
fi
# -------------------------------------------------------------------------


AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([config/Makefile])
AC_CONFIG_FILES([tools/Makefile])

AC_CONFIG_FILES([src/Makefile])

AC_CONFIG_FILES([src/atomic_ops/Makefile])

AC_CONFIG_FILES([src/fc/Makefile])
AC_CONFIG_FILES([src/fc/tests/Makefile])

AC_CONFIG_FILES([src/sthread/Makefile])
AC_CONFIG_FILES([src/sthread/tests/Makefile])

AC_CONFIG_FILES([src/common/Makefile])
AC_CONFIG_FILES([src/common/tests/Makefile])

AC_CONFIG_FILES([src/sm/Makefile])
AC_CONFIG_FILES([src/sm/tests/Makefile])
AC_CONFIG_FILES([src/sm/smsh/Makefile])
AC_CONFIG_FILES([src/sm/smsh/scripts/Makefile])
AC_CONFIG_FILES([src/sm/smsh/lib/Makefile])

AC_CONFIG_FILES([src/atomic_ops/asm_linkage/Makefile])
AC_CONFIG_FILES([src/atomic_ops/asm_linkage/usr/Makefile])
AC_CONFIG_FILES([src/atomic_ops/asm_linkage/usr/src/Makefile])
AC_CONFIG_FILES([src/atomic_ops/asm_linkage/usr/src/uts/Makefile])
AC_CONFIG_FILES([src/atomic_ops/asm_linkage/usr/src/uts/intel/Makefile])
AC_CONFIG_FILES([src/atomic_ops/asm_linkage/usr/src/uts/intel/ia32/Makefile])
AC_CONFIG_FILES([src/atomic_ops/asm_linkage/usr/src/uts/intel/ia32/sys/Makefile])
AC_CONFIG_FILES([src/atomic_ops/asm_linkage/usr/src/uts/intel/sys/Makefile])
AC_CONFIG_FILES([src/atomic_ops/asm_linkage/usr/src/uts/sparc/Makefile])
AC_CONFIG_FILES([src/atomic_ops/asm_linkage/usr/src/uts/sparc/sys/Makefile])
AC_CONFIG_FILES([src/atomic_ops/atomic_ops/Makefile])
AC_CONFIG_FILES([src/atomic_ops/atomic_ops/amd64/Makefile])
AC_CONFIG_FILES([src/atomic_ops/atomic_ops/i386/Makefile])
AC_CONFIG_FILES([src/atomic_ops/atomic_ops/sparc/Makefile])
AC_CONFIG_FILES([src/atomic_ops/atomic_ops/sparcv9/Makefile])

AC_OUTPUT
