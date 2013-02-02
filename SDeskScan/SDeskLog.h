#ifndef _sdesk_log_h
#define _sdesk_log_h

#include <stdio.h>
#include <time.h>

class SDeskLog
{
public:
    static void Log(int level, char *format, ...);
    static int LogLevel(int level = -1);
    static void LogFile(char *rootDir, bool val = true);

private:
    static FILE *mFd;
    static int mLvl;
    static time_t mStart;
};

#endif

