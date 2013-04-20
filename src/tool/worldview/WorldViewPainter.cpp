#include "WorldViewPainter.h"

namespace tool {
namespace worldview {

static const int PARTICLE_WIDTH = 8;

WorldViewPainter::WorldViewPainter(QWidget* parent, float scaleFactor_) :
    PaintField(parent, scaleFactor_),
    shouldPaintLocation(true)
{
}

void WorldViewPainter::paintLocationAction(bool state) {

    shouldPaintLocation = state;
    repaint();
}

void WorldViewPainter::paintEvent(QPaintEvent* event)
{
    PaintField::paintEvent(event);

    if(shouldPaintLocation) {
        // Paint actual location
        paintRobotLocation(event, curLoc, true);
    }

}

void WorldViewPainter::paintRobotLocation(QPaintEvent* event,
                                            messages::WorldModel loc,
                                            bool red)
{
	QPainter painter(this);

    if (red)
        painter.setBrush(Qt::red);

    QPoint locCenter(loc.my_x(), loc.my_y());

    //draw myself
	painter.drawEllipse(locCenter,
                        PARTICLE_WIDTH,
                        PARTICLE_WIDTH);

    //draw my heading
	painter.drawLine(loc.my_x(),
                     loc.my_y(),
                     PARTICLE_WIDTH * std::cos(loc.my_h()) + loc.my_x(),
                     PARTICLE_WIDTH * std::sin(loc.my_h()) + loc.my_y());

	//draw my uncertainty

	//draw where I think the ball is

	//draw how sure I am about where the ball is


}

void WorldViewPainter::updateWithLocationMessage(messages::WorldModel newLoc)
{
    curLoc = newLoc;
    if(shouldPaintLocation) {
        update();
    }
}

} // namespace viewer
} // namespace tool
