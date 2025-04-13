#include "ClipboardManager.h"

void ClipboardManager::loadFromFile(){
    std::ifstream inFile( BASE_PATH + "\\" + BUFFER_NAME, std::ios::binary);
    if (inFile){
        for (ClipboardObject& obj : m_ClipboardHistory) {
            inFile.read(reinterpret_cast<char*>(&obj.type), sizeof(obj.type));

            size_t textSize;
            inFile.read(reinterpret_cast<char*>(&textSize), sizeof(textSize));
            obj.text.resize(textSize);
            inFile.read(reinterpret_cast<char*>(obj.text.data()), textSize * sizeof(wchar_t));

            if (obj.type != ClipboardType::None)
                ++m_NumberObjects;
        }
    }
    else{
        std::ofstream outFile( BASE_PATH + "\\" + BUFFER_NAME, std::ios::binary);
        if(outFile){
            m_ClipboardHistory[ARRAY_SIZE - 1] = ClipboardObject(L"Hello, World!");
            ++m_NumberObjects;

            // Save data to file
            for (const ClipboardObject& obj : m_ClipboardHistory) {
                outFile.write(reinterpret_cast<const char*>(&obj.type), sizeof(obj.type));
                
                size_t textSize = obj.text.size();
                outFile.write(reinterpret_cast<const char*>(&textSize), sizeof(textSize));
                outFile.write(reinterpret_cast<const char*>(obj.text.data()), textSize * sizeof(wchar_t));
            }
        }
        else{
            throw std::runtime_error("Failed to create a file to record data.");
        }
        outFile.close();
    }
    inFile.close();
}


void ClipboardManager::copyToClipboard() {
    int counter = 0;
    for (auto it = m_ClipboardHistory.rbegin(); it != m_ClipboardHistory.rend(); ++it) {
        const ClipboardObject& obj = *it;
        if (obj.type != ClipboardType::None) {
            if (obj.type == ClipboardType::Text) {
                _myOpenClipboard();
                EmptyClipboard();

                HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (obj.text.size() + 1) * sizeof(wchar_t));
                if (hMem) {
                    wchar_t* pMem = static_cast<wchar_t*>(GlobalLock(hMem));
                    memcpy(pMem, obj.text.c_str(), obj.text.size() * sizeof(wchar_t));
                    pMem[obj.text.size()] = 0;
                    GlobalUnlock(hMem);
                    SetClipboardData(CF_UNICODETEXT, hMem);
                    std::cerr << counter + 1 << " - Text successfully returned to clipboard. (CF_TEXT)" << std::endl;
                }
                CloseClipboard();
            }
            
            if (counter < 3) {
                Sleep(2000);
            } else {
                Sleep(SPEED_COPY);
            }
            counter++;
        }
    }

    _myOpenClipboard();
    CloseClipboard();
}