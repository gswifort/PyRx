#pragma once
class PyRxAppSettings
{
public:
    static const std::tuple<bool, std::wstring> pyexecutable_path();
    static std::vector<std::wstring>& getCommandLineArgs();
};

