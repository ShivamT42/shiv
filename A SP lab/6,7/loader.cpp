#include <iostream>
#include <fstream>

class Loader {
public:
    int size;

    Loader(int size) : size(size) {}

    bool memoryAvailable(const std::string& fileName) {
        std::ifstream file(fileName, std::ifstream::ate | std::ifstream::binary);
        if (!file) {
            std::cerr << "File not found: " << fileName << std::endl;
            return false;
        }
        std::streampos fileSize = file.tellg();
        std::cout << "File size: " << fileSize << std::endl;
        if (fileSize < size) {
            size -= static_cast<int>(fileSize);
            std::cout << "Available Memory: " << size << std::endl;
            return true;
        }
        std::cout << "Insufficient memory" << std::endl;
        return false;
    }
};

int main() {
    std::cout << "Enter file size: ";
    int size;
    std::cin >> size;

    Loader loader(size);

    std::string fileName;
    std::cout << "Enter file name: ";
    std::cin >> fileName;

    if (loader.memoryAvailable(fileName)) {
        std::cout << "Memory is available to load the file." << std::endl;
    } else {
        std::cout << "Not enough memory to load the file." << std::endl;
    }

    return 0;
}
