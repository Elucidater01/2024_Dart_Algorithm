#include "function/function.hpp"

#include <fstream>
#include <filesystem>

uint64_t function::nanosecondsSinceEpoch() {
    return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
}


foxglove::Time function::getNowTimestamp() {
    auto now = function::nanosecondsSinceEpoch();
    return foxglove::Time(now / 1'000'000'000, now % 1'000'000'000);
}


double function::timestampMinus(
        const foxglove::Time &timestamp1,
        const foxglove::Time &timestamp2) {
    return (double)(((double)timestamp1.sec() - (double)timestamp2.sec()) * 1e3 +
                    ((double)timestamp1.nsec() - (double)timestamp2.nsec()) / 1e6);
}

std::string function::getFileContents(std::string_view path) {
    std::ifstream infile;
    infile.open(path.data(), std::ios::binary | std::ios::in);

    if (!infile) {
        throw std::runtime_error("Could not open file " + std::string(path));
    }

    infile.seekg(0, std::ios::end);
    int length = infile.tellg();
    infile.seekg(0, std::ios::beg);
    std::string result(length, '\0');
    infile.read(result.data(), length);
    infile.close();
    return result;
}


void function::tryToCreateAimFile(std::string &path) {
    std::filesystem::path file(path);

    if (!std::filesystem::exists(file)) {
        std::cout << "["<< file << "] does not exit, try to create" << std::endl;
        try {
            std::filesystem::create_directories(file.parent_path());
            std::ofstream outfile(file);
            outfile.close();
        } catch (std::exception &e) {
            std::cout << "create [" << file << "] failed: " << e.what() << std::endl;
        }
    }
}

std::string function::getLocalTime() {
    time_t now_time;
    time(&now_time);
    tm *p = localtime(&now_time);
    std::string now_time_str = std::to_string(p->tm_year + 1900) + "_" +
                               std::to_string(p->tm_mon + 1) + "_" +
                               std::to_string(p->tm_mday) + "_" +
                               std::to_string(p->tm_hour) + "_" +
                               std::to_string(p->tm_min) + "_" +
                               std::to_string(p->tm_sec);
    return now_time_str;
}

std::string function::getBagPath() {
    auto now_time_str = function::getLocalTime();
    return "../bag/" + now_time_str + ".mcap";
}