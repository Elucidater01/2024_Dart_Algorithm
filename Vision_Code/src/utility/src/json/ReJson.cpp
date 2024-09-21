#include "json/ReJson.hpp"

#include <iostream>
#include <sys/stat.h>
#include <ctime>

bool ReJson::verifyModifiedTime() {
    struct stat file_state{};
    if (stat(this->path_.c_str(), &file_state) != 0) {
        std::cout << "json file path stat error." << std::endl;
        return false;
    }

    std::time_t modified_time = file_state.st_mtime;

    if (this->last_modified_time_ == 0 && this->last_modified_time_ != modified_time) {
        this->last_modified_time_ = modified_time;
        return false;
    }

    bool status = (this->last_modified_time_ != modified_time);
    this->last_modified_time_ = modified_time;
    return status;
}

void ReJson::updateJson() {
    if (this->verifyModifiedTime()) {
        config_.release();
        config_.open(this->path_, cv::FileStorage::READ);
        std::cout << "json config changed." << std::endl;
    }
}

ReJson::~ReJson() {
    this->config_.release();
}


