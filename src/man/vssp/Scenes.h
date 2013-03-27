#pragma once

#include <boost/circular_buffer.hpp>
#include <string>
#include <map>
#include "Objects.h"

// Scene
class Scene
{
public:
    void addObject(TemporalObject obj);

    inline int stamp() { return framestamp; }
    inline void setStamp(int stamp) { framestamp = stamp; }

private:
    int framestamp;
    std::multimap<std::string, TemporalObject> objects;
};

typedef boost::circular_buffer<Scene>::iterator SceneIt;

// SceneBuffer
class SceneBuffer
{
public:
    explicit SceneBuffer(int capacity) : cb(capacity),
                                         framecount(1) {};

    bool addScene(Scene scene);
    bool addNewScene();

    SceneIt begin() { return cb.begin(); }
    SceneIt end() { return cb.end(); }

private:
    boost::circular_buffer<Scene> cb;
    int framecount;
};
