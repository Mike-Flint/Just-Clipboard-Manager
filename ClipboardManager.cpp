#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#define ClipboardCheck 500
#define CopySpeed 700
#define NumberObjects 15


void EnsureInStartup() {
    const char* regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const char* appName = "ClipboardManager";
    char filePath[MAX_PATH];

    // Continuing the path to the executable file
    GetModuleFileNameA(NULL, filePath, MAX_PATH);

    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
        char buffer[MAX_PATH];
        DWORD bufferSize = sizeof(buffer);

        // Checking if the entry already exists
        if (RegQueryValueExA(hKey, appName, NULL, NULL, (BYTE*)buffer, &bufferSize) != ERROR_SUCCESS ||
            strcmp(buffer, filePath) != 0) {
            // Add or update a record
            RegSetValueExA(hKey, appName, 0, REG_SZ, (BYTE*)filePath, strlen(filePath) + 1);
            std::cout << "Added to startup." << std::endl;
        } else {
            std::cout << "The program is already in autoload." << std::endl;
        }
        RegCloseKey(hKey);
    } else {
        std::cerr << "Registry access error!" << std::endl;
    }
}

void saveToFile(const std::vector<std::string>& dataCollection, const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (outFile) {
        size_t size = dataCollection.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        for (const auto& str : dataCollection) {
            size_t length = str.size();
            outFile.write(reinterpret_cast<const char*>(&length), sizeof(length));  
            outFile.write(str.c_str(), length);
        }
        outFile.close();
    } else {
        std::cerr << "Could not open file for writing." << std::endl;
    }
}

std::vector<std::string> loadFromFile(const std::string& filename) {
    std::vector<std::string> dataCollection;

    std::ifstream inFile(filename, std::ios::binary);

    if (inFile) {
        size_t size;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        dataCollection.resize(size);
        for (auto& str : dataCollection) {
            size_t length;
            inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
            str.resize(length);
            inFile.read(&str[0], length);
        }
        inFile.close();
        std::cout << "Data successfully loaded from file:" << filename << std::endl;
    } else {
        // Creating a new file
        std::ofstream outFile(filename, std::ios::binary);
        if (outFile) {
            std::cout << "File created successfully: " << filename << std::endl;
            return dataCollection = {"Hello, World!",};
        } else {
            std::cerr << "Error creating file!" << std::endl;
        }
        std::cerr << "The file could not be opened for reading." << std::endl;
        
    }
    return dataCollection;
}

std::string GetClipboardText() {
    if (!OpenClipboard(nullptr)) {
        return "ERROR: Failed to open clipboard.";
    }

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr) {
        CloseClipboard();
        return "ERROR: Failed to get clipboard data.";
    }

    char* pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        return "ERROR: Failed to lock clipboard text.";
    }

    std::string text(pszText);
    GlobalUnlock(hData);
    CloseClipboard();
    return text;
}

void CopyToClipboard(const std::string& text){
   if (!OpenClipboard(nullptr)) {
        std::cerr << "Failed to open clipboard." << std::endl;
        return;
    }

    EmptyClipboard();

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hMem) {
        std::cerr << "Failed to allocate memory." << std::endl;
        CloseClipboard();
        return;
    }

    memcpy(GlobalLock(hMem), text.c_str(), text.size() + 1);
    GlobalUnlock(hMem);

    SetClipboardData(CF_TEXT, hMem);

    CloseClipboard();
}


std::vector<std::string> clipboardHistory = loadFromFile("data.bin");

void SaveClipboardText() {
    std::string text = GetClipboardText();
    if (!text.empty() && (clipboardHistory.empty() || clipboardHistory.front() != text)) {
        if (std::find(clipboardHistory.begin(), clipboardHistory.end(), text) == clipboardHistory.end()){
            if(clipboardHistory.size() >= NumberObjects ){
                clipboardHistory.back() = text;
                std::rotate(clipboardHistory.rbegin(), clipboardHistory.rbegin() + 1, clipboardHistory.rend());
                std::cout << "Text saved to history and delete last: " << text << std::endl;
                saveToFile(clipboardHistory, "data.bin");
            }
            else{
                clipboardHistory.insert(clipboardHistory.begin(), text);
                std::cout << "Text saved to history: " << text << std::endl;
                saveToFile(clipboardHistory, "data.bin");
            } 
            std::cout << "Number: " <<  clipboardHistory.size() << " ---^"<< std::endl;
        }
    }
}

int main() {
    EnsureInStartup();
    std::cout << "Monitoring clipboard changes... Press Ctrl+C to copy text.\n";
    for (std::vector<std::string>::reverse_iterator it = clipboardHistory.rbegin();
        it != clipboardHistory.rend();
        ++it) {
        CopyToClipboard(*it);
        Sleep(CopySpeed);
    }

    while (true) {
        SaveClipboardText(); 
        Sleep(ClipboardCheck);
    }

    return 0;
}
