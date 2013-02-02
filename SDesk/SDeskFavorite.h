#ifndef _sdesk_favorite_h
#define _sdesk_favorite_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskFavorite : public SDeskLog
{
public:
    SDeskFavorite(const char* name, const char* filename, const char* type);
    virtual ~SDeskFavorite();

    CascadeString& getName() {
        return mName;
    }
    CascadeString& getFileName() {
        return mFilename;
    }
    int getType() {
        return mType;
    }

    // Internal item properties.
    CascadeString mName;
    CascadeString mFilename;

    // The item type.
    int mType;
};

#endif

