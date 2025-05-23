#pragma once
class PyRxAppSettings
{
public:
    static int optimizationLevel();
    static const std::tuple<bool, std::wstring> getOrCreateConfigPath();
    static const std::tuple<bool, std::wstring> pyexecutable_path();
    static std::vector<std::wstring>& getCommandLineArgs();
};

