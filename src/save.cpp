#include "ClipboardManager.h"

void ClipboardManager::_myOpenClipboard(){
    while(true){
        if(OpenClipboard(nullptr)) {
            break;
        }
        DWORD err = GetLastError();
        std::cerr << "OpenClipboard failed, error code: " << err << ". Waiting " << WAIT_ERROR <<" milliseconds"<< std::endl;
        Sleep(WAIT_ERROR);
    }
}

ClipboardObject ClipboardManager::_getLastClipboardText() {
    // Open the clipboard
    _myOpenClipboard();

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if(!hData) {
        CloseClipboard();
        std::cerr << "There is no Unicode text in the clipboard." << std::endl;
        return ClipboardObject();
    }

    // Getting text from the clipboard
    wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        std::cerr << "Unable to access text data." << std::endl;
        return ClipboardObject();
    }

    std::wstring clipText = std::wstring(pszText);

    for(int i = 0; i < ARRAY_SIZE; i++){
        if (m_ClipboardHistory[i].text == clipText){
            GlobalUnlock(hData);
            CloseClipboard();

            std::cerr << "False call, such object already exists in the clipboard" << std::endl;

            return ClipboardObject();
        }
    }

    GlobalUnlock(hData);
    CloseClipboard();
    
    return ClipboardObject(clipText);
}

void ClipboardManager::_saveToFile() {
    std::ofstream outFile( BASE_PATH + "\\" + BUFFER_NAME, std::ios::binary | std::ios::trunc);
    if (outFile) {
        for (const ClipboardObject& obj : m_ClipboardHistory) {
            outFile.write(reinterpret_cast<const char*>(&obj.type), sizeof(obj.type));
            
            size_t textSize = obj.text.size();
            outFile.write(reinterpret_cast<const char*>(&textSize), sizeof(textSize));
            outFile.write(reinterpret_cast<const char*>(obj.text.data()), textSize * sizeof(wchar_t));
        }
    }
    else {
        throw std::runtime_error("Failed to save data to file. " + '\n' + (BASE_PATH + "\\" + BUFFER_NAME));
    }
}

void ClipboardManager::check(){
    if (IsClipboardFormatAvailable(CF_UNICODETEXT)) {
        ClipboardObject obj = _getLastClipboardText();
        if(obj.type == ClipboardType::None) return;
        if (!obj.text.empty() && obj.text != L"") {
            std::cerr << "New text in the clipboard (CF_TEXT)"  << std::endl;
            if (ARRAY_SIZE == m_NumberObjects){
                m_ClipboardHistory[m_NumberObjects - 1] = obj;
                std::rotate(m_ClipboardHistory.rbegin(), m_ClipboardHistory.rbegin() + 1, m_ClipboardHistory.rend());
                _saveToFile();
            }
            else if (ARRAY_SIZE > m_NumberObjects){
                m_ClipboardHistory[(ARRAY_SIZE - 1) - m_NumberObjects] = obj;
                m_NumberObjects++;
                _saveToFile();
            }
        }
    }
    else {
        std::cerr << "The new data in the clipboard is not text" << std::endl;
    }
    std::cerr << std::endl;

}



