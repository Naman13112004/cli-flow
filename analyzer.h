#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

namespace cliflow {

struct Stats {
    int totalCommands = 0;
    int uniqueCommands = 0;
    std::vector<std::pair<std::string, int>> topCommands;
    std::pair<std::string, int> longestCommand = {"", 0};
    // Risk detection: command -> count
    std::unordered_map<std::string, int> riskyCommands; 
};

class Analyzer {
public:
    Analyzer(const std::vector<std::string>& commands);
    
    Stats analyze(int topN);

private:
    std::vector<std::string> m_commands;
    
    bool isRisky(const std::string& cmd);
    void computeTop(Stats& stats, const std::unordered_map<std::string, int>& freq, int n);
    void computeRisky(Stats& stats);
    void computeLongest(Stats& stats);
};

} // namespace cliflow

#endif // ANALYZER_H
