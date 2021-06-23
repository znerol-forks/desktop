#include "utils.h"

#include <shlobj_core.h>

std::string currentUserHomeDir()
{
    char buf[MAX_PATH + 1] = { 0 };
    SHGetSpecialFolderPathA(NULL, buf, CSIDL_PROFILE, false);

    return std::string(buf);
}

std::string logsFileName()
{
    const std::string homeDir = currentUserHomeDir();

    return homeDir + std::string("\\AppData\\Roaming\\Nextcloud\\logs\\ncoverlays.txt");
}