#ifndef _sdesk_item_rssfeed_h
#define _sdesk_item_rssfeed_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemRSSFeed : public SDeskItem
{
public:
    SDeskItemRSSFeed(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemRSSFeed();

    virtual bool OnKeyDown(u32 nKey);
};

#endif

