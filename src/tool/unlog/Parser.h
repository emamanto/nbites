#pragma once

#include "RoboGrams.h"
#include <vector>

namespace tool{
namespace unlog{

class MessageParser : public synchro::Thread {
public:
    static const uint32_t TOO_BIG_THRESHOLD = 2000000; // ~2MB

public:
    MessageParser();
    ~MessageParser();

    void readHeader();
    void openFile();
    bool readNextMessage();
    bool getPrev(uint32_t n);
    bool getPrev();

    virtual void run();

    uint32_t sizeOfLastNumMessages(uint32_t n) const;

    void setObjectToParseTo(Message::ptr newObject) {
        objectToParseTo = newObject;
    }

    void signalToParseNext() {
        this->signalToResume();
    }

private:
    void increaseBufferSizeTo(uint32_t new_size);
    uint32_t truncateNumberOfFramesToRewind(uint32_t n) const;

    template <typename T>
    T readValue() {
        T value;
        readIntoBuffer((char *)(&value), sizeof(value));
        return value;
    }

    bool readIntoBuffer(char* buffer, uint32_t num_bytes);

private:
    Message::ptr objectToParseTo;

    uint32_t current_message_size;
    //history of message sizes read, useful for rewinding
    std::vector<uint32_t> message_sizes;

    char* current_buffer;
    uint32_t current_buffer_size;

};

}
}
