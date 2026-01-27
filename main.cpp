#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "parser.h"
#include "analyzer.h"
#include "report.h"

void showHelp() {
    std::cout << "cliflow - CLI Shell History Analytics" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  cliflow [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --file <path>   Specify history file to analyze" << std::endl;
    std::cout << "  --top <N>       Number of top commands to show (default: 10)" << std::endl;
    std::cout << "  --help          Show this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    // Basic argument parsing
    std::string historyFile;
    int topN = 10;

    std::vector<std::string> args(argv + 1, argv + argc);
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--help") {
            showHelp();
            return 0;
        } else if (args[i] == "--file") {
            if (i + 1 < args.size()) {
                historyFile = args[i + 1];
                i++; // Skip next arg
            } else {
                std::cerr << "Error: --file requires a path argument." << std::endl;
                return 1;
            }
        } else if (args[i] == "--top") {
            if (i + 1 < args.size()) {
                try {
                    topN = std::stoi(args[i + 1]);
                    i++;
                } catch (...) {
                    std::cerr << "Error: --top requires an integer argument." << std::endl;
                    return 1;
                }
            } else {
                std::cerr << "Error: --top requires an argument." << std::endl;
                return 1;
            }
        }
    }

    // Auto-detect if file not specified
    if (historyFile.empty()) {
        std::string detected = cliflow::Parser::detectHistoryFile();
        if (!detected.empty()) {
            historyFile = detected;
            std::cout << "Auto-detected history file: " << historyFile << std::endl;
        } else {
            std::cerr << "Error: Could not automatically find .bash_history or .zsh_history." << std::endl;
            std::cerr << "Please specify one using --file <path>" << std::endl;
            return 1;
        }
    }

    std::vector<std::string> commands = cliflow::Parser::parseFile(historyFile);

    if (commands.empty()) {
        std::cout << "History file is empty or contains no valid commands." << std::endl;
        return 0;
    }

    cliflow::Analyzer analyzer(commands);
    cliflow::Stats stats = analyzer.analyze(topN);
    
    cliflow::ReportGenerator::generate(stats);

    return 0;
}
