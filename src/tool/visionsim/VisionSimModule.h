/*
 * @class VisionSimModule
 *
 * The main module that starts up the model and the GUI; can be added to the
 * Tool's diagram to run on call. This is both a QObject and a Module, so it
 * interfaces between RoboGrams and Qt stuff.
 *
 * @author Lizzie Mamantov
 */

#pragma once

#include <QObject>
#include "RoboGrams.h"
#include "Image.h"
#include "Controls.h"
#include "WorldView.h"
#include "ImageView.h"
//#include "VisionSimGUI.h"

namespace tool{
namespace visionsim{

class VisionSimModule :  public QObject,
                         public portals::Module
{
    Q_OBJECT;

public:
    VisionSimModule();

public slots:
    void runSubModules();

protected:
    // The required module method
    virtual void run_();

    portals::RoboGram subdiagram;
    Controls controls;
    WorldView worldView;
    Image topImage;
    Image bottomImage;
    ImageView topImageView;
    ImageView bottomImageView;
};

}
}
