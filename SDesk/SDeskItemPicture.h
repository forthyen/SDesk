#ifndef _sdesk_item_picture_h
#define _sdesk_item_picture_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemPicture : public SDeskItem
{
public:
    SDeskItemPicture(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemPicture();

    virtual bool OnKeyDown(u32 nKey);

    virtual CascadeString& getItemDefinition();
};

#endif

