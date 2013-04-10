/**
 * @class WorldView
 *
 * Widget for drawing the state of the world. Draws an overhead view of the
 * field based on the world model it contains. Can draw lines, posts, the
 * ball (movable) and the robot (movable).
 *
 * @author Lizzie Mamantov
 */

#pragma once

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "WorldConstants.h"
#include "WorldModel.pb.h"
#include "RoboGrams.h"

namespace tool{
namespace visionsim{

class WorldView : public QWidget,
                  public portals::Module
{
    Q_OBJECT;

public:
    WorldView(QWidget* parent = 0);

    portals::InPortal<messages::WorldModel> worldIn;

protected:
    virtual void run_();

    // Keeps the widget the right size
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    // Handles all of the drawing
    void paintEvent(QPaintEvent* event);
};

}
}
