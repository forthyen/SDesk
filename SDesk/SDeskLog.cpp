#include <string.h>
#include <stdarg.h>

#include "SDeskLog.h"

FILE *SDeskLog::mFd = 0;
int SDeskLog::mLvl = 9;
time_t SDeskLog::mStart = 0;

int SDeskLog::LogLevel(
    int l)
{
    if (l != -1) {
        SDeskLog::mLvl = l;
    }
    return SDeskLog::mLvl;
}

void SDeskLog::LogFile(
    char *rootDir,
    bool val)
{
    if ((!val) && SDeskLog::mFd) {
        fclose(SDeskLog::mFd);
        SDeskLog::mFd = 0;
    } else {
        char buffer[1024];
        
        strcpy(buffer, rootDir);
        strcat(buffer, "/sdesklog");
        if (val && (!SDeskLog::mFd)) {
            SDeskLog::mFd = fopen(buffer, "w");
        }
    }
}

void SDeskLog::Log(
    int level,
    char *format,
    ...)
{
    if (level > SDeskLog::mLvl) {
        return;
    }

    time_t t;
    va_list marker;
    va_start(marker, format);


    if (SDeskLog::mFd) {
        time(&t);
        if (!SDeskLog::mStart) {
            SDeskLog::mStart = t;
        }
        
        t = t - SDeskLog::mStart;
        
        printf("%02ld:%02ld ", t / 60, t % 60);
        
        vprintf(format, marker);
		
        fprintf(SDeskLog::mFd, "%02ld:%02ld ", t / 60, t % 60);
        vfprintf(SDeskLog::mFd, format, marker);
        fflush(SDeskLog::mFd);
    }
}

