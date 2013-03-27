#pragma once

#include "RoboGrams.h"
#include "Scenes.h"
#include "VisionBall.pb.h"

class MemoryModule : public portals::Module
{
public:
    MemoryModule();

    portals::InPortal<messages::VisionBall> input;

private:
    virtual void run_();

    SceneBuffer memory;
};
