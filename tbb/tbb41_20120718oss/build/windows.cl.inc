# Copyright 2005-2012 Intel Corporation.  All Rights Reserved.
#
# This file is part of Threading Building Blocks.
#
# Threading Building Blocks is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# version 2 as published by the Free Software Foundation.
#
# Threading Building Blocks is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Threading Building Blocks; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# As a special exception, you may use this file as part of a free software
# library without restriction.  Specifically, if other files instantiate
# templates or use macros or inline functions from this file, or you compile
# this file and link it with other files to produce an executable, this
# file does not by itself cause the resulting executable to be covered by
# the GNU General Public License.  This exception does not however
# invalidate any other reasons why the executable file might be covered by
# the GNU General Public License.

#------------------------------------------------------------------------------
# Define compiler-specific variables.
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Setting compiler flags.
#------------------------------------------------------------------------------
CPLUS = cl /nologo
LINK_FLAGS = /link /nologo
LIB_LINK_FLAGS=/link /nologo /DLL /MAP /DEBUG /fixed:no /INCREMENTAL:NO /DYNAMICBASE /NXCOMPAT

ifeq ($(arch), ia32)
	LIB_LINK_FLAGS += /SAFESEH
endif

ifeq ($(runtime), vc_mt)
	MS_CRT_KEY = /MT$(if $(findstring debug,$(cfg)),d)
else
	MS_CRT_KEY = /MD$(if $(findstring debug,$(cfg)),d)
endif
EH_FLAGS = /EHsc /GR
              
ifeq ($(cfg), release)
        CPLUS_FLAGS = $(MS_CRT_KEY) /O2 /Zi $(EH_FLAGS) /Zc:forScope /Zc:wchar_t /D__TBB_LIB_NAME=$(TBB.LIB)
        ASM_FLAGS =
endif
ifeq ($(cfg), debug)
        CPLUS_FLAGS = $(MS_CRT_KEY) /Od /Ob0 /Zi $(EH_FLAGS) /Zc:forScope /Zc:wchar_t /DTBB_USE_DEBUG /D__TBB_LIB_NAME=$(TBB.LIB)
        ASM_FLAGS = /DUSE_FRAME_POINTER
endif
CPLUS_FLAGS += /GS

COMPILE_ONLY = /c
PREPROC_ONLY = /TP /EP
INCLUDE_KEY = /I
DEFINE_KEY = /D
OUTPUT_KEY = /Fe
OUTPUTOBJ_KEY = /Fo
WARNING_AS_ERROR_KEY = /WX

ifeq ($(runtime),vc7.1)
        WARNING_KEY = /W3
else
        WARNING_KEY = /W4
endif

DYLIB_KEY = /DLL
EXPORT_KEY = /DEF:
NODEFAULTLIB_KEY = /Zl
NOINTRINSIC_KEY = /Oi-

ifeq ($(runtime),vc8)
        OPENMP_FLAG = /openmp
        WARNING_KEY += /Wp64
        CPLUS_FLAGS += /D_USE_RTM_VERSION
endif
ifeq ($(runtime),vc9)
        OPENMP_FLAG = /openmp
endif
ifeq ($(runtime),vc_mt)
        OPENMP_FLAG = /openmp
endif

CPLUS_FLAGS += /DDO_ITT_NOTIFY /DUSE_WINTHREAD /D_CRT_SECURE_NO_DEPRECATE \
        /D_WIN32_WINNT=$(_WIN32_WINNT)
C_FLAGS = $(CPLUS_FLAGS)
#------------------------------------------------------------------------------
# End of setting compiler flags.
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Setting assembler data.
#------------------------------------------------------------------------------
ASSEMBLY_SOURCE=$(arch)-masm
ifeq (intel64,$(arch))
    ASM=ml64 /nologo
    ASM_FLAGS += /DEM64T=1 /c /Zi
    TBB_ASM.OBJ = atomic_support.obj intel64_misc.obj
    MALLOC_ASM.OBJ = atomic_support.obj
else
    ASM=ml /nologo
    ASM_FLAGS += /c /coff /Zi /safeseh
    TBB_ASM.OBJ = atomic_support.obj lock_byte.obj
endif
#------------------------------------------------------------------------------
# End of setting assembler data.
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Setting tbbmalloc data.
#------------------------------------------------------------------------------
M_CPLUS_FLAGS = $(subst $(EH_FLAGS),/EHs-,$(CPLUS_FLAGS))
#------------------------------------------------------------------------------
# End of setting tbbmalloc data.
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# End of define compiler-specific variables.
#------------------------------------------------------------------------------

