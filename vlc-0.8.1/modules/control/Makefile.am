
# Autogenerated by bootstrap - DO NOT EDIT - edit Modules.am instead

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = corba
#DIST_SUBDIRS = $(SUBDIRS)
libvlcdir = $(libdir)/vlc/control
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
	if test "$(plugin)" != "no"; then z=$$($(VLC_CONFIG) --target plugin); for mod in gestures showintf http telnet netsync ntservice joystick hotkeys lirc rc rtci; do case "$$z" in */lib$${mod}_plugin*) echo lib$${mod}_plugin$(LIBEXT);; esac; done; fi; \
	if test "$(builtin)" != "no"; then z=$$($(VLC_CONFIG) --target builtin); for mod in gestures showintf http telnet netsync ntservice joystick hotkeys lirc rc rtci; do case "$$z" in */lib$${mod}.a*) echo lib$${mod}.a;; esac; done; fi; \
	if test "$(pic)" != "no"; then z=$$($(VLC_CONFIG) --target builtin pic); for mod in gestures showintf http telnet netsync ntservice joystick hotkeys lirc rc rtci; do case "$$z" in */lib$${mod}_pic.a*) echo lib$${mod}_pic.a;; esac; done; fi; \
	`; case "$$targets" in *lib*) $(MAKE) $(AM_MAKEFLAGS) $$targets || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; ;; esac; \
	test -z "$$fail"

# The gestures plugin

if UNTRUE
Lgesturesp = libgestures_plugin.a
Dgesturesp = libgestures_plugin$(LIBEXT)
Lgesturesb = libgestures.a
Lgesturespic = libgestures_pic.a
endif

libgestures_plugin_a_SOURCES = $(SOURCES_gestures)
libgestures_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gestures`
libgestures_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gestures`
libgestures_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gestures`

libgestures_pic_a_SOURCES = $(SOURCES_gestures)
libgestures_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gestures`
libgestures_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gestures`
libgestures_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gestures`

libgestures_a_SOURCES = $(SOURCES_gestures)
libgestures_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gestures`
libgestures_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gestures`
libgestures_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gestures`

libvlc_LIBRARIES += $(Lgesturesb) $(Lgesturespic)
libgestures_DATA = $(Dgesturesp)

