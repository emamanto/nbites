#include "Objects.h"

TemporalObject::TemporalObject(std::string type) :
    objectType(type),
    scene(NULL),
    previousObject(NULL),
    nextObject(NULL)
{
}

TemporalObject::TemporalObject(std::string type, int x_, int y_,
                               float dist_, float bear_) :
    objectType(type),
    visualX(x_),
    visualY(y_),
    visualDistance(dist_),
    visualBearing(bear_),
    scene(NULL),
    previousObject(NULL),
    nextObject(NULL)
{
}

TemporalBall::TemporalBall(const messages::VisionBall& vis) :
    TemporalObject("ball",
                   vis.visual_x(),
                   vis.visual_y(),
                   vis.distance(),
                   vis.bearing()),
    visualRadius(int(vis.radius())),
    confidence(vis.confidence())
{
}
