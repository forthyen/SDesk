#include <cascade/Cascade.h>

#include "SDeskLog.h"

class SDeskWorker : public CascadeThread, SDeskLog
{
public:
    SDeskWorker();
    virtual ~SDeskWorker();

protected:
    virtual void ThreadProc();
};

