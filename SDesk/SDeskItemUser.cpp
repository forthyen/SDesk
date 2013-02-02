#include "SDesk.h"
#include "SDeskItemUser.h"

SDeskItemUser::SDeskItemUser(
    const char* name,
    const char* filename,
    const char* user,
    const char* password)
    : SDeskItem(name, filename, false)
{
    Log(7, "SDeskItemUser(%s,%s,%s)\n", name, filename, password);

    // Track this items type.
    mType = I_USER;

    // Copy the user name
    strcpy(mUserName, user);

    // Copy the password
    strcpy(mPassword, password);

    Log(7, "SDeskItemUser() done\n");
}

SDeskItemUser::~SDeskItemUser()
{
    Log(7, "~SDeskItemUser()\n");

    Log(7, "~SDeskItemUser() done\n");
}

bool SDeskItemUser::OnKeyDown(u32 nKey)
{
    const char* str = mFilename;

    Log(7, "SDeskItemUser::OnKeyDown(%d)\n", nKey);

    if (nKey == CK_PLAY || nKey == CK_PAUSE || nKey == CK_PLAYPAUSE ||
        nKey == CK_SELECT || nKey == CK_EAST) {
        Log(7, "SDeskItemUser::OnKeyDown(%s) done true\n", str);

        if (mPassword[0] == '\0') {
            Log(7, "SDeskItemUser::OnKeyDown() no password\n");

            // Set the new configuraton file
            SDesk::getInstance().setConfigFile(str);

            // Enable child protection
            SDesk::getInstance().setProtected(true);

            // Set the user name
            SDesk::getInstance().getCurrentTheme().setUserName(mUserName);

            // Note that we will have to reread the configuration.
            SDesk::getInstance().setSetupApp(true);

            // This is an ok place to reset the setup
            SDesk::getInstance().SetupApp();
        } else {
            // Ask for the password
            Log(7, "SDeskItemUser::OnKeyDown() ask password\n");

            SDesk::getInstance().getCurrentTheme().ShowPopup(
                "Password (SELECT=done):");

            SDesk::getInstance().setAskPassword(mPassword, mUserName, str);
        }

        return true;
    }

    Log(7, "SDeskItemUser::OnKeyDown() done\n");
    return false;
}
