#ifndef DUMP_HPP_
#define DUMP_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "mcap/reader.hpp"

#include "CaptureInfomation_generated.h"

#include "lockfree/LockFree.hpp"
#include "function/function.hpp"
#include "global/Struct.hpp"
using mcap::ByteOffset;

static std::string to_string(const std::string &arg) {
    return arg;
}
static std::string to_string(std::string_view arg) {
    return std::string(arg);
}
static std::string to_string(const char* arg) {
    return std::string(arg);
}
template <typename... T>
[[nodiscard]] static std::string StrCat(T&&... args) {
    using ::to_string;
    using std::to_string;
    return ("" + ... + to_string(std::forward<T>(args)));
}

static std::string ToHex(uint8_t byte) {
    std::string result{2, '\0'};
    result[0] = "0123456789ABCDEF"[(uint8_t(byte) >> 4)  &0x0F];
    result[1] = "0123456789ABCDEF"[uint8_t(byte)  &0x0F];
    return result;
}

inline std::string ToString(const mcap::KeyValueMap &map) {
    std::stringstream ss;
    ss << "{";
    for (const auto &[key, value] : map) {
        if (ss.tellg() > 1) {
            ss << ", ";
        }
        ss << "\"" << key << "\": \"" << value << "\"";
    }
    ss << "}";
    return ss.str();
}

inline std::string ToString(const std::unordered_map<uint16_t, uint64_t> &map) {
    if (map.size() > 8) {
        return StrCat("<", map.size(), " entries>");
    }

    std::stringstream ss;
    ss << "{";
    for (const auto &[key, value] : map) {
        if (ss.tellg() > 1) {
            ss << ", ";
        }
        ss << key << ": " << value;
    }
    ss << "}";
    return ss.str();
}

inline std::string ToString(const std::vector<std::pair<mcap::Timestamp, ByteOffset>> &pairs) {
    if (pairs.size() > 8) {
        return StrCat("<", pairs.size(), " entries>");
    }

    std::stringstream ss;
    ss << "[";
    for (const auto &[timestamp, offset] : pairs) {
        if (ss.tellg() > 1) {
            ss << ", ";
        }
        ss << "{" << timestamp << ", " << offset << "}";
    }
    ss << "]";
    return ss.str();
}

inline std::string ToString(const mcap::Header &header) {
    return StrCat("[Header] profile=", header.profile, ", library=", header.library);
}

inline std::string ToString(const mcap::Footer &footer) {
    return StrCat("[Footer] summary_start=", footer.summaryStart,
                  ", summary_offset_start=", footer.summaryOffsetStart,
                  ", summary_crc=", footer.summaryCrc);
}

inline std::string ToString(const mcap::Schema &schema) {
    return StrCat("[Schema] id=", schema.id, ", name=", schema.name, ", encoding=", schema.encoding,
                  ", data=<", schema.data.size(), " bytes>");
}

inline std::string ToString(const mcap::Channel &channel) {
    return StrCat("[Channel] id=", channel.id, ", schema_id=", channel.schemaId,
                  ", topic=", channel.topic, ", message_encoding=", channel.messageEncoding,
                  ", metadata=", ToString(channel.metadata));
}

inline std::string ToString(const mcap::Message &message) {
    return StrCat("[Message] channel_id=", message.channelId, ", sequence=", message.sequence,
                  ", publish_time=", message.publishTime, ", log_time=", message.logTime, ", data=<",
                  message.dataSize, " bytes>");
}

inline std::string ToString(const mcap::Chunk &chunk) {
    return StrCat("[Chunk] message_start_time=", chunk.messageStartTime,
                  ", message_end_time=", chunk.messageEndTime,
                  ", uncompressed_size=", chunk.uncompressedSize,
                  ", uncompressed_crc=", chunk.uncompressedCrc, ", compression=", chunk.compression,
                  ", data=<", chunk.compressedSize, " bytes>");
}

inline std::string ToString(const mcap::MessageIndex &messageIndex) {
    return StrCat("[MessageIndex] channel_id=", messageIndex.channelId,
                  ", records=", ToString(messageIndex.records));
}

