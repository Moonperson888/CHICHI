#include <windows.h>
#include <shellapi.h>
#include <string>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

void RunHiddenProcess(const std::string& executable, const std::string& parameters) {
    SHELLEXECUTEINFOA sei = {0};
    sei.cbSize = sizeof(SHELLEXECUTEINFOA);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = NULL;
    sei.lpVerb = "open";
    sei.lpFile = executable.c_str();
    sei.lpParameters = parameters.c_str();
    sei.nShow = SW_HIDE;

    if (ShellExecuteExA(&sei)) {
        WaitForSingleObject(sei.hProcess, INFINITE);
        CloseHandle(sei.hProcess);
    }
}

void ExtractZipWithWindows(const std::string& zipPath, const std::string& outputFolder) {
    std::string parameters = "-Command \"Expand-Archive -LiteralPath \\\"" + zipPath + "\\\" -DestinationPath \\\"" + outputFolder + "\\\" -Force\"";
    RunHiddenProcess("powershell.exe", parameters);
}

void ExtractWithTar(const std::string& archivePath, const std::string& outputFolder) {
    fs::create_directories(outputFolder);
    std::string parameters = "-xf \"" + archivePath + "\" -C \"" + outputFolder + "\"";
    RunHiddenProcess("tar.exe", parameters);
}

std::string ToLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}

bool EndsWith(const std::string& str, const std::string& suffix) {
    return str.size() > suffix.size() && str.substr(str.size() - suffix.size()) == suffix;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::string archiveFile = lpCmdLine;

    if (!archiveFile.empty() && archiveFile.front() == '"') archiveFile.erase(0, 1);
    if (!archiveFile.empty() && archiveFile.back() == '"') archiveFile.pop_back();

    if (archiveFile.empty()) {
        archiveFile = "archive.zip";
    }

    std::string lowerFile = ToLower(archiveFile);
    std::string outputFolder = archiveFile;

    if (EndsWith(lowerFile, ".tar.gz") || EndsWith(lowerFile, ".tar.xz")) {
        outputFolder = archiveFile.substr(0, archiveFile.size() - 7);
        ExtractWithTar(archiveFile, outputFolder);
    } else {
        size_t lastDot = outputFolder.find_last_of(".");
        if (lastDot != std::string::npos) {
            outputFolder = outputFolder.substr(0, lastDot);
        }

        if (EndsWith(lowerFile, ".tgz") ||
            EndsWith(lowerFile, ".txz") ||
            EndsWith(lowerFile, ".tar") ||
            EndsWith(lowerFile, ".7z")  ||
            EndsWith(lowerFile, ".rar")) {
            ExtractWithTar(archiveFile, outputFolder);
        } else {
            ExtractZipWithWindows(archiveFile, outputFolder);
        }
    }

    if (fs::exists(outputFolder)) {
        ShellExecuteA(NULL, "open", outputFolder.c_str(), NULL, NULL, SW_SHOWNORMAL);
    }

    return 0;
}

