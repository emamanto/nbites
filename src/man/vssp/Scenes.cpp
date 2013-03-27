#include "Scenes.h"
#include <iostream>

void Scene::addObject(TemporalObject obj)
{
    obj.setScene(this);
    objects.insert(std::pair<std::string, TemporalObject>(obj.type(), obj));
}

bool SceneBuffer::addScene(Scene scene)
{
    bool replacement = cb.full() ? true : false;
    framecount++;
    scene.setStamp(framecount);
    cb.push_back(scene);
    return replacement;
}

bool SceneBuffer::addNewScene()
{
    return addScene(Scene());
}
