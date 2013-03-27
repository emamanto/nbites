#include "MemoryModule.h"

MemoryModule::MemoryModule() : portals::Module(),
                               memory(5)
{
}

void MemoryModule::run_()
{
    input.latch();
    Scene latest;
    latest.addObject(TemporalBall(input.message()));
    memory.addScene(latest);
}
