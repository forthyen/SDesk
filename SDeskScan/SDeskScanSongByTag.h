#ifndef _sdesk_scan_song_by_tag_h
#define _sdesk_scan_song_by_tag_h

#include <cascade/Cascade.h>

#include "SDeskLog.h"
#include "SDeskScanSong.h"

class SDeskScanSongByTag : SDeskLog
{
public:
    SDeskScanSongByTag(const char* name);
    virtual ~SDeskScanSongByTag();

    CascadeString& getName() {
        return mName;
    }
    
    void addSong (SDeskScanSong *song);

    void DumpTagList(const char* tagName);

    // The items defining the application
    CascadeString mName;

    vector<SDeskScanSong *> mSongList;
};

#endif

