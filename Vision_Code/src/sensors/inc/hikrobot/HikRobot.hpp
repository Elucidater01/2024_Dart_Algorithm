#ifndef HIKROBOT_HPP_
#define HIKROBOT_HPP_

#include "sensors.hpp"

#include "inc/MvCameraControl.h"

enum Camera_State{
    IDLE,
    OPENED,
    GRABBING,
    WRONG_STATE
};

class HikRobot : public virtual sensors::BaseCamera {
public:
    explicit HikRobot(SerialPort &sp);
    ~HikRobot();

    void cameraInit() override;

private:
    void getImage() override;
    void cameraModeChange(int now_mode, int my_color) override;
    void assertSuccess(uint32_t status) override;

    bool createHandle();

    bool openCamera();

    void closeCamera();

    unsigned int getIntValue(const char *key);

    float getFloatValue(const char *key);

    unsigned  int getEnumValue(const char *key);

    bool getBoolValue(const char *key);

    MVCC_STRINGVALUE getStringValue(const char *key);

    void setTriggerMode();

    void setIntValue(const char *key, unsigned int value);

    void setFloatValue(const char *key, float value);

    void setEnumValue(const char *key, unsigned int value);

    void setBoolValue(const char *key, bool value);

    void setStringValue(const char *key, const char *value);

    void setResolution(
            int max_width,
            int roi_width,
            int roi_width_offset,
            int max_height,
            int roi_height,
            int roi_height_offset);

    bool grapImage();
    MV_CC_DEVICE_INFO_LIST stDeviceList_;

    MV_CC_DEVICE_INFO* pDeviceInfo_ = nullptr;

    MVCC_INTVALUE stParam_;


    void *handle_ = nullptr;

    uint8_t camera_size_ = 0;

    unsigned int nDataSize_ = 0;

    unsigned char* pBGR_ = nullptr;

    int camera_state_ = Camera_State::IDLE;

    MV_FRAME_OUT stOutFrame_;

    foxglove::Time error_;
};

#endif // HIKROBOT_HPP_