#include "report.h"
#include <iostream>
#include <iomanip>

namespace cliflow {

void ReportGenerator::generate(const Stats& stats) {
    std::cout << "=== CLIFLOW REPORT ===" << std::endl;
    std::cout << "Total Commands: " << stats.totalCommands << std::endl;
    std::cout << "Unique Commands: " << stats.uniqueCommands << std::endl;
    std::cout << std::endl;

    std::cout << "--- Top Commands ---" << std::endl;
    int rank = 1;
    for (const auto& pair : stats.topCommands) {
        // Example: 1. git        → 312 times
        // We'll trust the default left/right alignment or use setw for basic alignment if needed.
        // Let's use basic spacing for now.
        std::cout << rank++ << ". " 
                  << std::left << std::setw(15) << pair.first 
                  << " -> " << pair.second << " times" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "--- Longest Command ---" << std::endl;
    std::cout << "Length: " << stats.longestCommand.second << " chars" << std::endl;
    // If command is very long, maybe we should wrap it? 
    // Requirement just says "Show: The longest single command ever typed".
    std::cout << stats.longestCommand.first << std::endl;
    std::cout << std::endl;

    std::cout << "--- Risky Commands ---" << std::endl;
    if (stats.riskyCommands.empty()) {
        std::cout << "No risky commands detected." << std::endl;
    } else {
        for (const auto& pair : stats.riskyCommands) {
            std::cout << "Pattern: " << pair.first 
                      // Actually pair.first is the FULL command here because we stored the raw command in map
                      // But wait, in analyzer.cpp we did: stats.riskyCommands[cmd]++;
                      // So it prints the exact command found.
                      // "Print count and examples". 
                      // Let's print the command and how many times it appeared.
                      << " (Count: " << pair.second << ")" << std::endl;
        }
    }
    std::cout << std::endl;
}

} // namespace cliflow
