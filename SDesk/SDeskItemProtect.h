#ifndef _sdesk_item_protect_h
#define _sdesk_item_protect_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemProtect : public SDeskItem
{
public:
    SDeskItemProtect(const char* name, const char* filename);
    virtual ~SDeskItemProtect();

    virtual bool OnKeyDown(u32 nKey);

    // Is the item currently protected
    bool mProtected;
};

#endif

