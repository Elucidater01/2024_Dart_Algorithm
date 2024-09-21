#include "hikrobot/HikRobot.hpp"

#include <opencv2/opencv.hpp>
#include <thread>

#include "function/function.hpp"

HikRobot::HikRobot(SerialPort &sp) : sensors::BaseCamera(sp) {
    this->error_ = function::getNowTimestamp();
}

HikRobot::~HikRobot() {
    this->closeCamera();
}

void HikRobot::cameraInit() {
    auto n_ret = MV_OK;
    this->camera_size_ = 0;
    memset(&this->stDeviceList_, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    n_ret = MV_CC_EnumDevices(MV_USB_DEVICE, &this->stDeviceList_);

    if (n_ret != MV_OK) {
        printf("MV_CC_EnumDevices fail for n_ret = [%x].\n", n_ret);
        return;
    }

    if (this->stDeviceList_.nDeviceNum > 0) {
        for (size_t i = 0 ; i < this->stDeviceList_.nDeviceNum ; ++i) {
            this->pDeviceInfo_ = this->stDeviceList_.pDeviceInfo[i];

            if (this->pDeviceInfo_ == nullptr) {
                break;
            }

            if (!this->createHandle()) {
                continue;
            }

            if (!this->openCamera()) {
                continue;
            }

            this->setEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);
            this->setEnumValue("PixelFormat", PixelType_Gvsp_BayerRG8);
            this->setEnumValue("ADCBitDepth", 0);
            this->setBoolValue("AcquisitionFrameRateEnable", true);
            this->setFloatValue("AcquisitionFrameRate", 250.0);
            this->setResolution(-1, 384, 300, -1, 640, 100);
            if (!this->grapImage()) {
                return;
            }
            setFloatValue("ExposureTime", 3000);
            setFloatValue("Gain", 8.0);//0~16
            ++this->camera_size_;
        }
    } else {
        printf("No Devices Found.\n");
        return;
    }

    printf("Initialize %d camera(s).\n", this->camera_size_);

}

bool HikRobot::createHandle() {
    auto n_ret = MV_CC_CreateHandle(&this->handle_, this->pDeviceInfo_);

    if (n_ret != MV_OK) {
        printf("MV_CC_CreateHandle fail for n_ret = [%x].\n", n_ret);
        return false;
    }

    return true;
}

bool HikRobot::openCamera() {
    if (this->handle_ == nullptr) {
        return false;
    }

    auto n_ret = MV_CC_OpenDevice(this->handle_);

    if (n_ret != MV_OK) {
        printf("MV_CC_OpenDevice fail for n_ret = [%x].\n", n_ret);
        return false;
    }

    return true;
}

void HikRobot::closeCamera() {
    auto n_ret = MV_CC_StopGrabbing(this->handle_);
    if (n_ret != MV_OK) {
        printf("MV_CC_StopGrabbing fail for n_ret = [%x].\n", n_ret);
    }

    n_ret = MV_CC_CloseDevice(this->handle_);
    if (n_ret != MV_OK) {
        printf("MV_CC_CloseDevice fail for n_ret = [%x].\n", n_ret);
    }

    n_ret = MV_CC_DestroyHandle(this->handle_);
    if (n_ret != MV_OK) {
        printf("MV_CC_DestroyHandle fail for n_ret = [%x].\n", n_ret);
    }
}

unsigned int HikRobot::getIntValue(const char *key) {
    MVCC_INTVALUE value;
    auto n_ret = MV_CC_GetIntValue(this->handle_, key, &value);

    if (n_ret != MV_OK) {
        printf("MV_CC_GetIntValue fail for n_ret = [%x].\n", n_ret);
        return 0;
    }

    return value.nCurValue;
}

float HikRobot::getFloatValue(const char *key) {
    MVCC_FLOATVALUE value;
    auto n_ret = MV_CC_GetFloatValue(this->handle_, key, &value);

    if (n_ret != MV_OK) {
        printf("MV_CC_GetFloatValue fail for n_ret = [%x].\n", n_ret);
        return 0;
    }

    return value.fCurValue;
}

