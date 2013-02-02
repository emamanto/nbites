#include "Threads.h"
#include <string>

namespace man {

CognitionThread::CognitionThread() : Thread("cognition"),
                                     cognitionDiagram()
{
}

void CognitionThread::run()
{
    while (Thread::running)
    {
        cognitionDiagram.run();
    }
}

SensorsThread::SensorsThread(boost::shared_ptr<AL::ALBroker> broker) :
    Thread("sensors"),
    sensorsDiagram(),
    logger(),
    sensors(broker)
{
    logger.addLogger<messages::JointAngles>(&sensors.jointsOutput_,
                                            std::string("joints"));
    sensorsDiagram.addModule(sensors);
    sensorsDiagram.addModule(logger);
}

void SensorsThread::run()
{
    while (Thread::running)
    {
        sensorsDiagram.run();
    }
}

}
