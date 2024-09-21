#ifndef TRACK_HPP_
#define TRACK_HPP_

#include "serialport/SerialPort.h"
#include "lockfree/LockFree.hpp"

#include "filter/OneEuroFilter.hpp"
class Track {
public:
    explicit Track(SerialPort &sp, const char *mode);
    ~Track() = default;

    template<class ...Args>
    void update(Args &&...args) {
        this->updateArgs(std::forward<Args>(args)...);
    }
private:

    enum STATE {
            LOST = 0,
            DROP = 1,
            FOUND = 2
        };

    template<typename T, class ...Args>
    void updateArgs(T &&t, Args &&...args) {
        if constexpr (std::is_same_v<T, LfStack<ImageInfo>&>) {
            this->track(t);
        } else if constexpr (std::is_same_v<T, LfStack<CaptureInfo>&>) {
            t.pop();
            t.push(CaptureInfo{
                    .cd = this->cache_.cd,
                    .vd = this->vd_,
                    .timestamp = this->cache_.timestamp,
                    .img = this->cache_.ori_mat.clone()
            });
        } else {
            std::cout << "error type in track." << std::endl;
        }
        this->updateArgs(args...);
    }

    void updateArgs() {};

    void track(LfStack<ImageInfo> &image_info_stack);


    void updateTimeInterval(const foxglove::Time &image_timestamp);

    static void insertTargetVec(
            std::deque<LightInfo> &vec,
            const LightInfo &light,
            const size_t &size);

    void updateState();

    void reset();

    static auto frameLimit(const double &frame, const double &plus, const double &limit) -> double;

    void chooseTarget(ImageInfo &image_info);

    auto filter(const cv::Point2d &point, const double &dt) -> cv::Point2d;

    void updateVisionData();
    SerialPort &sp_;
    std::string mode_;

    TargetCache target_;
    ImageInfo cache_;

    foxglove::Time last_image_timestamp_;
    double image_time_interval_; // s

    VisionData vd_;

    bool init_filter_ = false;
    OneEuroFilter filter_x_;
    OneEuroFilter filter_y_;


    cv::FileStorage config_;
    std::string path_;
};

#endif