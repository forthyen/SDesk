#ifndef _sdesk_scan_app_h
#define _sdesk_scan_app_h

#include <cascade/Cascade.h>

#include "SDeskLog.h"

class SDeskScanApp : SDeskLog
{
public:
    SDeskScanApp(const char* name, const char* filename, bool prot);
    virtual ~SDeskScanApp();

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

