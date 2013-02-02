#ifndef _sdesk_popup_h
#define _sdesk_popup_h

#include <cascade/Cascade.h>

#include "SDeskBitmap.h"
#include "SDeskFont.h"

class SDeskPopup
{
private:
    SDeskBitmap mBitmap;
    int mBx, mBy;
    CascadeString mString;
    int mTextYOffset;
    int mTextXOffset;
    SDeskFont mFont;
    CascadeColor mColor;

public:
    SDeskPopup();
    ~SDeskPopup();

    void LoadBitmap(char *filename, char *dir = 0);
    void SetString(const char *str = 0);
    void SetTextPosition(int text_x_offset, int text_y_offset);
    void SetColor(CascadeColor *color);
    void Draw(CascadeBitmap &drawing_bitmap);
    void SetFont(int size) {
        mFont.Set(size);
    }
    void SetColour(CascadeColor *color) {
        SetColor(color);
    }
};

#endif 

