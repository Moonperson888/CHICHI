#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

struct FileMetadata {
    char fileName[260]; 
    uint32_t fileSize;
};

struct ArchiveHeader {
    uint32_t totalFiles;
};

void PackArchive(const std::string& archiveName, const std::vector<std::string>& filesToPack) {
    std::ofstream archive(archiveName, std::ios::binary);
    if (!archive) {
        std::cerr << "[-] Error: Could not create archive file.\n";
        return;
    }

    ArchiveHeader archHeader;
    archHeader.totalFiles = static_cast<uint32_t>(filesToPack.size());
    archive.write(reinterpret_cast<char*>(&archHeader), sizeof(ArchiveHeader));

    std::cout << "[+] ChiChi is packing " << archHeader.totalFiles << " files into " << archiveName << "...\n";

    for (const auto& filePath : filesToPack) {
        std::ifstream inFile(filePath, std::ios::binary | std::ios::ate);
        if (!inFile) {
            std::cerr << "[-] Warning: Skipping file, could not open: " << filePath << "\n";
            continue;
        }

        uint32_t size = static_cast<uint32_t>(inFile.tellg());
        inFile.seekg(0, std::ios::beg);

        FileMetadata meta;
        std::memset(meta.fileName, 0, sizeof(meta.fileName));
        std::strncpy(meta.fileName, filePath.c_str(), sizeof(meta.fileName) - 1);
        meta.fileSize = size;

        archive.write(reinterpret_cast<char*>(&meta), sizeof(FileMetadata));

        std::vector<char> buffer(size);
        inFile.read(buffer.data(), size);
        archive.write(buffer.data(), size);

        std::cout << " -> Packed: " << filePath << " (" << size << " bytes)\n";
    }

    std::cout << "[+] Archive creation complete!\n\n";
}

void UnpackArchive(const std::string& archiveName) {
    std::ifstream archive(archiveName, std::ios::binary);
    if (!archive) {
        std::cerr << "[-] Error: Could not open archive: " << archiveName << "\n";
        return;
    }

    ArchiveHeader archHeader;
    archive.read(reinterpret_cast<char*>(&archHeader), sizeof(ArchiveHeader));

    std::cout << "[+] ChiChi is unpacking " << archHeader.totalFiles << " files from " << archiveName << "...\n";

    for (uint32_t i = 0; i < archHeader.totalFiles; ++i) {
        FileMetadata meta;
        archive.read(reinterpret_cast<char*>(&meta), sizeof(FileMetadata));

        if (archive.gcount() < sizeof(FileMetadata)) break;

        std::string outPath = "extracted_" + std::string(meta.fileName);
        std::ofstream outFile(outPath, std::ios::binary);
        if (!outFile) {
            std::cerr << "[-] Error: Could not create output file: " << outPath << "\n";
            archive.seekg(meta.fileSize, std::ios::cur);
            continue;
        }

        std::vector<char> buffer(meta.fileSize);
        archive.read(buffer.data(), meta.fileSize);
        outFile.write(buffer.data(), meta.fileSize);

        std::cout << " -> Extracted: " << outPath << " (" << meta.fileSize << " bytes)\n";
    }

    std::cout << "[+] Unpacking complete!\n";
}

int main() {
    std::vector<std::string> files = { "test1.txt", "test2.txt" };
    std::string archiveName = "mybackup.chichi";

    PackArchive(archiveName, files);
    UnpackArchive(archiveName);

    return 0;
}
