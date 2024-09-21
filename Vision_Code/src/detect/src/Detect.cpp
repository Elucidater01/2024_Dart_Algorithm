#include "Detect.hpp"

#include "function/function.hpp"
#include "global/Config.hpp"

Detect::Detect(const char *mode) : mode_(mode) {
    std::map<std::string, std::string> path;
    path.insert(std::pair<std::string, std::string>("XML", J_DETECT.config_["path"]["xml"]));
    path.insert(std::pair<std::string, std::string>("BIN", J_DETECT.config_["path"]["bin"]));
    path.insert(std::pair<std::string, std::string>("DEVICE", J_DETECT.config_["path"]["device"]));
    this->openvino_infer_ = new OpenvinoInfer(path);
}

Detect::~Detect() {
    delete this->openvino_infer_;
}

void Detect::detect(
        LfStack <FromData> &message_stack,
        LfStack <ImageInfo> &image_info_stack,
        LfStack <ImageInfo> &mouse_info_stack) {
    std::optional<FromData> opt_message_ = message_stack.pop();

    if (!opt_message_.has_value()) {
        return;
    }

    auto message = std::move(opt_message_.value());

    auto infer_results = this->openvino_infer_->infer(message.img, cv::Size(640, 384), 0, this->startup_);
//
    if (this->startup_) {
        this->startup_ = false;
        this->last_message_ = std::move(message);
        return;
    }


    //TODO::delete -> armor.id = 3 && add -> armor.id != 0;
    ImageInfo i_i;
    i_i.cd = this->last_message_.cd;
    i_i.ori_mat = this->last_message_.img;
    i_i.timestamp = this->last_message_.timestamp;

    for (const auto &result : infer_results) {
        LightInfo light_info;
        light_info.center_point = result.center_point;
        light_info.score = result.score;
        light_info.box = result.box;

        i_i.lights.emplace_back(light_info);
    }



    image_info_stack.pop();
    image_info_stack.push(i_i);

    mouse_info_stack.pop();
    mouse_info_stack.push(i_i);

    this->last_message_ = std::move(message);
}
