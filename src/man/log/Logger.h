#pragma once

#include "RoboGrams.h"
#include "IOExceptions.h"
#include "Common.h"
#include "synchro.h"
#include <string>
#include <fcntl.h>
#include <aio.h>
#include <stdint.h>
#include <stdexcept>

namespace man {
namespace log {

static const std::string PATH = "/home/nao/nbites/frames";

class Logger : public synchro::Thread {

    enum OpenFlags {
        //O_APPEND is crucial if you want to use aio_write on the file
        //descriptor you open
        NEW = O_WRONLY | O_CREAT | O_TRUNC | O_APPEND,
        EXISTING = O_RDONLY
    };

    enum OpenPermissions {
        PERMISSIONS_ALL = S_IRWXU | S_IRWXG | S_IRWXO
    };

public:
    Logger(std::string name);

    virtual ~Logger();

    void writeToLog();
    void writeHeader();
    void run();
    void openCommunicationChannel() throw (file_exception);
    bool fileOpened() const;
    void closeCommunicationChannel();
    bool writingInProgress() const;
    void waitForWriteToFinish() throw (std::runtime_error);
    void writeCharBuffer(const char* buffer, uint32_t size);

private:
    std::string file_name;
    struct aiocb control_block;
    bool is_open;
    std::string write_buffer;
    int file_descriptor;
};

}
}
