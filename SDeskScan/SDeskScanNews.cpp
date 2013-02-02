#include <string.h>

#include "SDeskScan.h"
#include "SDeskScanNews.h"

SDeskScanNews::SDeskScanNews(
    const char* name,
    const char* filename,
    bool prot)
{
    Log(7, "SDeskScanNews(%s,%s,%d)\n", name, filename, prot);

    mName = CascadeString(name);
    mFilename = CascadeString(filename);
    mProtected = prot;

    Log(7, "SDeskScanNews() done\n");
}

SDeskScanNews::~SDeskScanNews()
{
    Log(7, "~SDeskScanNews()\n");

    Log(7, "~SDeskScanNews() done\n");
}

