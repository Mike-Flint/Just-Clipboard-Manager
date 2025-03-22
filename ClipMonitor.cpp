#include "../ClipboardManager/ClipMonitor.h"

void ClipMonitor::lastClipboardText(std::wstring& clipText) {
    // Open the clipboard
    if (!OpenClipboard(nullptr)) {
        clipText = L"";
        return;
    }

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData == nullptr) {
        CloseClipboard();
        clipText = L"";
        return;
    }

    // Getting text from the clipboard
    wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        clipText = L"";
        return;
    }
    
    clipText = std::wstring(pszText);

    GlobalUnlock(hData);
    CloseClipboard();
}


void ClipMonitor::check(){
    std::wstring clipText;
    lastClipboardText(clipText);

    // Checking text from the clipboard
    if (!clipText.empty() && clipText != L"") {
        if (clipboardHistory.front() != clipText) {
            if (std::find(clipboardHistory.begin(), clipboardHistory.end(), clipText) == clipboardHistory.end()) {
                if (clipboardHistory.size() == numberObjects){
                    clipboardHistory.back() = clipText;
                    std::rotate(clipboardHistory.rbegin(), clipboardHistory.rbegin() + 1, clipboardHistory.rend());
                    // Saving a vector to a file
                    saveToFile(clipboardHistory);
                }
                else if(clipboardHistory.size() < numberObjects) {
                    clipboardHistory.insert(clipboardHistory.begin(), clipText);
                    // Saving a vector to a file
                    saveToFile(clipboardHistory);
                }
                else{
                    clipboardHistory.resize(numberObjects);
                    saveToFile(clipboardHistory);
                }
            }
        }
    }
}

void ClipMonitor::saveToFile(const vecStr& dataCollection){
    std::ofstream outFile(bufferPath, std::ios::binary | std::ios::trunc);
    if(outFile){
            size_t size = clipboardHistory.size();
            outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
            for (const std::wstring& str : clipboardHistory){
                size_t length = str.size();
                outFile.write(reinterpret_cast<const char*>(&length), sizeof(length));
                outFile.write(reinterpret_cast<const char*>(str.c_str()), length * sizeof(wchar_t));
            }
            outFile.close();
    }else{
            throw std::runtime_error("Failed to save data to file.");
    }
}

void ClipMonitor::loadFromFile(){
    std::ifstream inFile(bufferPath, std::ios::binary);
    if (inFile){
        std::size_t size;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        clipboardHistory.resize(size);

        for (std::wstring& str: clipboardHistory){
            std::size_t length;
            inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
            str.resize(length);
            inFile.read(reinterpret_cast<char*>(&str[0]), length * sizeof(wchar_t));
        }
        inFile.close();
    }else{
        std::ofstream outFile(bufferPath, std::ios::binary);
        if(outFile){
            clipboardHistory = {L"Hello, World!"};
            outFile.close();
        }else{
            throw std::runtime_error("Failed to create a file to record data.");
        }
    }
}

void ClipMonitor::copyToClipboard() {
    int counter = 0;
    for (auto it = clipboardHistory.rbegin(); it != clipboardHistory.rend(); ++it) {
        const std::wstring& text = *it;
        if (OpenClipboard(NULL)) {
            
            EmptyClipboard();

            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,(text.size() + 1) * sizeof(wchar_t));
            if (hMem) {
                memcpy(GlobalLock(hMem), text.c_str(), text.size() * sizeof(wchar_t));
                GlobalUnlock(hMem);
                SetClipboardData(CF_UNICODETEXT, hMem); 
                CloseClipboard();
                GlobalFree(hMem);
            }
        }

        if (counter < 3) {
            Sleep(4000);
            counter++;
        } else {
            Sleep(copySpeed); 
        }
    }
}