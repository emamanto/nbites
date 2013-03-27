#include "MemoryModule.h"

MemoryModule::MemoryModule() : portals::Module(),
                               memory(5)
{
}

void MemoryModule::run_()
{
    input.latch();
    memory.addNewScene();
    memory.end()->addObject(TemporalBall(input.message()));
}
