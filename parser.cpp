#include "parser.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>

namespace cliflow {

// Helper to check file existence
static bool fileExists(const std::string& path) {
    if (path.empty()) return false;
    std::ifstream f(path);
    return f.good();
}

std::string Parser::detectHistoryFile() {
    const char* home = std::getenv("HOME");
    if (!home) {
        home = std::getenv("USERPROFILE");
    }
    
    if (!home) return "";

    std::string homeDir = home;
    // Basic path handling for Windows vs Unix
    if (homeDir.back() != '/' && homeDir.back() != '\\') {
#ifdef _WIN32
        homeDir += "\\";
#else
        homeDir += "/";
#endif
    }

    std::string zshHist = homeDir + ".zsh_history";
    std::string bashHist = homeDir + ".bash_history";

    if (fileExists(zshHist)) {
        return zshHist;
    }
    if (fileExists(bashHist)) {
        return bashHist;
    }

    return "";
}

bool Parser::isZshTimestamp(const std::string& line) {
    // Basic check for Zsh extended history format: : 1234567890:0;cmd
    if (line.size() < 15) return false;
    return line[0] == ':' && line[1] == ' ' && line.find(';') != std::string::npos;
}

std::string Parser::cleanLine(const std::string& line) {
    if (line.empty()) return "";

    // Handle Zsh extended history
    // Format: : <timestamp>:<execution_time>;<command>
    if (isZshTimestamp(line)) {
        size_t semicolonPos = line.find(';');
        if (semicolonPos != std::string::npos && semicolonPos + 1 < line.size()) {
            return line.substr(semicolonPos + 1);
        }
        return ""; // Malformed zsh line or just metadata
    }

    return line;
}

std::vector<std::string> Parser::parseFile(const std::string& filepath) {
    std::vector<std::string> commands;
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return commands;
    }

    std::string line;
    std::string lastCmd;

    while (std::getline(file, line)) {
        std::string cleaned = cleanLine(line);

        // Ignore empty lines
        if (cleaned.empty()) continue;

        // Ignore consecutive duplicates
        if (cleaned == lastCmd) continue;

        commands.push_back(cleaned);
        lastCmd = cleaned;
    }

    return commands;
}

} // namespace cliflow
