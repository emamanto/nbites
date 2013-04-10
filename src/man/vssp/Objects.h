#pragma once

#include <string>
#include "BallModel.pb.h"

class Scene;

// TemporalObject
class TemporalObject
{
public:
    TemporalObject(std::string type);
    TemporalObject(std::string type, int x_, int y_, float dist_, float bear_);

    inline std::string type() { return objectType; }
    inline int x() { return visualX; }
    inline int y() { return visualY; }
    inline float distance() { return visualDistance; }
    inline float bearing() { return visualBearing; }

    Scene* getScene() { return scene; }
    void setScene(Scene* scene_) { scene = scene_; }
    TemporalObject* next() { return nextObject; }
    void setNext(TemporalObject* next_) { nextObject = next_; }
    TemporalObject* previous() { return previousObject; }
    void setPrevious(TemporalObject* prev_) { previousObject = prev_; }

protected:
    std::string objectType;
    int visualX;
    int visualY;
    float visualDistance;
    float visualBearing;

    Scene* scene;
    TemporalObject* previousObject;
    TemporalObject* nextObject;
};

class TemporalBall : public TemporalObject
{
public:
    TemporalBall(const messages::VisionBall& vis);

    TemporalBall* next() { return nextObject; }
    void setNext(TemporalBall* next_) { nextObject = next_; }
    TemporalBall* previous() { return previousObject; }
    void setPrevious(TemporalBall* prev_) { previousObject = prev_; }

private:
    int visualRadius;
    int confidence;

    TemporalBall* previousObject;
    TemporalBall* nextObject;
};
