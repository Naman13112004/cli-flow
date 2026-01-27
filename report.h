#ifndef REPORT_H
#define REPORT_H

#include "analyzer.h"

namespace cliflow {

class ReportGenerator {
public:
    static void generate(const Stats& stats);
};

} // namespace cliflow

#endif // REPORT_H