unsigned int HikRobot::getEnumValue(const char *key) {
    MVCC_ENUMVALUE value;
    auto n_ret = MV_CC_GetEnumValue(this->handle_, key, &value);

    if (n_ret != MV_OK) {
        printf("MV_CC_GetEnumValue fail for n_ret = [%x].\n", n_ret);
        return 0;
    }

    return value.nCurValue;
}

bool HikRobot::getBoolValue(const char *key) {
    bool value;
    auto n_ret = MV_CC_GetBoolValue(this->handle_, key, &value);

    if (n_ret != MV_OK) {
        printf("MV_CC_GetBoolValue fail for n_ret = [%x].\n", n_ret);
        return false;
    }

    return value;
}

MVCC_STRINGVALUE HikRobot::getStringValue(const char *key) {
    MVCC_STRINGVALUE value;
    auto n_ret = MV_CC_GetStringValue(this->handle_, key, &value);

    if (n_ret != MV_OK) {
        printf("MV_CC_GetStringValue fail for n_ret = [%x].\n", n_ret);
        return value;
    }

    return value;
}

void HikRobot::setTriggerMode() {
    auto n_ret = MV_CC_SetEnumValue(this->handle_, "TriggerMode", 0);

    if (n_ret != MV_OK) {
        printf("MV_CC_SetEnumValue fail for n_ret = [%x].\n", n_ret);
    }
}

void HikRobot::setIntValue(const char *key, unsigned int value) {
    auto n_ret = MV_CC_SetIntValue(this->handle_, key, value);

    if (n_ret != MV_OK) {
        printf("MV_CC_SetIntValue fail for n_ret = [%x].\n", n_ret);
    }
}

void HikRobot::setFloatValue(const char *key, float value) {
    auto n_ret = MV_CC_SetFloatValue(this->handle_, key, value);

    if (n_ret != MV_OK) {
        printf("MV_CC_SetFloatValue fail for n_ret = [%x].\n", n_ret);
    }
}

void HikRobot::setEnumValue(const char *key, unsigned int value) {
    auto n_ret = MV_CC_SetEnumValue(this->handle_, key, value);

    if (n_ret != MV_OK) {
        printf("MV_CC_SetEnumValue fail for n_ret = [%x].\n", n_ret);
    }
}

void HikRobot::setBoolValue(const char *key, bool value) {
    auto n_ret = MV_CC_SetBoolValue(this->handle_, key, value);

    if (n_ret != MV_OK) {
        printf("MV_CC_SetBoolValue fail for n_ret = [%x].\n", n_ret);
    }
}

void HikRobot::setResolution(
        int max_width,
        int roi_width,
        int roi_width_offset,
        int max_height,
        int roi_height,
        int roi_height_offset) {
    auto n_ret = MV_OK;
    if (this->camera_state_ >= Camera_State::GRABBING) {
        printf("Stop grabbing first.\n");

        n_ret = MV_CC_StopGrabbing(this->handle_);

        cv::waitKey(200);
    }

    if (n_ret != MV_OK) {
        printf("MV_CC_StopGrabbing fail for n_ret = [%x].\n", n_ret);
        return;
    }

    this->setIntValue("Width", roi_width);
    this->setIntValue("OffsetX", roi_width_offset);

    this->setIntValue("Height", roi_height);
    this->setIntValue("OffsetY", roi_height_offset);

    if (this->camera_state_ >= Camera_State::GRABBING) {
        this->grapImage();
    }
}

bool HikRobot::grapImage() {
    auto n_ret = MV_CC_StartGrabbing(this->handle_);
    if (n_ret != MV_OK) {
        printf("MV_CC_StartGrabbing fail for n_ret = [%x].\n", n_ret);
        return false;
    }

    memset(&this->stParam_, 0, sizeof(MVCC_INTVALUE));
    n_ret = MV_CC_GetIntValue(this->handle_, "PayloadSize", &this->stParam_);
    if (n_ret != MV_OK) {
        printf("MV_CC_GetIntValue fail for n_ret = [%x].\n", n_ret);
        return false;
    }


    this->nDataSize_ = this->stParam_.nCurValue;

    this->camera_state_ = Camera_State::GRABBING;

    return true;
}

