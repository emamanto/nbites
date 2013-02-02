#pragma once

#include "RoboGrams.h"
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
    void addLogger(portals::OutPortal<T>*, std::string name);

protected:
    void run_();
};

template<class T>
void LogModule::addLogger(portals::OutPortal<T>*, std::string name)
{
}

}
}
