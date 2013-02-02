#include "SDeskPopup.h"

SDeskPopup::SDeskPopup()
{	
    mFont.Set(38);
    mBx = -1;
    mTextYOffset = 33;
    mTextXOffset = 90;
    mColor = CascadeColor(255, 255, 255);
}

SDeskPopup::~SDeskPopup()
{
}

void SDeskPopup::SetString(
    const char *str)
{
    mString = CascadeString(str);
}

void SDeskPopup::SetTextPosition(
    int textXOffset,
    int textYOffset)
{
    mTextXOffset = textXOffset;
    mTextYOffset = textYOffset;
}

void SDeskPopup::SetColor(
    CascadeColor *color)
{
    mColor = *color;
}

void SDeskPopup::LoadBitmap(
    char *filename,
    char *dir)
{ 
    mBitmap.Load(filename, dir); 
}

void SDeskPopup::Draw(
    CascadeBitmap &drawBitmap)
{
    CascadeRect bitmapRect(mBitmap.GetRect());
	
    if (mBx == -1) {
        CascadeRect screenRect(drawBitmap.GetRect());
		
        mBx = screenRect.w / 2 - bitmapRect.w / 2;
        mBy = screenRect.h / 2 - bitmapRect.h / 2;
    }

    drawBitmap.Blit(CascadePoint(mBx, mBy), mBitmap, bitmapRect);
	
    if (mString.GetLength()) {
        drawBitmap.TextOut(CascadePoint(mBx + mTextXOffset,
            mBy + mTextYOffset), mString, mFont, mColor, TEXTOUT_TOP);
    }
}

