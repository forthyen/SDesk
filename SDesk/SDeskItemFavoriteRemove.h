#ifndef _sdesk_item_favorite_remove_h
#define _sdesk_item_favorite_remove_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemFavoriteRemove : public SDeskItem
{
public:
    SDeskItemFavoriteRemove(const char *menu, const char* name,
                            const char* filename);
    virtual ~SDeskItemFavoriteRemove();

    virtual bool OnKeyDown(u32 nKey);

    // The menu definition file to remove
    CascadeString mRemoveMenu;

    // The item name to remove
    CascadeString mRemoveItem;
};

#endif

