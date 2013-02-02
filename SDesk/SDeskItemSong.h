#ifndef _sdesk_item_song_h
#define _sdesk_item_song_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemSong : public SDeskItem
{
public:
    SDeskItemSong(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemSong();

    virtual bool OnKeyDown(u32 nKey);

    virtual CascadeString& getItemDefinition();
};

#endif

