#include <iostream>
#include <cstdlib> 
#include <string>
#include <windows.h>
#include <shellapi.h>

void ExtractZipWithWindows(const std::string& zipPath, const std::string& outputFolder) {
    std::cout << "[+] ChiChi is telling Windows to extract " << zipPath << "...\n";

    std::string command = "powershell -Command \"Expand-Archive -Path '" + zipPath + "' -DestinationPath '" + outputFolder + "' -Force\"";

    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "[+] Successfully extracted to: " << outputFolder << "\n";
    } else {
        std::cerr << "[-] Error: Windows failed to extract the ZIP file.\n";
        std::cout << "\nPress Enter to close..." << std::endl;
        std::cin.get();
    }
}

int main(int argc, char* argv[]) {
    std::string zipFile;

    if (argc > 1) {
        zipFile = argv[1];
    } else {
        std::cout << "[-] No file dropped. Looking for default 'archive.zip'...\n";
        zipFile = "archive.zip";
    }

    std::string outputFolder = zipFile;
    size_t lastDot = outputFolder.find_last_of(".");
    if (lastDot != std::string::npos) {
        outputFolder = outputFolder.substr(0, lastDot);
    }

    ExtractZipWithWindows(zipFile, outputFolder);

    ShellExecuteA(NULL, "open", "explorer.exe", outputFolder.c_str(), NULL, SW_SHOW);

    std::cout << "\nClosing window in 1 second...\n";
    std::string sleepCmd = "powershell -Command \"Start-Sleep -Seconds 1\"";
    std::system(sleepCmd.c_str());
    
    return 0;
}
