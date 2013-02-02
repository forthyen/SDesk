#ifndef _sdesk_item_dvd_h
#define _sdesk_item_dvd_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemDVD : public SDeskItem
{
public:
    SDeskItemDVD(const char* name, const char* filename, bool prot);
    virtual ~SDeskItemDVD();

    virtual bool OnKeyDown(u32 nKey);

    virtual CascadeString& getItemDefinition();
};

#endif

