#include "ClipboardManager.h"


int main(){
    HWND hwnd;

    try {
        // Class Initialization
        ClipboardManager сlipTracker;

        // Check if the program is already running
        сlipTracker.checkIfRunning();
        std::cout << "The checkIfRunning() function has completed its execution." << std::endl;

        // Adds the program to Windows startup via the registry.
        сlipTracker.addAppToStartup();
        std::cout << "The addAppToStartup() function has completed its execution." << std::endl;
        
        // Load data from a file or create a new file
        сlipTracker.loadFromFile();
        std::cout << "The loadFromFile() function has completed its execution." << std::endl;

        // Loading data from a file to the clipboard
        сlipTracker.copyToClipboard(); 
        std::cout << "The copyToClipboard() function has completed its execution." << std::endl;
    
        // Clipboard monitoring
        сlipTracker.clipboardWatcher(hwnd);

        MSG msg;
        while (GetMessage(&msg, 0, 0, 0)) {
            if (msg.message == WM_CLIPBOARDUPDATE){
                сlipTracker.check();
            }
        }

    } 
    catch (const std::exception& e) {
        // Error message
        MessageBox(NULL, e.what(), "Clipboard Manager - Error", MB_ICONERROR | MB_OK);
        RemoveClipboardFormatListener(hwnd);
    }
    return 0;
}