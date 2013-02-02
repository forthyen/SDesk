#include <string.h>

#include "SDeskScan.h"
#include "SDeskScanRadio.h"

SDeskScanRadio::SDeskScanRadio(
    const char* name,
    const char* url)
{
    Log(7, "SDeskScanRadio(%s,%s)\n", name, url);

    mName = CascadeString(name);
    mURL = CascadeString(url);

    Log(7, "SDeskScanRadio() done\n");
}

SDeskScanRadio::~SDeskScanRadio()
{
    Log(7, "~SDeskScanRadio()\n");

    Log(7, "~SDeskScanRadio() done\n");
}

