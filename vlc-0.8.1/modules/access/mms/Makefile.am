
# Autogenerated by bootstrap - DO NOT EDIT - edit Modules.am instead

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
#DIST_SUBDIRS = $(SUBDIRS)
libvlcdir = $(libdir)/vlc/access
include Modules.am

if BUILD_MOZILLA
if HAVE_WIN32
# There's no need for pic code on win32 so get rid of this to substantially
# reduce the compilation time.
pic = no
endif
else
pic = no
endif

clean-local:
	-rm -f *.a *.so *.dll *.sl *.dylib

all: all-modules
all-modules:
	@set fnord $$MAKEFLAGS; amf=$$2; targets=`\
	if test "$(plugin)" != "no"; then z=$$($(VLC_CONFIG) --target plugin); for mod in access_mms; do case "$$z" in */lib$${mod}_plugin*) echo lib$${mod}_plugin$(LIBEXT);; esac; done; fi; \
	if test "$(builtin)" != "no"; then z=$$($(VLC_CONFIG) --target builtin); for mod in access_mms; do case "$$z" in */lib$${mod}.a*) echo lib$${mod}.a;; esac; done; fi; \
	if test "$(pic)" != "no"; then z=$$($(VLC_CONFIG) --target builtin pic); for mod in access_mms; do case "$$z" in */lib$${mod}_pic.a*) echo lib$${mod}_pic.a;; esac; done; fi; \
	`; case "$$targets" in *lib*) $(MAKE) $(AM_MAKEFLAGS) $$targets || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; ;; esac; \
	test -z "$$fail"

# The access_mms plugin

if UNTRUE
Laccess_mmsp = libaccess_mms_plugin.a
Daccess_mmsp = libaccess_mms_plugin$(LIBEXT)
Laccess_mmsb = libaccess_mms.a
Laccess_mmspic = libaccess_mms_pic.a
endif

libaccess_mms_plugin_a_SOURCES = $(SOURCES_access_mms)
libaccess_mms_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin access_mms`
libaccess_mms_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin access_mms`
libaccess_mms_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin access_mms`

libaccess_mms_pic_a_SOURCES = $(SOURCES_access_mms)
libaccess_mms_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic access_mms`
libaccess_mms_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic access_mms`
libaccess_mms_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic access_mms`

libaccess_mms_a_SOURCES = $(SOURCES_access_mms)
libaccess_mms_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin access_mms`
libaccess_mms_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin access_mms`
libaccess_mms_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin access_mms`

libvlc_LIBRARIES += $(Laccess_mmsb) $(Laccess_mmspic)
libaccess_mms_DATA = $(Daccess_mmsp)

libaccess_mmsdir = $(libdir)/vlc/
noinst_LIBRARIES += $(Laccess_mmsp)
libaccess_mms_plugin$(LIBEXT): $(libaccess_mms_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage access_mms` in \
	  c++) echo $(CXXLINK) $(libaccess_mms_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin access_mms\` ; $(CXXLINK) $(libaccess_mms_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin access_mms` ;; \
	  objc) echo $(OBJCLINK) $(libaccess_mms_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin access_mms\` ; $(OBJCLINK) $(libaccess_mms_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin access_mms` ;; \
	  c|*) echo $(LINK) $(libaccess_mms_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin access_mms\` ; $(LINK) $(libaccess_mms_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin access_mms` ;; \
	esac


