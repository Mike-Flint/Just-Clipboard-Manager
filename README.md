# Just Clipboard Manager

A lightweight clipboard manager for Windows that saves your text copy history to a binary file and automatically restores it after system reboot.

---

## 🚀 Key Features

1. **Automatic Data Saving**  
   All copied text elements are saved to `data.bin`.
2. **Autostart on System Boot**  
   Adds itself to Windows startup via registry.
3. **History Restoration**  
   Loads saved data from the `data.bin` file and places it in the clipboard.
4. **Real-Time Monitoring**  
   Checks for clipboard changes every 500 ms.

---

## ⚠️ Frequently Asked Questions

### How to disable autostart?

#### Method 1: Through **Task Manager**

1. Press `Ctrl + Shift + Esc` to open Task Manager.
2. Go to the **"Startup"** tab.
3. Find `ClipboardManager` in the list → right-click → **"Disable"**.

#### Method 2: Through **Registry Editor**

1. Press `Win + R` → type `regedit` → OK.
2. Navigate to the following path:
   `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`
3. Delete the `ClipboardManager` entry.

---

## 📥 Download & Installation

### Method 1: Prebuilt Binary (For Users)

1. **Download the latest release** from [GitHub Releases](https://github.com/Mike-Flint/Just-Clipboard-Manager/releases).
2. **Unzip** the file to any folder.
3. **Run** `ClipboardManager.exe`.

### Method 2: Build from Source (For Developers)

```bash
git clone https://github.com/Mike-Flint/Just-Clipboard-Manager.git
cd Just-Clipboard-Manager

cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++
cmake --build build
```