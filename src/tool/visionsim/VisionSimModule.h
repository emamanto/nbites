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

#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include "RoboGrams.h"
#include "Image.h"
#include "Controls.h"
#include "WorldView.h"
#include "ImageView.h"
//#include "VisionSimGUI.h"

namespace tool{
namespace visionsim{

class VisionSimModule :  public QWidget,
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

    // Backend
    portals::RoboGram subdiagram;
    Image topImage;
    Image bottomImage;

    // GUI
    Controls controls;
    WorldView worldView;
    ImageView topImageView;
    ImageView bottomImageView;

    QTabWidget imageTabs;
    QWidget controller;
    QHBoxLayout mainLayout;
    QVBoxLayout sideLayout;
};

}
}
