#include "file_search.h"
#include <iostream>
#include <string>
#include <chrono>

int main() {

    #ifdef _WIN32
    std::string rootDirectory = "C:\\";
    #elif
    std::string rootDirectory = "/";
    #endif
    
    int maxThreads = 8;

    std::string targetFile;
    std::cout << "Enter the name of the file to search for: ";
    std::cin >> targetFile;

    auto startTime = std::chrono::high_resolution_clock::now();

    if (searchFile(rootDirectory, targetFile, maxThreads)) {
        std::cout << "File found." << std::endl;
    } else {
        std::cout << "File not found." << std::endl;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Execution time: " << duration << " milliseconds" << std::endl;
    system("pause");
    return 0;
}
