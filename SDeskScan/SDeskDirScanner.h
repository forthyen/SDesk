#ifndef _sdesk_dir_scanner_h
#define _sdesk_dir_scanner_h

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

#include <cascade/Cascade.h>

class SDeskDirScanner : public CascadeDirScanner
{
private:
	char *path;
	struct stat filestat;

	void GetStat(const char *fn) {
        char buf[400];
        
        sprintf(buf, "%s/%s", path, fn); stat(buf, &filestat);
    }

public:
	SDeskDirScanner(const char * pDirName) 
        : CascadeDirScanner(pDirName)
    {
        path = strdup(pDirName);
    }
	SDeskDirScanner(const char * pDirName, const char * pMatchPrefix) 
        : CascadeDirScanner(pDirName, pMatchPrefix)
    {
        path = strdup(pDirName);
    }
	~SDeskDirScanner() {
        if (path) {
            delete path;
        }
    }

	const char* GetEntry(u32 nIndex) {
        const char *v = CascadeDirScanner::GetEntry(nIndex);
        GetStat(v);
        return v;
    }

	bool IsDirectory() { 
        return S_ISDIR(filestat.st_mode);
    }
	u32 GetSize() { 
        return filestat.st_size;
    }
	time_t GetTime() { 
        return filestat.st_mtime;
    }
	char *GetExtension(const char *name) {
        char *ext = strrchr(name, '.'); 
        if (ext) {
            return (char*) ext + 1;
        } else {
            return ""; 
        }
    }
};

#endif

