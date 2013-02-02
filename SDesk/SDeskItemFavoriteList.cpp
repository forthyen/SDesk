#include "SDesk.h"
#include "SDeskItemFavoriteList.h"

SDeskItemFavoriteList::SDeskItemFavoriteList(
    const char* name,
    const char* filename,
    CascadeString &itemDefinition)
    : SDeskItem(name, filename, false)
{
    Log(7, "SDeskItemFavoriteList(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_FAVORITE_LIST;

    // Store the item definition.
    mItemDefinition = itemDefinition;

    Log(7, "SDeskItemFavoriteList() done\n");
}

SDeskItemFavoriteList::~SDeskItemFavoriteList()
{
    Log(7, "~SDeskItemFavoriteList()\n");

    Log(7, "~SDeskItemFavoriteList() done\n");
}

bool SDeskItemFavoriteList::OnKeyDown(u32 nKey)
{
    const char *str = mFilename;

    Log(7, "SDeskItemFavoriteList::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        FILE *fd;
        const char *definition = mItemDefinition;

        Log(7, "SDeskItemFavoriteList::OnKeyDown(%s)\n",
            str);
        Log(7, "SDeskItemFavoriteList::OnKeyDown(%s)\n",
            definition);

        if ((fd = fopen(str, "a+")) == NULL) {
            Log(8, "fopen failed (%s)...\n", str);
            return false;
        }

        fwrite(definition, sizeof(char), strlen(definition), fd);
        fclose(fd);

        SDesk::getInstance().getCurrentTheme().SetStatusMessage(
            "Added new favorite item...");

        Log(7, "SDeskItemFavoriteList::OnKeyDown() done true\n");
        return true;
    }

    Log(7, "SDeskItemFavoriteList::OnKeyDown() done\n");
    return false;
}

