/**
 * @class DataProcessingTool
 * Version of QTool for collecting vision data over a log.
 *
 *
 */

#pragma once

#include "EmptyQTool.h"

#include "data/DataLoader.h"
#include "viewer/VisionViewer.h"

namespace qtool {

class DataProcessingTool : public EmptyQTool {

    Q_OBJECT

public:
    DataProcessingTool();
    ~DataProcessingTool() {};

private:
    data::DataLoader* dataLoader;
    viewer::VisionViewer* visionViewer;
    QPushButton* start;
};

}
