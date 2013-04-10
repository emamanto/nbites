/**
 * @class ImageView
 *
 * A QWidget that draws all of the objects in an Image as a simulation
 * of an actual robot image. Is a friend of the Image class, so it can
 * access all of the info about visual objects directly.
 * @see Image.h for info about the objects
 *
 * @author Lizzie Mamantov
 */

#pragma once

#include <QWidget>
#include <QPainter>

#include "ImageConstants.h"
#include "Image.h"
#include "RoboGrams.h"
#include "BallModel.pb.h"
#include "VisionField.pb.h"

namespace tool{
namespace visionsim{

class ImageView : public QWidget,
                  public portals::Module
{
    Q_OBJECT;

public:
    ImageView(QWidget* parent = 0);
    ~ImageView() {};

    portals::InPortal<messages::VisionBall> ballIn;
    // Set this up!
    //portals::InPortal<messages::VisionField> fieldIn;
protected:
    virtual void run_();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    // Handles all of the painting
    void paintEvent(QPaintEvent* event);
};

}
}
