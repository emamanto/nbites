/**
 * The main Tool class!
 */

#pragma once

#include "EmptyTool.h"

#include "viewer/FieldViewer.h"

#include "ParticleSwarm.pb.h"

namespace tool {

class LocTool : public EmptyTool {
    Q_OBJECT;

public:
    LocTool(const char* title = "LocTool");
    ~LocTool();

public slots:
    void setUpModules();

 protected:
    viewer::FieldViewer fieldView;
};
}
