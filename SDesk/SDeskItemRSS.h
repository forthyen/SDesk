#ifndef _sdesk_item_rss_h
#define _sdesk_item_rss_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemRSS : public SDeskItem
{
public:
    SDeskItemRSS(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemRSS();

    void DrawMessage(CascadeBitmap& bitmap);
    void DrawText(CascadeBitmap& bitmap, int x, int y, int menuType);
    virtual bool OnKeyDown(u32 nKey);
};

#endif

