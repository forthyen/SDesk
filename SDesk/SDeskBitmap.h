#ifndef _sdesk_bitmap_h
#define _sdesk_bitmap_h

#include <cascade/Cascade.h>
#include <cascade/util/CascadeJpegFile.h>

#include "SDeskLog.h"

class SDeskBitmap : public CascadeBitmap , SDeskLog
{
public:
    SDeskBitmap();

    // Share memory management.
    static void InitSharedMem(u32 width, u32 height);
    static void CloseSharedMem();

    bool Load(const char* fn, const char* dir = 0);
    void Draw(CascadeBitmap& bitmap, int x = 0, int y = 0,
        int w = -1, int h = -1);
    u32 GetWidth() {
        return GetRect().w;
    }
    u32 GetHeight() {
        return GetRect().h;
    }
    bool isLoaded() {
        return mLoaded;
    }

private:
    bool mLoaded;

    bool mIsJPG;

    CascadeJpegFile mJPG;
};

#endif

