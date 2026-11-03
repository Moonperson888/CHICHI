#include <iostream>
#include <cstdlib> 
#include <string>

void ExtractZipWithWindows(const std::string& zipPath, const std::string& outputFolder) {
    std::cout << "[+] ChiChi is telling Windows to extract " << zipPath << "...\n";

    std::string command = "powershell -Command \"Expand-Archive -Path '" + zipPath + "' -DestinationPath '" + outputFolder + "' -Force\"";

    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "[+] Successfully extracted to: " << outputFolder << "\n";
    } else {
        std::cerr << "[-] Error: Windows failed to extract the ZIP file.\n";
    }
}

int main(int argc, char* argv[]) {
    std::string zipFile;

    if (argc > 1) {
    } else {
        std::cout << "[-] No file dropped. Looking for default 'archive.zip'...\n";
        zipFile = "archive.zip";
    }

    std::string outputFolder = zipFile + "_extracted";

    ExtractZipWithWindows(zipFile, outputFolder);

    std::cout << "\nClosing window in 4 seconds...\n";
    std::string sleepCmd = "powershell -Command \"Start-Sleep -Seconds 4\"";
    std::system(sleepCmd.c_str());

    return 0;
}

