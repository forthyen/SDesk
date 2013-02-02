#include <string.h>

#include "SDeskScan.h"
#include "SDeskScanApp.h"

SDeskScanApp::SDeskScanApp(
    const char* name,
    const char* filename,
    bool prot)
{
    Log(7, "SDeskScanApp(%s,%s,%d)\n", name, filename, prot);

    mName = CascadeString(name);
    mFilename = CascadeString(filename);
    mProtected = prot;

    Log(7, "SDeskScanApp() done\n");
}

SDeskScanApp::~SDeskScanApp()
{
    Log(7, "~SDeskScanApp()\n");

    Log(7, "~SDeskScanApp() done\n");
}

