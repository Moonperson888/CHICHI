# CHICHI
Lightweight, zero-dependency file extractor for Windows written in C++.

### Supported Formats
*(Non-password-protected archives)*
* `.zip`
* `.tar.gz` / `.tgz`
* `.tar.xz` / `.txz`
* `.tar`
* `.7z`
* `.rar`

### Features
* Lightweight extraction
* Names the extracted folder after the archive
* Automatically opens the extracted folder in File Explorer once finished extracting

### How to Use
1. Press `Win + R`, type `shell:sendto`, and press **Enter**.
2. Hold **Right-Click** on `CHICHI.exe`, drag it into the `SendTo` folder, and click **Create shortcuts here** (you can rename the shortcut and change its icon to whatever you like).
3. Right-click any supported archive file, click **Show more options** (on Windows 11) → **Send to** → select your **CHICHI** shortcut.

### Build from Source
```bash
g++ unzip.cpp -o CHICHI.exe -mwindows -std=c++17 -static
```
