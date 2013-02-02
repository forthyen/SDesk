#ifndef _sdesk_item_app_h
#define _sdesk_item_app_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemApp : public SDeskItem
{
public:
    SDeskItemApp(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemApp();

    virtual bool OnKeyDown(u32 nKey);
};

#endif

