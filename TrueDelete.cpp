
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::string filePath;
    std::cout << "Path to your file (drag & drop): ";
    std::getline(std::cin, filePath);

    if (filePath.size() >= 2 && filePath.front() == '"' && filePath.back() == '"')
        filePath = filePath.substr(1, filePath.size() - 2);

    DWORD attributes = GetFileAttributesA(filePath.c_str());

    if (attributes == INVALID_FILE_ATTRIBUTES ||
        (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        std::cout << "Invalid file path.\n";
        return 1;
    }

    HANDLE file = CreateFileA(
        filePath.c_str(),
        GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file == INVALID_HANDLE_VALUE) {
        std::cout << "Could not open file.\n";
        return 1;
    }

    LARGE_INTEGER size;

    if (!GetFileSizeEx(file, &size)) {
        std::cout << "Could not get file size.\n";
        CloseHandle(file);
        return 1;
    }

    std::cout << "\nFile size: " << size.QuadPart << " bytes\n";

    std::string confirmation;
    std::cout << "Overwrite with zeros and delete? (y/n): ";
    std::getline(std::cin, confirmation);

    if (confirmation != "y" && confirmation != "Y") {
        std::cout << "Cancelled.\n";
        CloseHandle(file);
        return 0;
    }

    std::vector<char> zeros(1024 * 1024, 0);
    LONGLONG remaining = size.QuadPart;

    while (remaining > 0) {
        DWORD amount = static_cast<DWORD>(
            std::min<LONGLONG>(remaining, zeros.size())
            );

        DWORD written = 0;

        if (!WriteFile(file, zeros.data(), amount, &written, nullptr) ||
            written != amount) {
            std::cout << "Overwrite failed. File not deleted.\n";
            CloseHandle(file);
            return 1;
        }

        remaining -= written;
    }

    if (!FlushFileBuffers(file)) {
        std::cout << "Flush failed. File not deleted.\n";
        CloseHandle(file);
        return 1;
    }

    CloseHandle(file);

    if (!DeleteFileA(filePath.c_str())) {
        std::cout << "Deletion failed. Error: " << GetLastError() << '\n';
        return 1;
    }

    std::cout << "File overwritten and deleted.\n";
    return 0;
}
