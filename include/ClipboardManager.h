#ifndef CLIPBORD_MANAGER_CLASS_H
#define CLIPBORD_MANAGER_CLASS_H

#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tlhelp32.h>
#include <array>

#include <ClipboardObject.h>

class ClipboardManager{
private: 
    
    std::string BASE_PATH; 
    std::string APP_NAME;

    // Settings
    const char* BUFFER_NAME = "data.bin"; 
    const unsigned short SPEED_COPY = 500;  
    const unsigned short WAIT_ERROR = 200;
    static const unsigned short ARRAY_SIZE = 25;

    unsigned short m_NumberObjects = 0;
    std::array<ClipboardObject, ARRAY_SIZE> m_ClipboardHistory;

    void _initPath();     
    void _saveToFile();
    void _myOpenClipboard();    
    ClipboardObject _getLastClipboardText();
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:

    ClipboardManager(){ _initPath(); }

    void checkIfRunning();         

    void addAppToStartup();        

    void loadFromFile();          

    void copyToClipboard();  
    
    void clipboardWatcher(HWND &hwnd);  
    
    void check();
};


#endif