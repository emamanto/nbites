/*
 * @class Controls
 *
 * The Controls class provides a GUI for moving a robot and ball around
 * a field. This involves lots of spin boxes so that the user can type
 * or click to a certain destination.
 *
 * @author Lizzie Mamantov
 *
 */

#pragma once

#include <QSpinBox>
#include "RoboGrams.h"
#include "WorldModel.pb.h"

namespace tool{
namespace visionsim{

class Controls : public QWidget,
                 public portals::Module
{
    Q_OBJECT;

public:
    Controls(QWidget* parent = 0);

    portals::OutPortal<messages::WorldModel> worldOut;

signals:
    void dataChanged();

protected slots:
    void signalToRun();

protected:
    virtual void run_();

    // The various spin boxes used to control values
    QSpinBox robotX;
    QSpinBox robotY;
    QSpinBox robotH;
    QSpinBox headYaw;
    QSpinBox ballX;
    QSpinBox ballY;
};

}
}
