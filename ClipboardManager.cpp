#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define ClipboardCheck 500
#define CopySpeed 700
#define NumberObjects 15
#define NameBuffer "data.bin"

typedef std::vector<std::string> vecStr;

bool StartStop = true;
std::string messageErrore = " ";

void loadFromFile(vecStr& vectorText){
    std::ifstream inFile(NameBuffer, std::ios::binary);
    if (inFile){
        std::size_t size;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
        vectorText.resize(size);
        for (std::string& str: vectorText){
            std::size_t lenght;
            inFile.read(reinterpret_cast<char*>(&lenght), sizeof(lenght));
            str.resize(lenght);
            inFile.read(&str[0], lenght);
        }
        inFile.close();
    }else{
        std::ofstream outFile(NameBuffer, std::ios::binary);
        if(outFile){
            vectorText = {"Hello, World!",};
        }else{
            messageErrore = "Failed to create a file to record data.";
            StartStop = false;
        }
    }  
}

void CopyToClipboard(std::string& text){
    if (OpenClipboard(nullptr)){
        EmptyClipboard();
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if(hMem){
            memcpy(GlobalLock(hMem), text.c_str(), text.size() + 1);
            GlobalUnlock(hMem);
            SetClipboardData(CF_TEXT, hMem);
        }
    }
    CloseClipboard();
}

void saveToFile(vecStr& dataCollection){
   std::ofstream outFile(NameBuffer, std::ios::binary);
   if(outFile){
        size_t size = dataCollection.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
        for (const std::string& str : dataCollection){
            size_t length = str.size();
            outFile.write(reinterpret_cast<const char*>(&length), sizeof(length));
            outFile.write(str.c_str(), length);
        }
        outFile.close();
   }else{
        messageErrore = "Failed to save data to file.";
        StartStop = false;
   }
}

void SaveClipboardText(std::string& text , bool& checkError ,
    HANDLE& hData, char*& pszText,vecStr& clipboardHistory){
    checkError = true;
    if(OpenClipboard(nullptr)){
        hData = GetClipboardData(CF_TEXT);
        if(hData != nullptr){
            pszText = static_cast<char*>(GlobalLock(hData));
            if(pszText != nullptr){
                text = pszText;
                GlobalUnlock(hData);
                checkError = false;
            }
        }
        CloseClipboard();
    }
    if(checkError){ 
        return;
    }
    
    if (!text.empty() && (clipboardHistory.empty() || clipboardHistory.front() != text) && 
    std::find(clipboardHistory.begin(), clipboardHistory.end(), text) == clipboardHistory.end()){
        if (clipboardHistory.size() >= NumberObjects){
            clipboardHistory.back() = text;
            std::rotate(clipboardHistory.rbegin(), clipboardHistory.rbegin() + 1, clipboardHistory.rend());
            saveToFile(clipboardHistory);
        }else{
            clipboardHistory.insert(clipboardHistory.begin(), text);
            saveToFile(clipboardHistory);
        }
    }
}

int main(){
    vecStr clipboardHistory;
    loadFromFile(clipboardHistory);
    
    for(vecStr::reverse_iterator it = clipboardHistory.rbegin(); 
    it != clipboardHistory.rend();
    ++it){
        CopyToClipboard(*it);
        Sleep(CopySpeed);
    }

    std::string ClipboardText;
    bool checkError = true;
    HANDLE hData;
    char* pszText;

    while(StartStop){
        SaveClipboardText(ClipboardText, checkError, hData , pszText, clipboardHistory);
        Sleep(ClipboardCheck);
    }

    MessageBox(NULL, messageErrore.c_str(), "The program has stopped working", MB_ICONERROR | MB_OK);
    return 0;
}
