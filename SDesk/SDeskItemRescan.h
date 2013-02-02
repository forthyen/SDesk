#ifndef _sdesk_item_rescan_h
#define _sdesk_item_rescan_h

#include <cascade/Cascade.h>

#include "SDeskItem.h"

class SDeskItemRescan : public SDeskItem
{
public:
    SDeskItemRescan(const char* name, const char* type, bool prot);
    virtual ~SDeskItemRescan();

    virtual bool OnKeyDown(u32 nKey);

    // The type of scan to perform
    char mScanType[100];
};

#endif

