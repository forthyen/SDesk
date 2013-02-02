#include "SDesk.h"
#include "SDeskItemMessage.h"

SDeskItemMessage::SDeskItemMessage(
    const char* name,
    const char* filename,
    bool prot)
    : SDeskItem(name, filename, prot)
{
    Log(7, "SDeskItemMessage(%s,%s)\n", name, filename);

    // Track this items type.
    mType = I_MESSAGE;

    Log(7, "SDeskItemMessage() done\n");
}

SDeskItemMessage::~SDeskItemMessage()
{
    Log(7, "~SDeskItemMessage()\n");

    Log(7, "~SDeskItemMessage() done\n");
}

bool SDeskItemMessage::OnKeyDown(u32 nKey)
{
    Log(7, "SDeskItemMessage::OnKeyDown(%d)\n", nKey);

    Log(7, "SDeskItemMessage::OnKeyDown() done\n");
    return false;
}

bool SDeskItemMessage::HaveMessage()
{
    if (mMessages.size() == 0) {
        return false;
    } else {
        return true;
    }
}

void SDeskItemMessage::CleanMessages()
{
    for (unsigned int i = 0; i < mMessages.size(); i++) {
        delete mMessages[i];
    }
    mMessages.clear();
}

void SDeskItemMessage::AddMessage(
    string* value)
{
    Log(8, "SDeskItemMessage::AddMessage()\n");

    mMessages.insert(mMessages.end(), value);

    Log(8, "SDeskItemMessage::AddMessage() done\n");
}

void SDeskItemMessage::DrawMessage(
    CascadeBitmap& bitmap)
{
    Log(8, "SDeskItemMessage::DrawMessage()\n");

    vector<string *>::const_iterator it, end = mMessages.end();
    int y = SDesk::getInstance().getCurrentTheme().getTextTopOffset();
    for(it = mMessages.begin(); it != end; it++) {
        CascadeRect r = CascadeRect(20, y,
            SDesk::getInstance().getCurrentTheme().getScreenWidth() - 40,
            SDesk::getInstance().getCurrentTheme().getTextCellHeight());
        
        bitmap.DrawText(
            SDesk::getInstance().getCurrentTheme().getNormalFont(),
            r, (*it)->c_str(),
            SDesk::getInstance().getCurrentTheme().getNormalColor(),
            TEXT_CENTER_VERTICALLY |
            TEXT_JUSTIFY_LEFT | TEXT_CLIP_WITH_DOTDOTDOT);
        y += SDesk::getInstance().getCurrentTheme().getTextCellHeight();
    }

    Log(8, "SDeskItemMessage::DrawMessage()\n");
}

