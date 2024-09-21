#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <opencv2/opencv.hpp>
class ReJson {
public:
    template<typename T>
    ReJson(T &path) : path_(path) {
        config_.open(path_, cv::FileStorage::READ);
    }
    ~ReJson();

    cv::FileStorage config_;
    void updateJson();

private:
    std::string path_;
    time_t last_modified_time_ = 0;
    bool verifyModifiedTime();
};

#endif // __JSON_HPP__