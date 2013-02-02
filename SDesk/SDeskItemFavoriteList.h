#ifndef _sdesk_item_favorite_list_h
#define _sdesk_item_favorite_list_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemFavoriteList : public SDeskItem
{
public:
    SDeskItemFavoriteList(const char* name, const char* filename,
                          CascadeString& itemDefinition);
    virtual ~SDeskItemFavoriteList();

    virtual bool OnKeyDown(u32 nKey);
};

#endif

