#include "Controls.h"
#include <QGridLayout>
#include <QLabel>
#include <math.h>
#include "WorldConstants.h"
#include "ImageConstants.h"

namespace tool{
namespace visionsim{

Controls::Controls(QWidget * parent) : QWidget(parent),
                                       worldOut(base()),
                                       robotX(this),
                                       robotY(this),
                                       robotH(this),
                                       headYaw(this),
                                       ballX(this),
                                       ballY(this)
{
    // Grid Layout organizes the spin boxes nicely
    QGridLayout* layout = new QGridLayout(this);

    /*
     * The min/max values come from the constant files constraining
     * the world.
     * @see WorldConstants.h
     * @see ImageConstants.h
     */

    // Robot X
    QLabel* xLabel = new QLabel(tr("Robot X"));
    robotX.setRange(GREEN_LEFT_EDGE_X, GREEN_RIGHT_EDGE_X);
    robotX.setSingleStep(10);
    layout->addWidget(xLabel,  0, 0);
    layout->addWidget(&robotX, 0, 1);

    connect(&robotX, SIGNAL(valueChanged(int)), this,
            SLOT(signalToRun()));

    // Robot Y
    QLabel* yLabel = new QLabel(tr("Robot Y"));
    robotY.setRange(GREEN_BOTTOM_Y, GREEN_TOP_Y);
    robotY.setSingleStep(10);
    layout->addWidget(yLabel,  1, 0);
    layout->addWidget(&robotY, 1, 1);

    connect(&robotY, SIGNAL(valueChanged(int)), this,
            SLOT(signalToRun()));

    // Robot H
    QLabel* hLabel = new QLabel(tr("Robot Heading"));
    robotH.setRange(-180, 180);
    layout->addWidget(hLabel,  2, 0);
    layout->addWidget(&robotH, 2, 1);

    connect(&robotH, SIGNAL(valueChanged(int)), this,
            SLOT(signalToRun()));

    // Head yaw
    QLabel* yawLabel = new QLabel(tr("Head Yaw"));
    headYaw.setRange((int)MIN_HEAD_YAW, (int)MAX_HEAD_YAW);
    layout->addWidget(yawLabel, 3, 0);
    layout->addWidget(&headYaw, 3, 1);

    connect(&headYaw, SIGNAL(valueChanged(int)), this,
            SLOT(signalToRun()));

    // Ball X
    QLabel* xBallLabel = new QLabel(tr("Ball X"));
    ballX.setRange(GREEN_LEFT_EDGE_X, GREEN_RIGHT_EDGE_X);
    ballX.setSingleStep(10);
    layout->addWidget(xBallLabel, 5, 0);
    layout->addWidget(&ballX,     5, 1);

    connect(&ballX, SIGNAL(valueChanged(int)), this,
            SLOT(signalToRun()));

    // Ball Y
    QLabel* yBallLabel = new QLabel(tr("Ball Y"));
    ballY.setRange(GREEN_BOTTOM_Y, GREEN_TOP_Y);
    ballY.setSingleStep(10);
    layout->addWidget(yBallLabel, 6, 0);
    layout->addWidget(&ballY,     6, 1);

    connect(&ballY, SIGNAL(valueChanged(int)), this,
            SLOT(signalToRun()));

    this->setLayout(layout);
}

void Controls::run_()
{
    portals::Message<messages::WorldModel> msg(0);

    msg.get()->set_my_x(robotX.value());
    msg.get()->set_my_y(robotY.value());
    msg.get()->set_my_h(robotH.value());
    msg.get()->set_head_yaw(headYaw.value());
    float x_dist = robotX.value() - ballX.value();
    float y_dist = robotY.value() - ballY.value();
    msg.get()->set_ball_dist(sqrt(pow(x_dist, 2) + pow(y_dist, 2)));
    msg.get()->set_ball_bearing(atan2(y_dist, x_dist));
}

void Controls::signalToRun()
{
    emit dataChanged();
}

}
}
