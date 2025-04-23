#include "stdafx.h"
#include "PyRxAppSettings.h"
#include "PyRxApp.h"

const std::tuple<bool, std::wstring> PyRxAppSettings::pyexecutable_path()
{
    std::error_code ec;
    {
        std::wstring exepath(MAX_PATH, 0);
        if (acedGetEnv(_T("PYRX_PYEXE_PATH"), exepath.data(), exepath.size()) == RTNORM)
        {
            if (std::filesystem::exists(exepath.c_str(), ec))
                return std::make_tuple(true, exepath.c_str());
        }
    }
    return std::make_tuple(false, L"");
}

std::vector<std::wstring>& PyRxAppSettings::getCommandLineArgs()
{
    static std::vector<std::wstring> pyrxArgs;
    if (pyrxArgs.size() == 0)
    {
        int nArgs = 0;
        LPWSTR* szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
        if (szArglist != nullptr)
        {
            for (int i = 0; i < nArgs; i++)
                pyrxArgs.emplace_back(std::wstring{ szArglist[i] });
            LocalFree(szArglist);
        }
    }
    return pyrxArgs;
}