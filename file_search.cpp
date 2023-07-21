#include "file_search.h"
#include <iostream>
#include <thread>
#include <filesystem>
#include <vector>
#include <queue>
#include <mutex>
#include <atomic>
#include <chrono>

std::mutex mtx;
std::atomic<bool> fileFound(false);
std::queue<std::filesystem::path> directoriesQueue;

void searchFileThread(const std::string& targetFile) {
    while (!fileFound) {
        std::filesystem::path directory;
        {
            std::unique_lock<std::mutex> lock(mtx);
            if (directoriesQueue.empty()) {
                return; 
            }
            directory = directoriesQueue.front();
            directoriesQueue.pop();
        }

        try {            
            for (const auto& entry : std::filesystem::directory_iterator(directory)) {
                if (fileFound) {
                    return;
                }

                if (entry.is_directory()) {
                    std::unique_lock<std::mutex> lock(mtx);
                    directoriesQueue.push(entry.path());
                } else if (entry.is_regular_file() && entry.path().filename().string() == targetFile) {
                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        std::cout << "Found file: " << entry.path().string() << std::endl;
                        fileFound = true;
                    }
                    return;
                }
            }
        } catch (const std::filesystem::filesystem_error& ex) {
            
        }
    }
}

bool searchFile(const std::string& rootDirectory, const std::string& targetFile, int maxThreads) {
    directoriesQueue.push(rootDirectory);

    for (const auto& entry : std::filesystem::directory_iterator(rootDirectory)) {
        if (std::filesystem::is_directory(entry)) {
            directoriesQueue.push(entry.path());
        }
    }

    std::vector<std::thread> threadPool;
    for (int i = 0; i < maxThreads; ++i) {
        threadPool.emplace_back(searchFileThread, std::cref(targetFile));
    }

    for (auto& thread : threadPool) {
        thread.join();
    }

    return fileFound;
}
