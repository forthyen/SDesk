#ifndef _sdesk_scan_radio_h
#define _sdesk_scan_radio_h

#include <cascade/Cascade.h>

#include "SDeskLog.h"

class SDeskScanRadio : SDeskLog
{
public:
    SDeskScanRadio(const char* name, const char* url);
    virtual ~SDeskScanRadio();

    CascadeString& getName() {
        return mName;
    }
    CascadeString& getURL() {
        return mURL;
    }

    // The items defining the application
    CascadeString mName;
    CascadeString mURL;
};

#endif

