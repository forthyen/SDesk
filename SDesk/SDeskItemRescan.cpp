#include <unistd.h>

#include "SDesk.h"
#include "SDeskItemRescan.h"

SDeskItemRescan::SDeskItemRescan(
    const char* name,
    const char* type,
    bool prot)
    : SDeskItem(name, "", prot)
{
    Log(7, "SDeskItemRescan(%s,%s)\n", name, type);

    // Track this items type.
    mType = I_RESCAN;

    // Determine the scan type.
    strcpy(mScanType, type);
    Log(7, "SDeskItemRescan() done\n");
}

SDeskItemRescan::~SDeskItemRescan()
{
    Log(7, "~SDeskItemRescan()\n");

    Log(7, "~SDeskItemRescan() done\n");
}

bool SDeskItemRescan::OnKeyDown(u32 nKey)
{
    Log(7, "SDeskItemRescan::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        SDesk::getInstance().getCurrentTheme().ShowPopup(
            "Building menus...");

        // Setup the rescan.
        SDesk::getInstance().setScanType(mScanType);

        // Update status message
        SDesk::getInstance().getCurrentTheme().SetStatusMessage(
            "Rescanning menus...");

        Log(7, "SDeskItemRescan::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemRescan::OnKeyDown() done\n");
    return false;
}

