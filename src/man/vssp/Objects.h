#pragma once

#include <string>

class Scene;

// TemporalObject
class TemporalObject
{
public:
    TemporalObject(std::string type);
    TemporalObject(std::string type, int x_, int y_, int dist_, int bear_);

    inline std::string type() { return objectType; }
    inline int x() { return visualX; }
    inline int y() { return visualY; }
    inline float distance() { return visualDistance; }
    inline float bearing() { return visualBearing; }

    Scene* getScene() { return scene; }
    void setScene(Scene* scene_) { scene = scene_; }
    TemporalObject* next() { return nextObject; }
    TemporalObject* previous() { return previousObject; }

private:
    std::string objectType;
    int visualX;
    int visualY;
    float visualDistance;
    float visualBearing;

    Scene* scene;
    TemporalObject* previousObject;
    TemporalObject* nextObject;
};
