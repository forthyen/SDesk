#include <unistd.h>

#include "SDesk.h"
#include "SDeskItemProtect.h"

SDeskItemProtect::SDeskItemProtect(
    const char* name,
    const char* filename)
    : SDeskItem("", filename, false)
{
    char protName[1024];
    struct stat filestat;

    Log(7, "SDeskItemProtect(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_PROTECT;

    strcpy(protName, filename);
    strcat(protName, ".sdprot");
    if (stat(protName, &filestat) == 0) {
        mName = "Unprotect Item";
        mProtected = true;
    } else {
        mName = "Protect Item";
        mProtected = false;
    }

    Log(7, "SDeskItemProtect() done\n");
}

SDeskItemProtect::~SDeskItemProtect()
{
    Log(7, "~SDeskItemProtect()\n");

    Log(7, "~SDeskItemProtect() done\n");
}

bool SDeskItemProtect::OnKeyDown(u32 nKey)
{
    char protName[1024];

    Log(7, "SDeskItemProtect::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        strcpy(protName, mFilename);
        strcat(protName, ".sdprot");

        if (mProtected) {
            unlink(protName);
        } else {
            FILE *fd;

            if ((fd = fopen(protName, "w+")) == NULL) {
                Log(8, "fopen failed (%s)...\n", protName);
                return false;
            }
            fclose(fd);
        }
        Log(7, "SDeskItemProtect::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemProtect::OnKeyDown() done\n");
    return false;
}
