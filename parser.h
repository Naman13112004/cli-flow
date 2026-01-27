#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

namespace cliflow {

class Parser {
public:
    // Tries to find default history file if path is empty. Returns empty string if not found.
    static std::string detectHistoryFile();

    // Parses the given history file and returns a list of commands
    static std::vector<std::string> parseFile(const std::string& filepath);

private:
    static std::string cleanLine(const std::string& line);
    static bool isZshTimestamp(const std::string& line);
};

} // namespace cliflow

#endif // PARSER_H