inline std::string ToString(const mcap::ChunkIndex &chunkIndex) {
    return StrCat(
            "[ChunkIndex] message_start_time=", chunkIndex.messageStartTime,
            ", message_end_time=", chunkIndex.messageEndTime,
            ", chunk_start_offset=", chunkIndex.chunkStartOffset, ", chunk_length=", chunkIndex.chunkLength,
            ", message_index_offsets=", ToString(chunkIndex.messageIndexOffsets),
            ", message_index_length=", chunkIndex.messageIndexLength,
            ", compression=", chunkIndex.compression, ", compressed_size=", chunkIndex.compressedSize,
            ", uncompressed_size=", chunkIndex.uncompressedSize);
}

inline std::string ToString(const mcap::Attachment &attachment) {
    return StrCat("[Attachment] log_time=", attachment.logTime,
                  ", create_time=", attachment.createTime, ", name=", attachment.name,
                  ", media_type=", attachment.mediaType, ", data=<", attachment.dataSize,
                  " bytes>, crc=", attachment.crc);
}

inline std::string ToString(const mcap::AttachmentIndex &attachmentIndex) {
    return StrCat("[AttachmentIndex] offset=", attachmentIndex.offset,
                  ", length=", attachmentIndex.length, ", log_time=", attachmentIndex.logTime,
                  ", create_time=", attachmentIndex.createTime,
                  ", data_size=", attachmentIndex.dataSize, ", name=", attachmentIndex.name,
                  ", media_type=", attachmentIndex.mediaType);
}

inline std::string ToString(const mcap::Statistics &statistics) {
    return StrCat(
            "[Statistics] message_count=", statistics.messageCount,
            ", schema_count=", statistics.schemaCount, ", channel_count=", statistics.channelCount,
            ", attachment_count=", statistics.attachmentCount,
            ", metadata_count=", statistics.metadataCount, ", chunk_count=", statistics.chunkCount,
            ", message_start_time=", statistics.messageStartTime,
            ", message_end_time=", statistics.messageEndTime,
            ", channel_message_counts=", ToString(statistics.channelMessageCounts));
}

inline std::string ToString(const mcap::Metadata &metadata) {
    return StrCat("[Metadata] name=", metadata.name, ", metadata=", ToString(metadata.metadata));
}

inline std::string ToString(const mcap::MetadataIndex &metadataIndex) {
    return StrCat("[MetadataIndex] offset=", metadataIndex.offset, ", length=", metadataIndex.length,
                  ", name=", metadataIndex.name);
}

inline std::string ToString(const mcap::SummaryOffset &summaryOffset) {
    return StrCat("[SummaryOffset] group_opcode=", mcap::OpCodeString(summaryOffset.groupOpCode),
                  " (0x", ToHex(uint8_t(summaryOffset.groupOpCode)),
                  "), group_start=", summaryOffset.groupStart,
                  ", group_length=", summaryOffset.groupLength);
}

inline std::string ToString(const mcap::DataEnd &dataEnd) {
    return StrCat("[DataEnd] data_section_crc=", dataEnd.dataSectionCrc);
}

inline std::string ToString(const mcap::Record &record) {
    return StrCat("[Unknown] opcode=0x", ToHex(uint8_t(record.opcode)), ", data=<", record.dataSize,
                  " bytes>");
}

inline std::string ToStringRaw(const mcap::Record &record) {
    return StrCat("[", mcap::OpCodeString(record.opcode), "] opcode=0x",
                  ToHex(uint8_t(record.opcode)), ", data=<", record.dataSize, " bytes>");
}

inline void DumpRaw(mcap::IReadable &dataSource) {
    // Iterate all of the raw records in the data source, ignoring the magic bytes
    // at the beginning and end of the file. This will not print the contents of
    // chunk records since chunks are not parsed or decompressed
    mcap::RecordReader reader{dataSource, 8, dataSource.size() - 8};

    bool running = true;
    while (running) {
        const auto record = reader.next();
        if (record.has_value()) {
            std::cout << ToStringRaw(record.value()) << "\n";
        } else {
            running = false;
        }

        if (!reader.status().ok()) {
            std::cout << "! " << reader.status().message << "\n";
        }
    }
}

