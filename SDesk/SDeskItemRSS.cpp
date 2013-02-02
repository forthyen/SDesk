#include "SDesk.h"
#include "SDeskItemRSS.h"

SDeskItemRSS::SDeskItemRSS(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemRSS(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_RSS;

    Log(7, "SDeskItemRSS() done\n");
}

SDeskItemRSS::~SDeskItemRSS()
{
    Log(7, "~SDeskItemRSS()\n");

    Log(7, "~SDeskItemRSS() done\n");
}

void SDeskItemRSS::DrawMessage(
    CascadeBitmap& bitmap)
{
    int x = SDesk::getInstance().getCurrentTheme().getTextTopOffset();
    int y = SDesk::getInstance().getCurrentTheme().getTextLeftOffset();
    const char* str = mName;
    SDeskBitmap b;

    Log(7, "SDeskItemRSS::DrawMessage(%s)\n", str);

    // This is the rectangle we draw in.
    CascadeRect r = CascadeRect(x, y,
        SDesk::getInstance().getCurrentTheme().getScreenWidth() - (2 * x),
        SDesk::getInstance().getCurrentTheme().getScreenHeight() - y - 20);

    // The item is not selected.
    bitmap.DrawText(
        SDesk::getInstance().getCurrentTheme().getNormalFont(),
        r, mFilename,
        SDesk::getInstance().getCurrentTheme().getNormalColor(),
        TEXT_JUSTIFY_TOP |
        TEXT_JUSTIFY_LEFT | TEXT_WORD_WRAP);

    Log(7, "SDeskItemRSS::DrawMessage() done\n");
}

void SDeskItemRSS::DrawText(
    CascadeBitmap& bitmap,
    int x,
    int y,
    int menuType)
{
    const char* str = mName;

    Log(1, "SDeskItemRSS::DrawText(%s)\n", str);

    // This is the rectangle we draw in.
    CascadeRect r = CascadeRect(x, y,
        SDesk::getInstance().getCurrentTheme().getScreenWidth() - (2 * x),
        SDesk::getInstance().getCurrentTheme().getTextCellHeight());

    // The item is not selected.
    bitmap.DrawText(
        SDesk::getInstance().getCurrentTheme().getNormalFont(),
        r, mName,
        SDesk::getInstance().getCurrentTheme().getNormalColor(),
        TEXT_CENTER_VERTICALLY |
        TEXT_JUSTIFY_LEFT | TEXT_CLIP_WITH_DOTDOTDOT);

    Log(7, "SDeskItemRSS::DrawText() done\n");
}

bool SDeskItemRSS::OnKeyDown(u32 nKey)
{
    Log(7, "SDeskItemRSS::OnKeyDown(%d)\n", nKey);

    Log(7, "SDeskItemRSS::OnKeyDown() done\n");
    return false;
}

