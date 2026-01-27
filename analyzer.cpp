#include "analyzer.h"
#include <algorithm>
#include <vector>
#include <iostream>

namespace cliflow {

// Initializer list to copy commands into member variables
Analyzer::Analyzer(const std::vector<std::string>& commands) 
    : m_commands(commands) {}

bool Analyzer::isRisky(const std::string& cmd) {
    // Simple substring checks as per requirements
    // rm -rf, sudo rm, mkfs, dd if=
    if (cmd.find("rm -rf") != std::string::npos) return true;
    if (cmd.find("sudo rm") != std::string::npos) return true;
    if (cmd.find("mkfs") != std::string::npos) return true;
    if (cmd.find("dd if=") != std::string::npos) return true;
    return false;
}

void Analyzer::computeTop(Stats& stats, const std::unordered_map<std::string, int>& freq, int n) {
    // Convert map to vector for sorting
    std::vector<std::pair<std::string, int>> sortedCmds(freq.begin(), freq.end());
    
    // Sort by frequency descending
    std::sort(sortedCmds.begin(), sortedCmds.end(), 
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        }
    );

    // Take top N
    for (int i = 0; i < std::min((int)sortedCmds.size(), n); ++i) {
        stats.topCommands.push_back(sortedCmds[i]);
    }
}

void Analyzer::computeRisky(Stats& stats) {
    for (const auto& cmd : m_commands) {
        if (isRisky(cmd)) {
            // We'll store the exact dangerous command string found in the history?
            // Or maybe we want to group them? 
            // The requirement says "Print count and examples". 
            // Let's store unique dangerous commands for the report to decide.
            stats.riskyCommands[cmd]++;
        }
    }
}

void Analyzer::computeLongest(Stats& stats) {
    for (const auto& cmd : m_commands) {
        if (cmd.length() > stats.longestCommand.second) {
            stats.longestCommand = {cmd, (int)cmd.length()};
        }
    }
}

Stats Analyzer::analyze(int topN) {
    Stats stats;
    stats.totalCommands = m_commands.size();

    std::unordered_map<std::string, int> freq;
    for (const auto& cmd : m_commands) {
        // For frequency, we usually want the base command (e.g. 'git') or the full line?
        // Requirement 5.1 says "git -> 312 times", "cd -> 289 times".
        // This implies extracting the first token.
        // HOWEVER, "Longest Command" example shows full arguments.
        // Let's count frequency based on the *first word* (program name) as is common in these tools,
        // OR the full command line? 
        // Example: "git -> 312". This strongly implies the first token.
        // But "Top Commands" usually means "most used programs".
        // Let's assume first token for frequency map, but we keep full lines for "Longest".
        
        std::string prog = cmd;
        size_t spacePos = cmd.find(' ');
        if (spacePos != std::string::npos) {
            prog = cmd.substr(0, spacePos);
        }
        if (!prog.empty()) {
             freq[prog]++;
        }
    }
    
    stats.uniqueCommands = freq.size();
    
    computeTop(stats, freq, topN);
    computeRisky(stats);
    computeLongest(stats);

    return stats;
}

} // namespace cliflow
