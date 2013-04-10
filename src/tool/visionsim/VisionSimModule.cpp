#include "VisionSimModule.h"
#include <iostream>

using namespace portals;

namespace tool{
namespace visionsim{

VisionSimModule::VisionSimModule() : QObject(),
                                     Module(),
                                     controls(),
                                     worldView(),
                                     topImage(TOP),
                                     bottomImage(BOTTOM),
                                     topImageView(),
                                     bottomImageView()
{
    worldView.worldIn.wireTo(&controls.worldOut);
    topImage.worldIn.wireTo(&controls.worldOut);
    bottomImage.worldIn.wireTo(&controls.worldOut);
    topImageView.ballIn.wireTo(&topImage.ballOut);
    bottomImageView.ballIn.wireTo(&bottomImage.ballOut);

    subdiagram.addModule(controls);
    subdiagram.addModule(worldView);
    subdiagram.addModule(topImage);
    subdiagram.addModule(bottomImage);
    subdiagram.addModule(topImageView);
    subdiagram.addModule(bottomImageView);

    connect(&controls, SIGNAL(dataChanged()), this, SLOT(runSubModules));
}

void VisionSimModule::runSubModules()
{
    subdiagram.run();
}

void VisionSimModule::run_()
{
    subdiagram.run();
}

}
}
