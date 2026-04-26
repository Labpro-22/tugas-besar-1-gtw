#include "utils/FileHelper.hpp"
#include "utils/NimonspoliException.hpp"
#include <fstream>

std::vector<std::string> FileHelper::readAllLines(const std::string& path){
    std::ifstream file(path);

    if (!file.is_open()) throw FileTidakValidException();

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}
void FileHelper::writeAllLines(const std::string& path, const std::vector<std::string>& lines){
    std::ofstream file(path);
    for (const std::string& line : lines) {
        file << line << '\n';
    }
}
