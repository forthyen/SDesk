#include "SDesk.h"
#include "SDeskFavorite.h"
#include "SDeskItemMenu.h"

SDeskFavorite::SDeskFavorite(
    const char* name,
    const char* filename,
    const char* type)
{
    Log(7, "SDeskFavorite(%s,%s, %s)\n", name, filename, type);

    // Store this item's properties.
    mName = CascadeString(name);
    mFilename = CascadeString(filename);

    // Track the default content for this menu.
    if (strcmp(type, "%songs") == 0) {
        mType = M_SONGS;
    } else if (strcmp(type, "%movies") == 0) {
        mType = M_MOVIES;
    } else if (strcmp(type, "%pictures") == 0) {
        mType = M_PICTURES;
    } else if (strcmp(type, "%radios") == 0) {
        mType = M_RADIOS;
    } else if (strcmp(type, "%slideshow") == 0) {
        mType = M_SLIDESHOW;
    } else {
        mType = M_MISC;
    }

    Log(7, "SDeskFavorite() done\n");
}

SDeskFavorite::~SDeskFavorite()
{
    Log(7, "~SDeskFavorite()\n");

    Log(7, "~SDeskFavorite() done\n");
}


