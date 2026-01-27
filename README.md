# cliflow

A production-quality CLI analytics tool that reads a user’s shell history file and generates insightful usage statistics. 
Built with modern C++ (compatible with C++17/14/11 restricted environments) using only the standard library (STL).

## Features

- **Top Commands**: Displays the top N most frequently used commands.
- **Longest Command**: Identifies the single longest command ever typed.
- **Risky Commands**: Detects and counts potentially dangerous commands (e.g., `rm -rf`, `sudo rm`, `mkfs`, `dd if=`).
- **Cross-Platform**: Works on Linux, macOS, and Windows.
- **Auto-Detection**: Automatically finds `~/.bash_history` or `~/.zsh_history`.
- **Parsing**: Handles Bash and Zsh extended history formats, ignoring empty lines and consecutive duplicates.

## Building

Requires a C++ compiler (GCC/Clang/MSVC). 
The code is written to be compatible with older compilers (tested on GCC 6.3.0) by avoiding C++17 specific features like `<filesystem>` and `<optional>`.

```bash
g++ -std=c++17 *.cpp -o cliflow
```

## Installation (Binaries)

You don't need to compile the code yourself. You can download the latest pre-compiled binary for your system from the [Releases](https://github.com/Naman13112004/cli-flow/releases) page.

1.  **Download** the binary for your OS (`cliflow.exe` for Windows, `cliflow-linux`, or `cliflow-macos`).
2.  **Make Global** (Optional):
    *   **Windows**: Move `cliflow.exe` to a folder in your `PATH` (e.g., `C:\Windows\System32` or a custom `bin` folder).
    *   **Linux/macOS**:
        ```bash
        chmod +x cliflow-*
        sudo mv cliflow-* /usr/local/bin/cliflow
        ```
3.  **Run**:
    ```bash
    cliflow --help
    ```

## Usage

### Default (Auto-detect)
Analyzes `~/.zsh_history` or `~/.bash_history`:
```bash
./cliflow
```

### Specify File
Analyze a specific history file:
```bash
./cliflow --file /path/to/.bash_history
```

### Custom Top N
Show the top 20 commands (default is 10):
```bash
./cliflow --top 20
```

### Help
Show usage instructions:
```bash
./cliflow --help
```

## Example Output

```text
=== CLIFLOW REPORT ===
Total Commands: 8432
Unique Commands: 91

--- Top Commands ---
1. git             -> 312 times
2. cd              -> 289 times
3. docker          -> 150 times
...

--- Longest Command ---
Length: 127 chars
docker run -it -v $(pwd):/app ubuntu bash

--- Risky Commands ---
Pattern: rm -rf /tmp/junk (Count: 1)
```

## Project Structure

- **`main.cpp`**: CLI argument parsing and application orchestration.
- **`parser.h/cpp`**: Handles file reading and history format parsing.
- **`analyzer.h/cpp`**: Computes statistics (frequencies, risks, etc.).
- **`report.h/cpp`**: Formats and prints the final report.

## Design Principles

- **Zero Dependencies**: Uses only C++ Standard Library (STL).
- **Performance**: O(n) parsing and analysis.
- **Robustness**: Handles malformed lines and large files without crashing.
- **Separation of Concerns**: specialized modules for parsing, analysis, and reporting.
