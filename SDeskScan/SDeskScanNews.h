#ifndef _sdesk_scan_news_h
#define _sdesk_scan_news_h

#include <cascade/Cascade.h>

#include "SDeskLog.h"

class SDeskScanNews : SDeskLog
{
public:
    SDeskScanNews(const char* name, const char* filename, bool prot);
    virtual ~SDeskScanNews();

    CascadeString& getName() {
        return mName;
    }
    CascadeString& getFileName() {
        return mFilename;
    }
    bool getProtected() {
        return mProtected;
    }

    // The items defining the application
    CascadeString mName;
    CascadeString mFilename;

    bool mProtected;
};

#endif

