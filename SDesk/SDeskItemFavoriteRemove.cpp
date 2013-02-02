#include <unistd.h>

#include "SDesk.h"
#include "SDeskItemFavoriteRemove.h"

SDeskItemFavoriteRemove::SDeskItemFavoriteRemove(
    const char* menu,
    const char* name,
    const char* filename)
    : SDeskItem("Remove from Favorites", filename, false)
{
    Log(7, "SDeskItemFavoriteRemove(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_FAVORITE_REMOVE;

    // Assign the remove menu name
    mRemoveMenu = menu;

    // Assign the remove name
    mRemoveItem = name;
    mRemoveItem += "\n";

    Log(7, "SDeskItemFavoriteRemove() done\n");
}

SDeskItemFavoriteRemove::~SDeskItemFavoriteRemove()
{
    Log(7, "~SDeskItemFavoriteRemove()\n");

    Log(7, "~SDeskItemFavoriteRemove() done\n");
}

bool SDeskItemFavoriteRemove::OnKeyDown(u32 nKey)
{
    const char *str = mRemoveMenu;
    const char *itemName = mRemoveItem;
    char *pos;
    char outFile[1024];
    char outBakFile[1024];
    char outBuf[4096];
    char buf[1024];
    bool skipItem = false;

    Log(7, "SDeskItemFavoriteRemove::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        FILE *oldFd, *newFd;
 
        strcpy(outFile, str);
        strcat(outFile, ".new");
        strcpy(outBakFile, str);
        strcat(outBakFile, ".bak");
        Log(7, "SDeskItemFavoriteRemove::OnKeyDown(%s, %s)\n",
            str, outFile);

        if ((oldFd = fopen(str, "r")) == NULL) {
            Log(8, "fopen failed (%s)...\n", str);
            return false;
        }

        if ((newFd = fopen(outFile, "w+")) == NULL) {
            Log(8, "fopen failed (%s)...\n", outFile);
            fclose(oldFd);
            return false;
        }

        outBuf[0] = '\0';
        while ((pos = fgets(buf, 1023, oldFd)) != 0) {
            if (strcmp(buf, "%movie\n") == 0 ||
                strcmp(buf, "%radio\n") == 0 ||
                strcmp(buf, "%picture\n") == 0 ||
                strcmp(buf, "%song\n") == 0 ||
                strcmp(buf, "%menu\n") == 0) {
                // We reached the next item...
                if (skipItem) {
                    // We are inside the item to be removed.
                    strcpy(outBuf, buf);
                    skipItem = false;
                } else {
                    // Write the item to file...
                    fwrite(outBuf, sizeof(char), strlen(outBuf), newFd);
                    strcpy(outBuf, buf);
                }
                continue;
            }
            if (strcmp(buf, itemName) == 0) {
                // We reached the item to be removed...
                skipItem = true;
                continue;
            }
            strcat(outBuf, buf);
        }

        // Write the last item to file...
        if (!skipItem) {
            fwrite(outBuf, sizeof(char), strlen(outBuf), newFd);
        }

        fclose(newFd);
        fclose(oldFd);

        // Put the new file in place.
        unlink(outBakFile);
        rename(str, outBakFile);
        rename(outFile, str);

        SDesk::getInstance().getCurrentTheme().SetStatusMessage(
            "Removed favorite from list...");

        Log(7, "SDeskItemFavoriteRemove::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemFavoriteRemove::OnKeyDown() done\n");
    return false;
}

