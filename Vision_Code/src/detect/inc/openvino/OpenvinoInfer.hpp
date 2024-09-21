#ifndef OPENVINO_INFER_HPP_
#define OPENVINO_INFER_HPP_
#include <openvino/openvino.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

class OpenvinoInfer {
public:
    struct Resize {
        cv::Mat resized_image;
        int dw;
        int dh;
    };

    struct Light {
        int id = -1;
        double score = 0.;
        cv::Rect2d box;
        cv::Point2d center_point;
    };

    explicit OpenvinoInfer(std::map<std::string, std::string> &path);
    ~OpenvinoInfer() = default;

    static void fitRec(std::vector<OpenvinoInfer::Light> &bboxes, cv::Size2d ori_size, cv::Size2d now_size);
    std::vector<OpenvinoInfer::Light> infer(cv::Mat &src, const cv::Size2d &dst_size, const int &my_color, const bool &startup);

private:
    static OpenvinoInfer::Resize letterBox(cv::Mat &src, const cv::Size2d &dst_size);

    ov::Core core_;
    ov::CompiledModel compiled_model_;
    ov::CompiledModel compiled_model_next_;
    std::shared_ptr<ov::Model> model_;
    std::map<bool, ov::InferRequest> infer_requests_;
};



#endif