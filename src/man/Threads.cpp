#include "Threads.h"

namespace man {

CognitionThread::CognitionThread() : Thread("cognition"),
                                     cognitionDiagram(),
                                     logger()
{
    cognitionDiagram.addModule(logger);
}

void CognitionThread::run()
{
    while (Thread::running)
    {
        cognitionDiagram.run();
    }
}

}
