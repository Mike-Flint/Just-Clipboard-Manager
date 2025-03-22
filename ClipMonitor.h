#ifndef CLIP_MONITOR_CLASS_H
#define CLIP_MONITOR_CLASS_H

#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tlhelp32.h>

typedef std::vector<std::wstring> vecStr;

class ClipMonitor{
private: 
    std::string bufferPath; // ClipboardManager.exe
    std::string appPath; 
    std::string appName;

    const char* nameBuffer; // data.bin 
    
    const unsigned short& numberObjects; // 15-20
    const unsigned short& copySpeed;  // 500

    void initPath();
    void lastClipboardText(std::wstring& clipText);
    void saveToFile(const vecStr& dataCollection);

public:
    vecStr clipboardHistory;

    ClipMonitor( const unsigned short& copySpeed, const unsigned short& numberObjects, const char* nameBuffer):
                    numberObjects(numberObjects),
                    copySpeed(copySpeed),
                    nameBuffer(nameBuffer){
                        initPath();
                    };

    void checkIfRunning();
    void addAppToStartup();

    void loadFromFile();
    void copyToClipboard();

    void check();
};

#endif