#ifndef _sdesk_setup_h
#define _sdesk_setup_h

#include <cascade/Cascade.h>

class SDeskSetup : public SDeskLog
{
public:
    SDeskSetup();
    virtual ~SDeskSetup();

    void ReScan(const char* type);

private:
    char mCommand[1024];
};

#endif
