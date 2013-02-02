#ifndef _sdesk_scan_genre_h
#define _sdesk_scan_genre_h

#include <cascade/Cascade.h>

#include "SDeskLog.h"

enum tmpGENRE { G_OTHER, G_ROCK, G_POP, G_JAZZ, G_CLASSIC, G_DANCE,
                G_TECHNO, G_BLUES, G_GOTH, G_RB, G_COUNTRY, G_NEW_AGE,
                G_TALK, G_REGGAE, G_AMBIENT, G_FOLK, G_LATIN, G_OLDIES,
                G_CHRISTIAN, G_NATIONAL, G_ALL };

class SDeskScanGenre : SDeskLog
{
public:
    SDeskScanGenre(const char* name, int genre);
    virtual ~SDeskScanGenre();

    CascadeString& getName() {
        return mName;
    }
    int getGenre() {
        return mGenre;
    }
    bool isGenre(const char *genre);
    
    void addPattern (const char* pattern) {
        mPatterns.push_back(new string(pattern));
    }
    void addRadio (SDeskScanRadio *radio) {
        mRadioList.push_back(radio);
    }

    void DumpGenre();

    // The items defining the application
    CascadeString mName;
    int mGenre;

    vector<string *> mPatterns;
    vector<SDeskScanRadio *> mRadioList;
};

#endif

