#ifndef _sdesk_item_url_h
#define _sdesk_item_url_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemURL : public SDeskItem
{
public:
    SDeskItemURL(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemURL();

    virtual bool OnKeyDown(u32 nKey);

    // Internal item properties.
    CascadeString mURL;
};

#endif

