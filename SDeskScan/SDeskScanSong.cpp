#include <string.h>

#include "SDeskScan.h"
#include "SDeskScanSong.h"

SDeskScanSong::SDeskScanSong(
    const char* name,
    const char* location)
{
    Log(7, "SDeskScanSong(%s,%s)\n", name, location);

    struct stat filestat;
    char buffer[1024];

    mName = CascadeString(name);
    mLocation = CascadeString(location);
    
    strcpy(buffer, mLocation);
    strcat(buffer, ".sdprot");
    if (stat(buffer, &filestat) != 0) {
        mProtected = false;
    } else {
        mProtected = true;
    }

    Log(7, "SDeskScanSong() done\n");
}

SDeskScanSong::~SDeskScanSong()
{
    Log(7, "~SDeskScanSong()\n");

    Log(7, "~SDeskScanSong() done\n");
}

