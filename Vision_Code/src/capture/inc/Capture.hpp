#ifndef CAPTURE_HPP_
#define CAPTURE_HPP_

#include <flatbuffers/flatbuffers.h>

#include "mcap/writer.hpp"

#include "global/Struct.hpp"

class Capture {
public:
    Capture(std::string topic_name,
            std::string frame_id,
            std::string schema_name,
            std::string schema_path,
            std::string encoding,
            std::string mcap_path = "");
    Capture() = delete;
    ~Capture();
    void init();
    void finish();
    void setMacpPath(std::string mcap_path);
    void update(CaptureInfo &info);
private:
    mcap::McapWriter writer_;
    std::unique_ptr<mcap::Channel> chatter_;
    std::unique_ptr<mcap::Schema> schema_;
    flatbuffers::FlatBufferBuilder builder_;
    std::string topic_name_;
    std::string frame_id_;
    std::string schema_name_;
    std::string schema_path_;
    std::string mcap_path_;
    std::string encoding_;
};

#endif