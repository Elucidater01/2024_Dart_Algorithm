#include "Capture.hpp"

#include "function/function.hpp"

#include "CaptureInformation_generated.h"

Capture::Capture(std::string topic_name,
                  std::string frame_id,
                  std::string schema_name,
                  std::string schema_path,
                  std::string encoding,
                  std::string mcap_path) :
        topic_name_(std::move(topic_name)),
        frame_id_(std::move(frame_id)),
        schema_name_(std::move(schema_name)),
        schema_path_(std::move(schema_path)),
        encoding_(std::move(encoding)),
        mcap_path_(std::move(mcap_path)) {}

void Capture::update(CaptureInfo &info) {
    this->builder_.Clear();

    auto cd = foxglove::CreateCarData(
            this->builder_,
            info.cd.mode,
            info.cd.status,
            info.cd.number,
            info.cd.dune);

    auto vd = foxglove::CreateVisionData(
            this->builder_,
            info.vd.yaw_error,
            info.vd.target_status);

    auto encoding = this->builder_.CreateString(this->encoding_);
    auto frame_id = this->builder_.CreateString(this->frame_id_);
    auto data_vec = this->builder_.CreateVector(info.img.data, info.img.total() * info.img.elemSize());
    auto timestamp = function::getNowTimestamp();

    auto raw_image = foxglove::CreateRawImage(
            this->builder_,
            &timestamp,
            frame_id,
            info.img.cols,
            info.img.rows,
            encoding,
            info.img.step,
            data_vec);


    auto capture_info_builder = foxglove::CaptureInformationBuilder(this->builder_);
    capture_info_builder.add_cd(cd);
    capture_info_builder.add_vd(vd);
    capture_info_builder.add_raw_image(raw_image);

    auto _capture_info = capture_info_builder.Finish();

    this->builder_.Finish(_capture_info);

    auto data_ptr = this->builder_.GetBufferPointer();
    auto data_size = this->builder_.GetSize();

    std::shared_ptr<std::byte>data_msg(new std::byte[data_size]);
    std::memcpy(data_msg.get(), data_ptr, data_size);

    mcap::Message msg;
    msg.channelId = this->chatter_->id;
    msg.sequence = 1;
    msg.logTime = function::nanosecondsSinceEpoch();
    msg.publishTime = msg.logTime;
    msg.data = data_msg.get();
    msg.dataSize = data_size;

    this->writer_.write(msg);
}

Capture::~Capture() {
    std::cout << "Capture end." << std::endl;
    this->writer_.close();
}

void Capture::init() {
    function::tryToCreateAimFile(this->mcap_path_);

    auto status = this->writer_.open(this->mcap_path_, mcap::McapWriterOptions("flatbuffer"));

    if (!status.ok()) {
        std::cerr << "[" << this->mcap_path_ << "] Failed to open MCAP file for writing: " << status.message << "\n";
        return;
    }

    auto encoding = function::getFileContents(this->schema_path_);

//    mcap::Schema schema("foxglove.RawImage", "flatbuffer", encoding);
    this->schema_ = std::make_unique<mcap::Schema>(this->schema_name_, "flatbuffer", encoding);
    this->writer_.addSchema(*this->schema_.get());

    this->chatter_ = std::make_unique<mcap::Channel>(this->topic_name_, "flatbuffer", this->schema_.get()->id);
    this->writer_.addChannel(*this->chatter_.get());
}

void Capture::finish() {
    this->writer_.close();
}

void Capture::setMacpPath(std::string mcap_path) {
    this->mcap_path_ = std::move(mcap_path);
}