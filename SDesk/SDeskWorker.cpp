#include "SDesk.h"

SDeskWorker::SDeskWorker()
{
    Log(1, "SDeskWorker()\n");

    Log(1, "SDeskWorker() done\n");
}

SDeskWorker::~SDeskWorker()
{
    Log(1, "~SDeskWorker()\n");

    Log(1, "~SDeskWorker() done\n");
}

void SDeskWorker::ThreadProc()
{
    Log(7, "SDeskWorker::ThreadProc()\n");

    while (!SDesk::getInstance().getExitApp()) {
        SDesk::getInstance().RunStatus();
        CascadeThread::Sleep(1000);
    }

    Log(7, "SDeskWorker::ThreadProc() done\n");
}


