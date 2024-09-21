#include "openvino/OpenvinoInfer.hpp"

#include "global/Config.hpp"
#define BLUE 0
#define RED  1
#define NONE_ 2
#define CURR true
#define NEXT false

OpenvinoInfer::OpenvinoInfer(std::map<std::string, std::string> &path) {
    this->model_ = this->core_.read_model(path["XML"], path["BIN"]);

    ov::preprocess::PrePostProcessor ppp = ov::preprocess::PrePostProcessor(this->model_);

    ppp.
            input().
            tensor().
            set_element_type(ov::element::u8).
            set_layout("NHWC").
            set_color_format(ov::preprocess::ColorFormat::BGR);

    ppp.
            input().
            preprocess().
            convert_element_type(ov::element::f32).
            convert_color(ov::preprocess::ColorFormat::RGB).
            scale({255.f, 255.f, 255.f});

    ppp.
            input().
            model().
            set_layout("NCHW");

    ppp.
            output(0).
            tensor().
            set_element_type(ov::element::f32);

    ppp.
            output(1).
            tensor().
            set_element_type(ov::element::f32);

    ppp.
            output(2).
            tensor().
            set_element_type(ov::element::f32);


    this->model_ = ppp.build();
    this->compiled_model_ = this->core_.compile_model(this->model_, path["DEVICE"]);
    this->compiled_model_next_ = this->core_.compile_model(this->model_, path["DEVICE"]);

    this->infer_requests_.insert(std::pair<bool, ov::InferRequest>(CURR, this->compiled_model_.create_infer_request()));
    this->infer_requests_.insert(std::pair<bool, ov::InferRequest>(NEXT, this->compiled_model_next_.create_infer_request()));
}

std::vector<OpenvinoInfer::Light> OpenvinoInfer::infer(cv::Mat &src, const cv::Size2d &dst_size, const int &my_color, const bool &startup) {
    OpenvinoInfer::Resize resize = OpenvinoInfer::letterBox(src, dst_size);
//    cv::imshow("resized_image", resize.resized_image);
//    cv::waitKey(1);
    auto *input_data = (uint8_t *)resize.resized_image.data;

    ov::Tensor input_tensor = ov::Tensor(
            this->compiled_model_.input().get_element_type(),
            this->compiled_model_.input().get_shape(),
            input_data);

    if (startup) {
        this->infer_requests_[CURR].set_input_tensor(input_tensor);
        this->infer_requests_[CURR].start_async();
        return {};
    }

    this->infer_requests_[NEXT].set_input_tensor(input_tensor);
    this->infer_requests_[NEXT].start_async();
    this->infer_requests_[CURR].wait();

//    std::vector<int> _class_ids;
    std::vector<float> confidences;
//    std::vector<double> _sizes;
    std::vector<cv::Rect> boxes;
    std::vector<OpenvinoInfer::Light> Lights;
    std::vector<int> nms_result;
    const ov::Tensor &outputTensor = this->infer_requests_[CURR].get_output_tensor(0);
    ov::Shape outputShape = outputTensor.get_shape();

    auto *outputData = outputTensor.data<float>();
    
    for(size_t i = 0 ; i < outputShape[1] ; ++i){
        float* detection = &outputData[i * outputShape[2]];
        //normalize the coordinate
        float confidence = detection[4];
        if(confidence >= (double)J_DETECT.config_["openvino"]["score_threshold"]){
            float* classesConfidence = &detection[4 + 1];
            float* colorsConfidence = &detection[4 + 1 + 9];
            cv::Mat scores(1, 9, CV_32FC1, classesConfidence);
            cv::Mat colors(1, 4, CV_32FC1, colorsConfidence);
            cv::Point classIdPoint;
            double maxClassConfidence;
            cv::minMaxLoc(scores, nullptr, &maxClassConfidence, nullptr, &classIdPoint);

            if(maxClassConfidence >= (double)J_DETECT.config_["openvino"]["score_threshold"]){

                if (classIdPoint.x != 8) {
                    continue;
                }
                
                OpenvinoInfer::Light Light;

                Light.id = classIdPoint.x;
                Light.score = confidence;
                Light.center_point = cv::Point2f(detection[0], detection[1]);
                Light.box = cv::Rect2d(detection[0] - detection[2] / 2., detection[1] - detection[3] / 2., detection[2], detection[3]);
                Lights.emplace_back(Light);

                confidences.emplace_back(confidence);
                boxes.emplace_back(cv::Rect(detection[0] - detection[2] / 2., detection[1] - detection[3] / 2., detection[2], detection[3]));
               
            }
        }
    }

    cv::dnn::NMSBoxes(
            boxes,
            confidences,
            J_DETECT.config_["openvino"]["score_threshold"],
            J_DETECT.config_["openvino"]["nms_threshold"], nms_result);
    std::vector<OpenvinoInfer::Light> result;
    for(const int &idx : nms_result){
        result.emplace_back(OpenvinoInfer::Light {
                Lights[idx].id,
                Lights[idx].score,
                Lights[idx].box,
                Lights[idx].center_point
        });
    }

    auto change = this->infer_requests_[CURR];
    this->infer_requests_[CURR] = this->infer_requests_[NEXT];
    this->infer_requests_[NEXT] = change;
    return result;
}

OpenvinoInfer::Resize OpenvinoInfer::letterBox(cv::Mat &src, const cv::Size2d &dst_size) {
    int w = src.cols;
    int h = src.rows;

    double r_w = (double)dst_size.width / w;
    double r_h = (double)dst_size.height / h;
    double r = std::min(r_w, r_h);

    int new_w = (int)(w * r);
    int new_h = (int)(h * r);

    cv::Mat resized_image;

    cv::resize(src, resized_image, cv::Size(new_w, new_h), cv::INTER_CUBIC);

    cv::Mat canvas(dst_size, CV_8UC3, cv::Scalar(128, 128, 128));

    int dx = (int)((dst_size.width - new_w) / 2.);
    int dy  =(int)((dst_size.height - new_h) / 2.);

    resized_image.copyTo(canvas(cv::Rect(dx, dy, new_w, new_h)));
//     cv::imshow("resized_image", canvas);
//     cv::waitKey(1);
    return {canvas, dx, dy};
}

void OpenvinoInfer::fitRec(std::vector<OpenvinoInfer::Light> &bboxes, cv::Size2d ori_size, cv::Size2d now_size) {
    double scale = std::max((double)ori_size.width / now_size.width, (double)ori_size.height / now_size.height);

    for (auto &bbox : bboxes) {
        bbox.box.x = (bbox.box.x - now_size.width / 2) * scale + ori_size.width / 2;
        bbox.box.y = (bbox.box.y - now_size.height / 2) * scale + ori_size.height / 2;
        bbox.box.width *= scale;
        bbox.box.height *= scale;
        bbox.center_point.x = (bbox.center_point.x - now_size.width / 2) * scale + ori_size.width / 2;
        bbox.center_point.y = (bbox.center_point.y - now_size.height / 2) * scale + ori_size.height / 2;
    }
}