void HikRobot::cameraModeChange(int now_mode, int my_color) {
    if ((now_mode == 1 || now_mode == 0) && (now_mode != this->pre_mode_ || my_color != this->pre_my_color_)) {
        if (my_color == 0) {
            std::cout << "Detect Red." << std::endl;
            float val = J_DETECT.config_["camera"]["daheng"]["exposure"]["red_exposure"];
            std::cout << "val = " << val << std::endl;
            this->setFloatValue("ExposureTime", val);
        } else if (my_color == 1) {
            std::cout << "Detect Blue." << std::endl;
            float val = J_DETECT.config_["camera"]["daheng"]["exposure"]["blue_exposure"];
            std::cout << "val = " << val << std::endl;
            this->setFloatValue("ExposureTime", val);
        }
    }
    this->pre_mode_ = now_mode;
    this->pre_my_color_ = my_color;
}

void HikRobot::assertSuccess(uint32_t status) {
    if (status != MV_OK) {
        printf("Camera_Error: %x\n", status);
        this->closeCamera();
        return;
    }
}

void HikRobot::getImage() {
    CarData car_data;
    while(!this->sp_.receive(car_data)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
//    this->cameraModeChange(car_data.mode, car_data.my_color);

    this->message_.cd = car_data;
    this->message_.timestamp = function::getNowTimestamp();
    this->error_ = function::getNowTimestamp();
    this->stOutFrame_ = {0};
    memset(&this->stOutFrame_, 0, sizeof(MV_FRAME_OUT));
    while(MV_CC_GetImageBuffer(this->handle_, &this->stOutFrame_, 1000) != MV_OK) {
        this->message_.timestamp = function::getNowTimestamp();
        auto minus = function::timestampMinus(this->message_.timestamp, this->error_) / 1e3;
        if (minus > (double)J_SENSORS.config_["camera"]["drop_time"]) {
            exit(0);
        }
    }

    if (this->stOutFrame_.stFrameInfo.enPixelType == PixelType_Gvsp_BayerRG8 && this->stOutFrame_.pBufAddr != nullptr) {

        delete[] this->pBGR_;
        this->pBGR_ = new unsigned char[sizeof(unsigned char) * this->stOutFrame_.stFrameInfo.nWidth * this->stOutFrame_.stFrameInfo.nHeight * 4 + 2048];

        MV_CC_PIXEL_CONVERT_PARAM stConvertParam;
        stConvertParam.nWidth = this->stOutFrame_.stFrameInfo.nWidth;
        stConvertParam.nHeight = this->stOutFrame_.stFrameInfo.nHeight;
        stConvertParam.pSrcData = this->stOutFrame_.pBufAddr;
        stConvertParam.nSrcDataLen = this->stOutFrame_.stFrameInfo.nFrameLen;
        stConvertParam.enSrcPixelType = this->stOutFrame_.stFrameInfo.enPixelType;
        stConvertParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed;
        stConvertParam.pDstBuffer = this->pBGR_;
        stConvertParam.nDstBufferSize = this->stOutFrame_.stFrameInfo.nWidth * this->stOutFrame_.stFrameInfo.nHeight * 4 + 2048;
        auto n_ret = MV_CC_ConvertPixelType(this->handle_, &stConvertParam);

        if (n_ret != MV_OK) {
            printf("MV_CC_ConvertPixelType fail for n_ret = [%x].\n", n_ret);
            return;
        }

        this->message_.img = cv::Mat(this->stOutFrame_.stFrameInfo.nHeight, this->stOutFrame_.stFrameInfo.nWidth, CV_8UC3, this->pBGR_).clone();
        cv::transpose(this->message_.img, this->message_.img);
        cv::flip(this->message_.img, this->message_.img, -1);
        cv::flip(this->message_.img, this->message_.img, 1);
        //
        MV_CC_FreeImageBuffer(this->handle_, &this->stOutFrame_);
//        cv::imshow("img", this->message_.img);
//        cv::waitKey(1);
    } else {
        printf("Unsupported PixelType.\n");
        return;
    }
}




