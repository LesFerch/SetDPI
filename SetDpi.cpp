#include "DpiHelper.h"
#include <Windows.h>
#include <cstringt.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

// Get OS recommended scale
int GetRecommendedDPIScaling()
{
    int dpi = 0;

    auto retval = SystemParametersInfo(SPI_GETLOGICALDPIOVERRIDE, 0, (LPVOID)&dpi, 1);
    if (retval != 0)
    {
        int currDPI = DpiVals[dpi * -1];
        return currDPI;
    }

    return -1;
}

//to store display info along with corresponding list item
struct DisplayData {
    LUID m_adapterId;
    int m_targetID;
    int m_sourceID;

    DisplayData()
    {
        m_adapterId = {};
        m_targetID = m_sourceID = -1;
    }
};

std::vector<DisplayData> GetDisplayData()
{
    std::vector<DisplayData> displayDataCache;
    std::vector<DISPLAYCONFIG_PATH_INFO> pathsV;
    std::vector<DISPLAYCONFIG_MODE_INFO> modesV;
    int flags = QDC_ONLY_ACTIVE_PATHS;
    if (false == DpiHelper::GetPathsAndModes(pathsV, modesV, flags))
    {
        cout << "DpiHelper::GetPathsAndModes() failed\n";
    }
    displayDataCache.resize(pathsV.size());
    int idx = 0;
    for (const auto &path : pathsV)
    {
        //get display name
        auto adapterLUID = path.targetInfo.adapterId;
        auto targetID = path.targetInfo.id;
        auto sourceID = path.sourceInfo.id;

        DISPLAYCONFIG_TARGET_DEVICE_NAME deviceName;
        deviceName.header.size = sizeof(deviceName);
        deviceName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
        deviceName.header.adapterId = adapterLUID;
        deviceName.header.id = targetID;
        if (ERROR_SUCCESS != DisplayConfigGetDeviceInfo(&deviceName.header))
        {
            cout << "DisplayConfigGetDeviceInfo() failed";
        }
        else
        {
            std::wstring nameString = std::to_wstring(idx) + std::wstring(L". ") + deviceName.monitorFriendlyDeviceName;
            if (DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL == deviceName.outputTechnology)
            {
                nameString += L"(internal display)";
            }
            DisplayData dd = {};
            dd.m_adapterId = adapterLUID;
            dd.m_sourceID = sourceID;
            dd.m_targetID = targetID;

            displayDataCache[idx] = dd;
        }
        idx++;
    }
    return displayDataCache;
}

bool DPIFound(int val)
{
    bool found = false;
    for (int i = 0; i < 12; i++)
    {
        if (val == DpiVals[i])
        {
            found = true;
            break;
        }
    }
    return found;
}

BOOL DisableWow64Redirection()
{
    BOOL bIsWow64 = FALSE;
    typedef BOOL(APIENTRY* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

    HMODULE hModule = GetModuleHandle(TEXT("kernel32"));
    if (hModule == NULL)
    {
        return FALSE;
    }

    LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
        hModule, "IsWow64Process");

    if (fnIsWow64Process != NULL)
    {
        if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
        {
            return FALSE;
        }
    }

    if (bIsWow64)
    {
        PVOID OldValue = NULL;
        return Wow64DisableWow64FsRedirection(&OldValue);
    }

    return TRUE;
}

int main(int argc, char *argv[])
{
    auto dpiToSet = 0;
    auto displayIndex = 1;

    enum
    {
        RESOLUTION_SET,
        RESOLUTION_GET,
        RESOLUTION_VALUE,
    } resolutionMode = RESOLUTION_SET;

    bool restartExplorer = false;

    if (argc <= 1)
    {
        cout << "1. argument: Scale percent, or \"get\" or \"value\" to display current\n";
        cout << "2. argument: Monitor index, leave empty to use primary monitor\n";
        cout << "3. argument: Include \"/e\" to restart explorer\n";
        return 0;
    }

    if (argc >= 2)
    {
        if (strcmp(argv[1], "get") == 0)
        {
            resolutionMode = RESOLUTION_GET;
        }
        else if (strcmp(argv[1], "value") == 0)
        {
            resolutionMode = RESOLUTION_VALUE;
        }
        else
        {
            dpiToSet = atoi(argv[1]);
        }
    }

    if (argc >= 3)
    {
        // Check if second argument is "/e" or a number
        if (strcmp(argv[2], "/e") == 0) {
            restartExplorer = true;
        }
        else {
            displayIndex = atoi(argv[2]);
        }
    }

    if (argc >= 4)
    {
        if (strcmp(argv[3], "/e") == 0) {
            restartExplorer = true;
        }
    }

    auto displayDataCache = GetDisplayData();
    if (displayIndex < 1 || displayDataCache.size() < displayIndex)
    {
        if (DPIFound(displayIndex) && 1 <= dpiToSet && dpiToSet <= displayDataCache.size())
        {
            auto t = dpiToSet;
            dpiToSet = displayIndex;
            displayIndex = t;
        }
        else
        {
            cout << "Invalid monitor number: " << displayIndex;
            return 0;
        }
    }

    displayIndex -= 1; // change from 1...X to 0...(X-1)

    auto currentResolution = DpiHelper::GetDPIScalingInfo(displayDataCache[displayIndex].m_adapterId, displayDataCache[displayIndex].m_sourceID);
    if (resolutionMode == RESOLUTION_GET)
    {
        cout << "Current scale value: " << currentResolution.current;
        return 0;
    }
    if (resolutionMode == RESOLUTION_VALUE)
    {
        cout << currentResolution.current;
        return 0;
    }
    if (!DPIFound(dpiToSet))
    {
        cout << "Invalid scale value: " << dpiToSet;
        return 0;
    }
    auto success = DpiHelper::SetDPIScaling(displayDataCache[displayIndex].m_adapterId, displayDataCache[displayIndex].m_sourceID, dpiToSet);
    if (success == false)
    {
        cout << "DpiHelper::SetDPIScaling() failed";
        return 0;
    }
    else
    {
        if (displayIndex == 0)
        {
            HKEY hKey;
            LPCWSTR sKeyPath;
            DWORD value = static_cast<DWORD>(int(dpiToSet * 0.96));
            int iResult;
            sKeyPath = L"Control Panel\\Desktop\\WindowMetrics\\";
            iResult = RegOpenKeyEx(HKEY_CURRENT_USER, sKeyPath, NULL, KEY_ALL_ACCESS, &hKey);
            iResult = RegSetValueEx(hKey, L"AppliedDPI", NULL, REG_DWORD, (const BYTE*)&value, sizeof(value));
            RegCloseKey(hKey);
        }
    }
    if (restartExplorer) {
        DisableWow64Redirection();
        system("taskkill /f /im explorer.exe");
        Sleep(100);
        system("start explorer.exe");
    }
    return 0;
}
