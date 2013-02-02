#ifndef _sdesk_scan_song_h
#define _sdesk_scan_song_h

#include <cascade/Cascade.h>

#include "SDeskLog.h"

class SDeskScanSong : SDeskLog
{
public:
    SDeskScanSong(const char* name, const char* url);
    virtual ~SDeskScanSong();

    CascadeString& getName() {
        return mName;
    }
    CascadeString& getLocation() {
        return mLocation;
    }
    bool getProtected() {
        return mProtected;
    }

    // The items defining the application
    CascadeString mName;
    CascadeString mLocation;
    bool mProtected;
};

#endif

