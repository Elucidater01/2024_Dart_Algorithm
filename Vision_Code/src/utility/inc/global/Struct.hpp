#ifndef __GLOBAL_STRUCT_HPP__
#define __GLOBAL_STRUCT_HPP__

#include <opencv2/opencv.hpp>
#include "Time_generated.h"

struct VisionData {
    uint8_t header = 0xA5;
    uint8_t CRC8 = 0;

    float yaw_error;
    uint8_t target_status; // 0: Lost 1: Found

    uint16_t CRC16 = 0;
}__attribute__((packed));

// publish
struct CarData {
    uint8_t header = 0xA5;
    uint8_t mode; // 0: 不开自瞄, 不录像 1: 开自瞄且录像 2: 录像
    uint8_t CRC8;

    uint8_t status;
    uint8_t number; // 第number号飞镖
    uint8_t dune; //0：完全展开 1：完全关闭 2：正在进行时
    uint16_t CRC16;

}__attribute__((packed));

typedef struct {
    CarData cd;
    foxglove::Time timestamp;
    cv::Mat img;
} FromData;

typedef struct {
    cv::Point2d center_point;
    cv::Rect2d box;
    double score;
} LightInfo;

typedef struct {
    std::vector<LightInfo> lights;
    foxglove::Time timestamp;

    cv::Mat ori_mat;
    CarData cd;

} ImageInfo;


typedef struct {
    std::deque<LightInfo> vec;
    double drop_frame;
    double found_frame;
    int state;
}TargetCache;

typedef struct {
    CarData cd;
    VisionData vd;
    foxglove::Time timestamp;
    cv::Mat img;
} CaptureInfo;

typedef struct {
    cv::Mat raw_image;
} RawImageInfo;


#endif // __GLOBAL_STRUCT_HPP__