#include "Logger.h"
#include <iostream>
#include <string.h>
#include <errno.h>

namespace man{
namespace log{

Logger::Logger(std::string name) : Thread(name),
                                   file_name(PATH + name)
{
    memset(&control_block, 0, sizeof(control_block));
}

Logger::~Logger() {
    this->stop();
    this->waitForThreadToFinish();
}

void Logger::run() {
    while (running) {
        if (!fileOpened()) {
            try {
                openCommunicationChannel();
            } catch (io_exception& io_exception) {
                std::cout << file_name << ": " << io_exception.what() << " ";
                return;
            }
            std::cout << "Writing header to " << file_name << std::endl;
            this->writeHeader();
        }
        this->waitForSignal();
        this->writeToLog();
        this->waitForWriteToFinish();
    }
}

void Logger::writeHeader() {
}

void Logger::writeToLog() {
}

void Logger::openCommunicationChannel() throw (file_exception) {

    file_descriptor = open(file_name.c_str(), NEW, PERMISSIONS_ALL);

    if (file_descriptor < 0) {
        throw file_exception(file_exception::CREATE_ERR);
    }

    is_open = true;
}

bool Logger::fileOpened() const {
    return is_open;
}

void Logger::closeCommunicationChannel() {
    close(file_descriptor);
    file_descriptor = -1;
    is_open = false;
}

bool Logger::writingInProgress() const {
    return aio_error(&control_block) == EINPROGRESS;
}

void Logger::waitForWriteToFinish() throw (std::runtime_error) {

    const struct aiocb* cblist[] = { &control_block };
    int result = aio_suspend(cblist, 1, NULL);

    if (result != 0) {
        throw_errno(errno);
    }
}

//yields before the other write is done!
void Logger::writeCharBuffer(const char* buffer, uint32_t size) {
    if (!fileOpened()) {
        std::cout<<"Cannot write to an unopened channel!" << std::endl;
        return;
    }

    //const_casting is bad(!!!) but aio_write demands a non-const buffer
    control_block.aio_fildes = file_descriptor;
    control_block.aio_buf = const_cast<char *>(buffer);
    control_block.aio_nbytes = size;
    control_block.aio_sigevent.sigev_notify = SIGEV_NONE;
    int result = aio_write(&control_block);

    if (result == -1) {
        std::cout<<"AIO write enqueue failed with error " << strerror(errno) << std::endl;
    }

    while(writingInProgress()) {
        pthread_yield();
    }

    int bytes_written = aio_return(&control_block);

    if (bytes_written == -1) {
        std::cout<< "AIO write failed with error " << strerror(aio_error(&control_block)) << std::endl;
        this->closeCommunicationChannel();
    } else {
        if ((uint32_t) (bytes_written) < size) {
            writeCharBuffer(buffer + bytes_written, size - bytes_written);
        }
    }
}
}
}