libgesturesdir = $(libdir)/vlc/
noinst_LIBRARIES += $(Lgesturesp)
libgestures_plugin$(LIBEXT): $(libgestures_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage gestures` in \
	  c++) echo $(CXXLINK) $(libgestures_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin gestures\` ; $(CXXLINK) $(libgestures_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin gestures` ;; \
	  objc) echo $(OBJCLINK) $(libgestures_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin gestures\` ; $(OBJCLINK) $(libgestures_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin gestures` ;; \
	  c|*) echo $(LINK) $(libgestures_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin gestures\` ; $(LINK) $(libgestures_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin gestures` ;; \
	esac


# The showintf plugin

if UNTRUE
Lshowintfp = libshowintf_plugin.a
Dshowintfp = libshowintf_plugin$(LIBEXT)
Lshowintfb = libshowintf.a
Lshowintfpic = libshowintf_pic.a
endif

libshowintf_plugin_a_SOURCES = $(SOURCES_showintf)
libshowintf_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin showintf`
libshowintf_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin showintf`
libshowintf_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin showintf`

libshowintf_pic_a_SOURCES = $(SOURCES_showintf)
libshowintf_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic showintf`
libshowintf_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic showintf`
libshowintf_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic showintf`

libshowintf_a_SOURCES = $(SOURCES_showintf)
libshowintf_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin showintf`
libshowintf_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin showintf`
libshowintf_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin showintf`

libvlc_LIBRARIES += $(Lshowintfb) $(Lshowintfpic)
libshowintf_DATA = $(Dshowintfp)

libshowintfdir = $(libdir)/vlc/
noinst_LIBRARIES += $(Lshowintfp)
libshowintf_plugin$(LIBEXT): $(libshowintf_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage showintf` in \
	  c++) echo $(CXXLINK) $(libshowintf_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin showintf\` ; $(CXXLINK) $(libshowintf_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin showintf` ;; \
	  objc) echo $(OBJCLINK) $(libshowintf_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin showintf\` ; $(OBJCLINK) $(libshowintf_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin showintf` ;; \
	  c|*) echo $(LINK) $(libshowintf_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin showintf\` ; $(LINK) $(libshowintf_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin showintf` ;; \
	esac


# The http plugin

if UNTRUE
Lhttpp = libhttp_plugin.a
Dhttpp = libhttp_plugin$(LIBEXT)
Lhttpb = libhttp.a
Lhttppic = libhttp_pic.a
endif

libhttp_plugin_a_SOURCES = $(SOURCES_http)
libhttp_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin http`
libhttp_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin http`
libhttp_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin http`

libhttp_pic_a_SOURCES = $(SOURCES_http)
libhttp_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic http`
libhttp_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic http`
libhttp_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic http`

libhttp_a_SOURCES = $(SOURCES_http)
libhttp_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin http`
libhttp_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin http`
libhttp_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin http`

libvlc_LIBRARIES += $(Lhttpb) $(Lhttppic)
libhttp_DATA = $(Dhttpp)

libhttpdir = $(libdir)/vlc/
noinst_LIBRARIES += $(Lhttpp)
libhttp_plugin$(LIBEXT): $(libhttp_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage http` in \
	  c++) echo $(CXXLINK) $(libhttp_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin http\` ; $(CXXLINK) $(libhttp_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin http` ;; \
	  objc) echo $(OBJCLINK) $(libhttp_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin http\` ; $(OBJCLINK) $(libhttp_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin http` ;; \
	  c|*) echo $(LINK) $(libhttp_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin http\` ; $(LINK) $(libhttp_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin http` ;; \
	esac


# The telnet plugin

if UNTRUE
Ltelnetp = libtelnet_plugin.a
Dtelnetp = libtelnet_plugin$(LIBEXT)
Ltelnetb = libtelnet.a
Ltelnetpic = libtelnet_pic.a
endif

libtelnet_plugin_a_SOURCES = $(SOURCES_telnet)
libtelnet_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin telnet`
libtelnet_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin telnet`
libtelnet_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin telnet`

libtelnet_pic_a_SOURCES = $(SOURCES_telnet)
libtelnet_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic telnet`
libtelnet_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic telnet`
libtelnet_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic telnet`

libtelnet_a_SOURCES = $(SOURCES_telnet)
libtelnet_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin telnet`
libtelnet_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin telnet`
libtelnet_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin telnet`

libvlc_LIBRARIES += $(Ltelnetb) $(Ltelnetpic)
libtelnet_DATA = $(Dtelnetp)

libtelnetdir = $(libdir)/vlc/
noinst_LIBRARIES += $(Ltelnetp)
libtelnet_plugin$(LIBEXT): $(libtelnet_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage telnet` in \
	  c++) echo $(CXXLINK) $(libtelnet_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin telnet\` ; $(CXXLINK) $(libtelnet_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin telnet` ;; \
	  objc) echo $(OBJCLINK) $(libtelnet_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin telnet\` ; $(OBJCLINK) $(libtelnet_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin telnet` ;; \
	  c|*) echo $(LINK) $(libtelnet_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin telnet\` ; $(LINK) $(libtelnet_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin telnet` ;; \
	esac


# The netsync plugin

if UNTRUE
Lnetsyncp = libnetsync_plugin.a
Dnetsyncp = libnetsync_plugin$(LIBEXT)
Lnetsyncb = libnetsync.a
Lnetsyncpic = libnetsync_pic.a
endif

libnetsync_plugin_a_SOURCES = $(SOURCES_netsync)
libnetsync_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin netsync`
libnetsync_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin netsync`
libnetsync_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin netsync`

libnetsync_pic_a_SOURCES = $(SOURCES_netsync)
libnetsync_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic netsync`
libnetsync_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic netsync`
libnetsync_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic netsync`

libnetsync_a_SOURCES = $(SOURCES_netsync)
libnetsync_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin netsync`
libnetsync_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin netsync`
libnetsync_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin netsync`

libvlc_LIBRARIES += $(Lnetsyncb) $(Lnetsyncpic)
libnetsync_DATA = $(Dnetsyncp)

libnetsyncdir = $(libdir)/vlc/
noinst_LIBRARIES += $(Lnetsyncp)
libnetsync_plugin$(LIBEXT): $(libnetsync_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage netsync` in \
	  c++) echo $(CXXLINK) $(libnetsync_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin netsync\` ; $(CXXLINK) $(libnetsync_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin netsync` ;; \
	  objc) echo $(OBJCLINK) $(libnetsync_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin netsync\` ; $(OBJCLINK) $(libnetsync_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin netsync` ;; \
	  c|*) echo $(LINK) $(libnetsync_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin netsync\` ; $(LINK) $(libnetsync_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin netsync` ;; \
	esac


# The ntservice plugin

if UNTRUE
Lntservicep = libntservice_plugin.a
Dntservicep = libntservice_plugin$(LIBEXT)
Lntserviceb = libntservice.a
Lntservicepic = libntservice_pic.a
endif

libntservice_plugin_a_SOURCES = $(SOURCES_ntservice)
libntservice_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin ntservice`
libntservice_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin ntservice`
libntservice_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin ntservice`

libntservice_pic_a_SOURCES = $(SOURCES_ntservice)
libntservice_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic ntservice`
libntservice_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic ntservice`
libntservice_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic ntservice`

libntservice_a_SOURCES = $(SOURCES_ntservice)
libntservice_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin ntservice`
libntservice_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin ntservice`
libntservice_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin ntservice`

libvlc_LIBRARIES += $(Lntserviceb) $(Lntservicepic)
libntservice_DATA = $(Dntservicep)

libntservicedir = $(libdir)/vlc/
noinst_LIBRARIES += $(Lntservicep)
libntservice_plugin$(LIBEXT): $(libntservice_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage ntservice` in \
	  c++) echo $(CXXLINK) $(libntservice_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin ntservice\` ; $(CXXLINK) $(libntservice_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin ntservice` ;; \
	  objc) echo $(OBJCLINK) $(libntservice_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin ntservice\` ; $(OBJCLINK) $(libntservice_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin ntservice` ;; \
	  c|*) echo $(LINK) $(libntservice_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin ntservice\` ; $(LINK) $(libntservice_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin ntservice` ;; \
	esac


# The joystick plugin

if UNTRUE
Ljoystickp = libjoystick_plugin.a
Djoystickp = libjoystick_plugin$(LIBEXT)
Ljoystickb = libjoystick.a
Ljoystickpic = libjoystick_pic.a
endif

libjoystick_plugin_a_SOURCES = $(SOURCES_joystick)
libjoystick_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin joystick`
libjoystick_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin joystick`
libjoystick_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin joystick`

libjoystick_pic_a_SOURCES = $(SOURCES_joystick)
libjoystick_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic joystick`
libjoystick_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic joystick`
libjoystick_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic joystick`

libjoystick_a_SOURCES = $(SOURCES_joystick)
libjoystick_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin joystick`
libjoystick_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin joystick`
libjoystick_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin joystick`

libvlc_LIBRARIES += $(Ljoystickb) $(Ljoystickpic)
libjoystick_DATA = $(Djoystickp)

libjoystickdir = $(libdir)/vlc/
noinst_LIBRARIES += $(Ljoystickp)
libjoystick_plugin$(LIBEXT): $(libjoystick_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage joystick` in \
	  c++) echo $(CXXLINK) $(libjoystick_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin joystick\` ; $(CXXLINK) $(libjoystick_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin joystick` ;; \
	  objc) echo $(OBJCLINK) $(libjoystick_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin joystick\` ; $(OBJCLINK) $(libjoystick_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin joystick` ;; \
	  c|*) echo $(LINK) $(libjoystick_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin joystick\` ; $(LINK) $(libjoystick_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin joystick` ;; \
	esac


# The hotkeys plugin

if UNTRUE
Lhotkeysp = libhotkeys_plugin.a
Dhotkeysp = libhotkeys_plugin$(LIBEXT)
Lhotkeysb = libhotkeys.a
Lhotkeyspic = libhotkeys_pic.a
endif

libhotkeys_plugin_a_SOURCES = $(SOURCES_hotkeys)
libhotkeys_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin hotkeys`
libhotkeys_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin hotkeys`
libhotkeys_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin hotkeys`

libhotkeys_pic_a_SOURCES = $(SOURCES_hotkeys)
libhotkeys_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic hotkeys`
libhotkeys_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic hotkeys`
libhotkeys_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic hotkeys`

libhotkeys_a_SOURCES = $(SOURCES_hotkeys)
libhotkeys_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin hotkeys`
libhotkeys_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin hotkeys`
libhotkeys_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin hotkeys`

libvlc_LIBRARIES += $(Lhotkeysb) $(Lhotkeyspic)
libhotkeys_DATA = $(Dhotkeysp)

libhotkeysdir = $(libdir)/vlc/
noinst_LIBRARIES += $(Lhotkeysp)
libhotkeys_plugin$(LIBEXT): $(libhotkeys_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage hotkeys` in \
	  c++) echo $(CXXLINK) $(libhotkeys_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin hotkeys\` ; $(CXXLINK) $(libhotkeys_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin hotkeys` ;; \
	  objc) echo $(OBJCLINK) $(libhotkeys_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin hotkeys\` ; $(OBJCLINK) $(libhotkeys_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin hotkeys` ;; \
	  c|*) echo $(LINK) $(libhotkeys_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin hotkeys\` ; $(LINK) $(libhotkeys_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin hotkeys` ;; \
	esac


# The lirc plugin

if UNTRUE
Llircp = liblirc_plugin.a
Dlircp = liblirc_plugin$(LIBEXT)
Llircb = liblirc.a
Llircpic = liblirc_pic.a
endif

liblirc_plugin_a_SOURCES = $(SOURCES_lirc)
liblirc_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin lirc`
liblirc_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin lirc`
liblirc_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin lirc`

liblirc_pic_a_SOURCES = $(SOURCES_lirc)
liblirc_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic lirc`
liblirc_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic lirc`
liblirc_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic lirc`

liblirc_a_SOURCES = $(SOURCES_lirc)
liblirc_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin lirc`
liblirc_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin lirc`
liblirc_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin lirc`

libvlc_LIBRARIES += $(Llircb) $(Llircpic)
liblirc_DATA = $(Dlircp)

liblircdir = $(libdir)/vlc/
noinst_LIBRARIES += $(Llircp)
liblirc_plugin$(LIBEXT): $(liblirc_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage lirc` in \
	  c++) echo $(CXXLINK) $(liblirc_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin lirc\` ; $(CXXLINK) $(liblirc_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin lirc` ;; \
	  objc) echo $(OBJCLINK) $(liblirc_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin lirc\` ; $(OBJCLINK) $(liblirc_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin lirc` ;; \
	  c|*) echo $(LINK) $(liblirc_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin lirc\` ; $(LINK) $(liblirc_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin lirc` ;; \
	esac


# The rc plugin

if UNTRUE
Lrcp = librc_plugin.a
Drcp = librc_plugin$(LIBEXT)
Lrcb = librc.a
Lrcpic = librc_pic.a
endif

librc_plugin_a_SOURCES = $(SOURCES_rc)
librc_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin rc`
librc_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin rc`
librc_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin rc`

librc_pic_a_SOURCES = $(SOURCES_rc)
librc_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic rc`
librc_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic rc`
librc_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic rc`

librc_a_SOURCES = $(SOURCES_rc)
librc_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin rc`
librc_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin rc`
librc_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin rc`

libvlc_LIBRARIES += $(Lrcb) $(Lrcpic)
librc_DATA = $(Drcp)

librcdir = $(libdir)/vlc/
noinst_LIBRARIES += $(Lrcp)
librc_plugin$(LIBEXT): $(librc_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage rc` in \
	  c++) echo $(CXXLINK) $(librc_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin rc\` ; $(CXXLINK) $(librc_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin rc` ;; \
	  objc) echo $(OBJCLINK) $(librc_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin rc\` ; $(OBJCLINK) $(librc_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin rc` ;; \
	  c|*) echo $(LINK) $(librc_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin rc\` ; $(LINK) $(librc_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin rc` ;; \
	esac


# The rtci plugin

if UNTRUE
Lrtcip = librtci_plugin.a
Drtcip = librtci_plugin$(LIBEXT)
Lrtcib = librtci.a
Lrtcipic = librtci_pic.a
endif

librtci_plugin_a_SOURCES = $(SOURCES_rtci)
librtci_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin rtci`
librtci_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin rtci`
librtci_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin rtci`

librtci_pic_a_SOURCES = $(SOURCES_rtci)
librtci_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic rtci`
librtci_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic rtci`
librtci_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic rtci`

librtci_a_SOURCES = $(SOURCES_rtci)
librtci_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin rtci`
librtci_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin rtci`
librtci_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin rtci`

libvlc_LIBRARIES += $(Lrtcib) $(Lrtcipic)
librtci_DATA = $(Drtcip)

librtcidir = $(libdir)/vlc/
noinst_LIBRARIES += $(Lrtcip)
librtci_plugin$(LIBEXT): $(librtci_plugin_a_OBJECTS)
	@case `$(VLC_CONFIG) --linkage rtci` in \
	  c++) echo $(CXXLINK) $(librtci_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin rtci\` ; $(CXXLINK) $(librtci_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin rtci` ;; \
	  objc) echo $(OBJCLINK) $(librtci_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin rtci\` ; $(OBJCLINK) $(librtci_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin rtci` ;; \
	  c|*) echo $(LINK) $(librtci_plugin_a_OBJECTS) \`$(VLC_CONFIG) --libs plugin rtci\` ; $(LINK) $(librtci_plugin_a_OBJECTS) `$(VLC_CONFIG) --libs plugin rtci` ;; \
	esac


