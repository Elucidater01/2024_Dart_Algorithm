#ifndef __INLINE_FUNCTION_HPP__
#define __INLINE_FUNCTION_HPP__


#include <opencv2/opencv.hpp>

#include "Time_generated.h"

namespace function {
    uint64_t nanosecondsSinceEpoch();

    foxglove::Time getNowTimestamp();

    double timestampMinus(
            const foxglove::Time &timestamp1,
            const foxglove::Time &timestamp2);

    std::string getFileContents(std::string_view path);
    
    void tryToCreateAimFile(std::string &path);

    std::string getLocalTime();

    std::string getBagPath();
}

#endif