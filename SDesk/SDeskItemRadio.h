#ifndef _sdesk_item_radio_h
#define _sdesk_item_radio_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemRadio : public SDeskItem
{
public:
    SDeskItemRadio(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemRadio();

    virtual bool OnKeyDown(u32 nKey);

    virtual CascadeString& getItemDefinition();
};

#endif

