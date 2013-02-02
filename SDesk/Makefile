SDK_MAKEFILE_DIR := /usr/local/roku/sdk/makefiles

EXTRA_CXXFLAGS = -I. -Wall -I../id3lib-3.8.3/include -I../mxml-2.2.2
EXTRA_LDFLAGS = -lCascade 

ifdef ROKU
include $(SDK_MAKEFILE_DIR)/CascadeSDKProject.mk

$(OBJECT_DIR)/$(PROGRAMS): $(ALL_OBJECTS) $(OBJECT_DIR)/.made_object_dirs
	$(CXX) $(LDFLAGS) -o $@ $(ALL_OBJECTS) ../id3lib-3.8.3/src/.libs/libid3.a ../id3lib-3.8.3/zlib/src/.libs/libz.a ../mxml-2.2.2/libmxml.a

all:
	cp SDesk SDesk.roku
	strip SDesk.roku
	strip SDeskScan
else
deploy:
	-rm -f SDesk.tar.gz
	-rm -f SDesk.zip
	tar cvzf SDesk.tar.gz SDesk.roku SDeskScan README themes apps menus images etc var SDeskScan.cfg.example SDeskRunOnce
	zip -r SDesk.zip SDesk.roku SDeskScan README themes apps menus images etc var SDeskScan.cfg.example SDeskRunOnce
endif

