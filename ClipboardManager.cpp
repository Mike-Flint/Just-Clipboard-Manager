#include "../ClipboardManager/ClipMonitor.h"


int main(){
    try {

        // Settings
        const unsigned short ClipboardCheck = 500;
        const unsigned short CopySpeed = 500;
        const unsigned short NumberObjects = 20;
        const char NameBuffer[] = "data.bin";

        // Class Initialization
        ClipMonitor сlipTracker( CopySpeed, NumberObjects, NameBuffer);

        // Check if the program is already running
        сlipTracker.checkIfRunning();
        std::cout << "The checkIfRunning() function worked." << std::endl;

        // Adds the program to Windows startup via the registry.
        сlipTracker.addAppToStartup();
        std::cout << "The addAppToStartup() function worked." << std::endl;
        
        // Load data from a file or create a new file
        сlipTracker.loadFromFile();
        std::cout << "The loadFromFile() function worked." << std::endl;

        // Loading data from a file to the clipboard
        сlipTracker.copyToClipboard(); 
        std::cout << "The copyToClipboard() function worked." << std::endl;
    
        // Clipboard monitoring
        while(true){
            сlipTracker.check();
            Sleep(ClipboardCheck);
        }

    } 
    catch (const std::exception& e) {
        // Error message
        MessageBox(NULL, e.what(), "Clipboard Manager - Error", MB_ICONERROR | MB_OK);
    }
    return 0;
}