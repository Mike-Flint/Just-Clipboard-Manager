#include "ClipboardManager.h"

LRESULT CALLBACK ClipboardManager::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, msg, wParam, lParam); 
}

void ClipboardManager::clipboardWatcher( HWND &hwnd) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(0);
    wc.lpszClassName = "ClipboardWatcher";
    RegisterClass(&wc);

    hwnd = CreateWindow(
        wc.lpszClassName,
        nullptr,
        0,
        0, 0, 0, 0,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    AddClipboardFormatListener(hwnd);
}

void ClipboardManager::_initPath(){
    char Path[MAX_PATH];
    GetModuleFileNameA(NULL, Path, MAX_PATH);

    char* lastSlash = strrchr(Path, '\\');
    
    if (lastSlash) {
        APP_NAME = lastSlash + 1;
        *lastSlash = '\0';
    }
    BASE_PATH = Path;
}

void ClipboardManager::checkIfRunning(){
    // Let's try to create a unique mutex
    HANDLE hMutex = CreateMutexA(NULL, TRUE, "Global\\ClipboardManager");
    
    if (hMutex == NULL) {
        throw std::runtime_error("Unable to create a mutex.");
    }

    // We check if the mutex already exists, then the program is already running
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(hMutex);  // Close the mutex
        throw std::runtime_error("Another instance of the program is already running.");
    }
}


void ClipboardManager::addAppToStartup(){
    const char* regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    std::string quotedPath = "\"" + BASE_PATH + "\\" + APP_NAME + "\"";

    HKEY hKey;
    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey);

    if (result == ERROR_SUCCESS) {
        // Check if the process name matches and if it is not the current process
        DWORD dwType;
        DWORD cbData = MAX_PATH;
        char szBuffer[255];

        result = RegQueryValueEx(hKey, "Clipboard Manager", NULL, &dwType, (LPBYTE)szBuffer, &cbData);

        if (result != ERROR_SUCCESS) {
            // If the record does not exist, we create it.
            result = RegSetValueEx(hKey, "Clipboard Manager", 0, REG_SZ, (BYTE*)quotedPath.c_str(), quotedPath.size() + 1);
            if (result != ERROR_SUCCESS) {
                throw std::runtime_error("Failed to open registry key.");
            }
        }
        else {
            // If the record exists, we check if it is correct
            std::string currentPath(szBuffer);
            if (currentPath != quotedPath) {
                // If the path is incorrect, update it
                result = RegSetValueEx(hKey, "Clipboard Manager", 0, REG_SZ, (BYTE*)quotedPath.c_str(), quotedPath.size() + 1);
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

