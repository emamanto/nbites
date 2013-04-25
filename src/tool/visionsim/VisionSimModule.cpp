#include "VisionSimModule.h"
#include <iostream>

using namespace portals;

namespace tool{
namespace visionsim{

VisionSimModule::VisionSimModule() : QWidget(),
                                     Module(),
                                     controls(),
                                     worldView(),
                                     topImage(TOP),
                                     bottomImage(BOTTOM),
                                     topImageView(),
                                     bottomImageView()
{
    //Backend
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

    // GUI
    imageTabs.addTab(&topImageView, "TOP");
    imageTabs.addTab(&bottomImageView, "BOTTOM");
    mainLayout.addWidget(&imageTabs);

    sideLayout.addWidget(&controls);
    sideLayout.addWidget(&worldView);
    controller.setLayout(&sideLayout);
    mainLayout.addWidget(&controller);

    setLayout(&mainLayout);

    connect(&controls, SIGNAL(dataChanged()), this, SLOT(runSubModules()));
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
