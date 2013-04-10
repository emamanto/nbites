/*
 * @class VisionSimGUI
 *
 * Main GUI for the simulator; can be added to the QTool as a tab widget.
 * Provides image viewers, a control panel, and a world view so that the
 * model can be controlled and the results viewed in the projected images.
 *
 * @author Lizzie Mamantov
 */

#pragma once

#include "ImageView.h"
#include "WorldView.h"
#include "Controls.h"

namespace tool{
namespace visionsim{

class VisionSimGUI  : public QWidget {
    Q_OBJECT;

public:
    VisionSimGUI(ImageView& top, ImageView& bottom, WorldView& world,
                 Controls& cont, QWidget* parent = 0);

private:
    ImageView& topImageView;
    ImageView& bottomImageView;
    WorldView& worldView;
    Controls& controls;
};

}
}
