#include <windows.h>
#include <shellapi.h>
#include <string>
#include <iostream>

void ExtractZipWithWindows(const std::string& zipPath, const std::string& outputFolder) {
    std::string parameters = "-Command \"Expand-Archive -LiteralPath \\\"" + zipPath + "\\\" -DestinationPath \\\"" + outputFolder + "\\\" -Force\"";

    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize = sizeof(SHELLEXECUTEINFOA);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = NULL;
    sei.lpVerb = "open";
    sei.lpFile = "powershell.exe";        
    sei.lpParameters = parameters.c_str(); 
    sei.nShow = SW_HIDE;                  

    if (ShellExecuteExA(&sei)) {
        WaitForSingleObject(sei.hProcess, INFINITE);
        CloseHandle(sei.hProcess);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::string zipFile = lpCmdLine;

    if (!zipFile.empty() && zipFile.front() == '"') zipFile.erase(0, 1);
    if (!zipFile.empty() && zipFile.back() == '"') zipFile.pop_back();

    if (zipFile.empty()) {
        zipFile = "archive.zip";
    }

    std::string outputFolder = zipFile;
    size_t lastDot = outputFolder.find_last_of(".");
    if (lastDot != std::string::npos) {
        outputFolder = outputFolder.substr(0, lastDot);
    }

    ExtractZipWithWindows(zipFile, outputFolder);

    ShellExecuteA(NULL, "open", "explorer.exe", outputFolder.c_str(), NULL, SW_SHOW);

    return 0;
}
