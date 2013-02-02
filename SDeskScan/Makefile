SDK_MAKEFILE_DIR := /usr/local/roku/sdk/makefiles

EXTRA_CXXFLAGS = -I. -Wall -I../id3lib-3.8.3/include -I../mxml-2.2.1
EXTRA_LDFLAGS = -lCascade

include $(SDK_MAKEFILE_DIR)/CascadeSDKProject.mk

$(OBJECT_DIR)/$(PROGRAMS): $(ALL_OBJECTS) $(OBJECT_DIR)/.made_object_dirs
	$(CXX) $(LDFLAGS) -o $@ $(ALL_OBJECTS) -L. ../id3lib-3.8.3/src/.libs/libid3.a ../id3lib-3.8.3/zlib/src/.libs/libz.a ../mxml-2.2.1/libmxml.a
	strip SDeskScan

all:
	cp SDeskScan ../SDesk
