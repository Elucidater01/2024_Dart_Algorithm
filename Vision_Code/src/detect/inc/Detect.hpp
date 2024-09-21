#ifndef __ARMOR_DETECT_HPP__
#define __ARMOR_DETECT_HPP__

#include <opencv2/opencv.hpp>

#include "lockfree/LockFree.hpp"
#include "openvino/OpenvinoInfer.hpp"
#include "global/Struct.hpp"

class Detect {
public:
    explicit Detect(const char *mode);
    Detect() = delete;
    ~Detect();

    void detect(
            LfStack<FromData> &message_stack,
            LfStack<ImageInfo> &image_info_stack,
            LfStack <ImageInfo> &mouse_info_stack);

private:
    OpenvinoInfer *openvino_infer_;

    FromData last_message_;

    std::deque<double> time_minus_queue_;

    bool startup_ = true;

    std::string mode_;

};


#endif // __ARMOR_DETECT_HPP__