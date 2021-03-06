# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.

ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

#===== USEFILE - the file containing the usage message for the application. 
USEFILE=$(PROJECT_ROOT)/src/Usemsg

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PROJECT_ROOT)/src  \
	C:/ide-4.6-workspace/firstMenu/headers  \
	C:/ide-4.6-workspace/firstMenu/lib  \
	C:/ide-4.6-workspace/firstMenu/dbcode  \
	$(PROJECT_ROOT)/phutils

#===== POST_BUILD - extra steps to do after building the image.
define POST_BUILD
phabbind $(BUILDNAME) $(PHAB_MODULES)
endef

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PROJECT_ROOT)/lib

#===== EXTRA_LIBVPATH - a space-separated list of directories to search for library files.
EXTRA_LIBVPATH+=C:/ide-4.6-workspace/firstMenu/lib

#===== LIBS - a space-separated list of library items to be included in the link.
LIBS+=-Bstatic sqlitewrapped sqlite3 -Bdynamic

include $(MKFILES_ROOT)/qmacros.mk
ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif
include $(QNX_INTERNAL)
postbuild:
	$(POST_BUILD)

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))

