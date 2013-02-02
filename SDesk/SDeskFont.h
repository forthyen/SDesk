#ifndef _sdesk_font_h
#define _sdesk_font_h

#include <cascade/Cascade.h>

class SDeskFont : public CascadeFont
{
private:
    int mSize;

public:
    bool Set(int size, CascadeFont::WeightFlags flags = CascadeFont::kNormal,
        char* face="system");
    void Draw(CascadeBitmap& bitmap, char* str, int x, int y,
        CascadeColor& color);
    int Size() {
        return mSize;
    }
};

#endif

