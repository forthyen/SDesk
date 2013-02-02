#include "SDeskFont.h"

bool SDeskFont::Set(
    int size,
    CascadeFont::WeightFlags flags,
    char* face)
{
    mSize = size;
    CascadeFont::Attributes attr(face, flags, mSize);
    SetAttributes(attr);
    return true;
}

void SDeskFont::Draw(
    CascadeBitmap& bitmap,
    char* str,
    int x,
    int y,
    CascadeColor& color)
{
    bitmap.TextOut(CascadePoint(x, y), CascadeString(str),
        *this, color, TEXTOUT_TOP);
}

