#include "Tool.h"
#include <QTextStream>
#include <QFileDialog>
#include <QKeyEvent>
#include <QDebug>
#include <iostream>

namespace tool {

// This file saves the dimensions from the last use of the tool
QFile file(QString("./.geometry"));

Tool::Tool(const char* title) :
    QMainWindow(),
    diagram(),
    selector(),
    logView(this),
    toolTabs(new QTabWidget),
    toolbar(new QToolBar),
    nextButton(new QPushButton(tr(">"))),
    prevButton(new QPushButton(tr("<"))),
    recordButton(new QPushButton(tr("Rec"))),
    scrollArea(new QScrollArea),
    scrollBarSize(new QSize(5, 35)),
    tabStartSize(new QSize(toolTabs->size()))
{
    // Set up the GUI and slots
    this->setWindowTitle(tr(title));

	//connect both to a slot that switches the run direction boolean if neccessary
	connect(nextButton, SIGNAL(clicked()), &diagram, SLOT(setForwardDir()));
	connect(prevButton, SIGNAL(clicked()), &diagram, SLOT(setBackDir()));

    //connect both fwd and prv button to the run slot
	connect(nextButton, SIGNAL(clicked()), &diagram, SLOT(run()));
	connect(prevButton, SIGNAL(clicked()), &diagram, SLOT(run()));

    connect(&selector, SIGNAL(signalNewDataSet(std::vector<std::string>)),
            &diagram, SLOT(addUnloggers(std::vector<std::string>)));

    connect(&diagram, SIGNAL(signalNewProviders(std::vector<unlog::GenericProviderModule*>)),
            &logView, SLOT(addProtoViewers(std::vector<unlog::GenericProviderModule*>)));
    connect(&diagram, SIGNAL(signalNewImageProviders(std::vector<image::YUVtoRGBModule*>)),
            &logView, SLOT(addImageViewers(std::vector<image::YUVtoRGBModule*>)));

    connect(&diagram, SIGNAL(signalNewProviders(std::vector<unlog::GenericProviderModule*>)),
            this, SLOT(setUpModules()));

    connect(&logView, SIGNAL(newDisplayModule(image::ImageDisplayQModule*)),
            &diagram, SLOT(addDisplayModule(image::ImageDisplayQModule*)));

    toolbar->addWidget(prevButton);
    toolbar->addWidget(nextButton);
    toolbar->addWidget(recordButton);

    toolTabs->addTab(&selector, tr("Data"));
    toolTabs->addTab(&logView, tr("Log View"));

    this->setCentralWidget(toolTabs);
    this->addToolBar(toolbar);

    // Figure out the appropriate dimensions for the window
    if (file.open(QIODevice::ReadWrite)){
            QTextStream in(&file);
            geometry = new QRect(in.readLine().toInt(), in.readLine().toInt(),
                                 in.readLine().toInt(), in.readLine().toInt());
            file.close();
    }
    // If we don't have dimensions, default to hard-coded values
    if((geometry->width() == 0) && (geometry->height() == 0)){
        geometry = new QRect(75, 75, 1132, 958);
    }
    this->setGeometry(*geometry);
}

Tool::~Tool() {
    // Write the current dimensions to the .geometry file for next use
    if (file.open(QIODevice::ReadWrite)){
        QTextStream out(&file);
        out << this->pos().x() << "\n"
            << this->pos().y() << "\n"
            << this->width() << "\n"
            << this->height() << "\n";
    }
}

void Tool::setUpModules()
{
}

// Keyboard control
void Tool::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
    case Qt::Key_J:
    case Qt::Key_D:
    case Qt::Key_N:
		//unlog::UnlogBase::readDir = 1;
        diagram.run();
        break;
    case Qt::Key_K:
    case Qt::Key_S:
    case Qt::Key_P:
		//unlog::UnlogBase::readDir = 0;
		diagram.run();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

// Provides scrollbars appropriately if the window gets too small
void Tool::resizeEvent(QResizeEvent* ev)
{
    QSize widgetSize = ev->size();
    if((widgetSize.width() > tabStartSize->width()) ||
       (widgetSize.height() > tabStartSize->height())) {
        toolTabs->resize(widgetSize-*scrollBarSize);
    }
    QWidget::resizeEvent(ev);
}

}
