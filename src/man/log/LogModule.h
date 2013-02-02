#pragma once

#include "RoboGrams.h"
#include "Logger.h"
#include <string>
#include <fstream>
#include <vector>

namespace man {
namespace log {

class LogModule : public portals::Module {

public:
	LogModule();
	virtual ~LogModule();

    template<class T>
    void addLogger(portals::OutPortal<T>* port, std::string name)
    {
    }

protected:
    void run_();
    std::vector<LoggerBase> logs;
};

}
}