inline void Dump(mcap::IReadable &dataSource) {
    // Iterate and parse all of the records in the data source, ignoring the magic
    // bytes in the header. This will print the contents of chunk records as well
    mcap::TypedRecordReader reader{dataSource, 8};
    bool inChunk = false;

    reader.onHeader = [](const mcap::Header &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onFooter = [](const mcap::Footer &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onSchema = [&](const mcap::SchemaPtr recordPtr, ByteOffset, std::optional<ByteOffset>) {
        std::cout << (inChunk ? "  " : "") << ToString(*recordPtr) << "\n";
    };
    reader.onChannel = [&](const mcap::ChannelPtr recordPtr, ByteOffset, std::optional<ByteOffset>) {
        std::cout << (inChunk ? "  " : "") << ToString(*recordPtr) << "\n";
    };
    reader.onMessage = [&](const mcap::Message &record, ByteOffset, std::optional<ByteOffset>) {
        std::cout << (inChunk ? "  " : "") << ToString(record) << "\n";
    };
    reader.onChunk = [&](const mcap::Chunk &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
        inChunk = true;
    };
    reader.onMessageIndex = [](const mcap::MessageIndex &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onChunkIndex = [](const mcap::ChunkIndex &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onAttachment = [](const mcap::Attachment &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onAttachmentIndex = [](const mcap::AttachmentIndex &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onStatistics = [](const mcap::Statistics &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onMetadata = [](const mcap::Metadata &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onMetadataIndex = [](const mcap::MetadataIndex &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onSummaryOffset = [](const mcap::SummaryOffset &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onDataEnd = [](const mcap::DataEnd &record, ByteOffset) {
        std::cout << ToString(record) << "\n";
    };
    reader.onUnknownRecord = [](const mcap::Record &record, ByteOffset, std::optional<ByteOffset>) {
        std::cout << ToString(record) << "\n";
    };
    reader.onChunkEnd = [&](ByteOffset) {
        inChunk = false;
    };

    bool running = true;
    while (running) {
        running = reader.next();
        if (!reader.status().ok()) {
            std::cerr << "! " << reader.status().message << "\n";
        }
    }
}

inline void DumpMessages(const std::string &name, mcap::IReadable &dataSource, LfStack<FromData> &f_c) {
    mcap::McapReader reader;
    auto status = reader.open(dataSource);
    if (!status.ok()) {
        std::cerr << "! " << status.message << "\n";
        return;
    }

    auto onProblem = [](const mcap::Status &problem) {
        std::cerr << "! " << problem.message << "\n";
    };

    auto messages = reader.readMessages(onProblem);

    std::string img_path = "../data/" + function::getLocalTime() + "/";
    function::tryToCreateAimFile(img_path);
    std::vector img_format = {cv::IMWRITE_PNG_COMPRESSION, 0};

    cv::namedWindow("capture_info", cv::WINDOW_NORMAL);
    for (const auto &msgView : messages) {
//        const mcap::Channel &channel = *msgView.channel;
//        std::cout << "[" << channel.topic << "] " << ToString(msgView.message) << "\n";

        auto _f_c = foxglove::GetCaptureInfomation(msgView.message.data);

        FromData _f_c_info;

        _f_c_info.cd = std::move(CarData{
                .mode = _f_c->cd()->mode(),
                .ready_to_fire = _f_c->cd()->ready_to_fire(),
                .bullet_id = _f_c->cd()->bullet_id(),
                .yaw = _f_c->cd()->yaw(),
                .pitch = _f_c->cd()->pitch(),
                .roll = _f_c->cd()->roll(),
                .bullet_speed = _f_c->cd()->bullet_speed(),
                .my_color = _f_c->cd()->my_color(),
                .dune = _f_c->cd()->dune()
        });

        cv::Mat raw_image(cv::Size(_f_c->raw_image()->width(), _f_c->raw_image()->height()),
                                        CV_8UC3,
                                       (void*)_f_c->raw_image()->data()->data(),
                                        _f_c->raw_image()->step());

        _f_c_info.img = std::move(raw_image);

        _f_c_info.timestamp = *_f_c->raw_image()->timestamp();
        cv::imshow("capture_info", _f_c_info.img);
        auto key = cv::waitKey(0);

        static int cnt = 0;
        if (key == 'q' || key == 'Q') {
            cv::imwrite(img_path + name + "_" + std::to_string(++cnt) + ".png", _f_c_info.img, img_format);
        }

        f_c.pop();
        f_c.push(_f_c_info);
   }
    reader.close();
}

#endif //DUMP_HPP_