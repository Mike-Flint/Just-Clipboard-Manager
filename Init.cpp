#include "../ClipboardManager/ClipMonitor.h"


void ClipMonitor::checkIfRunning(){
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Unable to create a snapshot of running processes.");
    }

    DWORD currentProcessId = GetCurrentProcessId(); 
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hProcessSnap, &pe32)) {
        do {
            // Check if the process name matches and if it is not the current process
            if (std::string(pe32.szExeFile) == appName && pe32.th32ProcessID != currentProcessId) {
                CloseHandle(hProcessSnap);
                throw std::runtime_error("Another instance of the program is already running.");
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }
    CloseHandle(hProcessSnap);
}

void ClipMonitor::addAppToStartup(){
    const char* regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    std::string quotedPath = "\"" + appPath + "\"";

    HKEY hKey;
    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey);

    if (result == ERROR_SUCCESS) {
        // Check if the process name matches and if it is not the current process
        DWORD dwType;
        DWORD cbData = MAX_PATH;
        char szBuffer[255];

        result = RegQueryValueEx(hKey, appName.c_str(), NULL, &dwType, (LPBYTE)szBuffer, &cbData);

        if (result != ERROR_SUCCESS) {
            // If the record does not exist, we create it.
            result = RegSetValueEx(hKey, appName.c_str(), 0, REG_SZ, (BYTE*)quotedPath.c_str(), quotedPath.size() + 1);
            if (result != ERROR_SUCCESS) {
                throw std::runtime_error("Failed to open registry key.");
            }
        }
        else {
            // If the record exists, we check if it is correct
            std::string currentPath(szBuffer);
            if (currentPath != quotedPath) {
                // If the path is incorrect, update it
                result = RegSetValueEx(hKey, appName.c_str(), 0, REG_SZ, (BYTE*)quotedPath.c_str(), quotedPath.size() + 1);
                if (result != ERROR_SUCCESS) {
                    throw std::runtime_error("Failed to update the registry entry for application startup.");
                }
            }
        }
        RegCloseKey(hKey);
    } else {
        throw std::runtime_error("Failed to open registry key.");
    }
}

void ClipMonitor::initPath(){
    char Path[MAX_PATH];
    GetModuleFileNameA(NULL, Path, MAX_PATH);
    
    appPath = Path;
    char* lastSlash = strrchr(Path, '\\');
    if (lastSlash) {
        appName = lastSlash + 1;
        *lastSlash = '\0';
    }
    
    bufferPath = std::string(Path) + "\\" + nameBuffer;
}