
#include "VisionViewer.h"
#include "Camera.h"
#include "ClassHelper.h"
#include <vector>

namespace qtool {
namespace viewer {

using namespace data;
using boost::shared_ptr;
using namespace man::memory;
using namespace man::corpus;
using namespace qtool::image;
using namespace man::memory::proto;
using namespace google::protobuf;

VisionViewer::VisionViewer(DataManager::ptr memoryManager) :
                 memoryManager(memoryManager),
                 speech(new Speech()),
                 sensors(new Sensors(speech)),
                 bottomRawImage(new proto::PRawImage()),
                 topRawImage(new proto::PRawImage()),
                 output()
{
    offlineMVision = MVision::ptr(new MVision());

    pose = shared_ptr<NaoPose> (new NaoPose(sensors));
    vision = shared_ptr<Vision> (new Vision(pose, offlineMVision));

    imageTranscribe = OfflineImageTranscriber::ptr
        (new OfflineImageTranscriber(sensors,
                                     memoryManager->getMemory()->
                                     get<MRawImages>()));

    bottomRawImage->set_width(AVERAGED_IMAGE_WIDTH);
    bottomRawImage->set_height(AVERAGED_IMAGE_HEIGHT);
    topRawImage->set_width(AVERAGED_IMAGE_WIDTH);
    topRawImage->set_height(AVERAGED_IMAGE_HEIGHT);

    bottomRawImage->mutable_image()->assign(AVERAGED_IMAGE_SIZE, 0);
    topRawImage->mutable_image()->assign(AVERAGED_IMAGE_SIZE, 0);

    //*** TOOLBAR STUFF ***//
	QToolBar* toolBar = new QToolBar(this);
	QToolBar* toolBar2 = new QToolBar(this);
    QPushButton* loadTableButton = new QPushButton(tr("&Load Table"));
    connect(loadTableButton, SIGNAL(clicked()), this, SLOT(loadColorTable()));
    toolBar->addWidget(loadTableButton);
    this->addToolBar(toolBar);
	this->addToolBarBreak();
	this->addToolBar(toolBar2);

#define ADD_DEBUG_CHECKBOX1(text, func) {           \
        QCheckBox* debug = new QCheckBox(tr(text)); \
        connect(debug, SIGNAL(stateChanged(int)),   \
                this, SLOT(func(int)));             \
        toolBar->addWidget(debug);                  \
    }

#define ADD_DEBUG_CHECKBOX2(text, func) {           \
        QCheckBox* debug = new QCheckBox(tr(text)); \
        connect(debug, SIGNAL(stateChanged(int)),   \
                this, SLOT(func(int)));             \
        toolBar2->addWidget(debug);                 \
    }

    ADD_DEBUG_CHECKBOX1("Horizon Debug", setHorizonDebug);
    ADD_DEBUG_CHECKBOX1("Shooting Debug", setShootingDebug);
    ADD_DEBUG_CHECKBOX1("Open Field Debug", setOpenFieldDebug);
    ADD_DEBUG_CHECKBOX1("Edge Detection Debug", setEdgeDetectionDebug);
    ADD_DEBUG_CHECKBOX2("Hough Debug", setHoughTransformDebug);
    ADD_DEBUG_CHECKBOX2("Robot Detection Debug", setRobotsDebug);
    ADD_DEBUG_CHECKBOX2("Visual Line Debug", setVisualLinesDebug);
    ADD_DEBUG_CHECKBOX2("Visual Corner Debug", setVisualCornersDebug);


	//*** VISION IMAGE STUFF ***//
    bottomVisionImage = new ThresholdedImage(bottomRawImage, this);
    topVisionImage = new ThresholdedImage(topRawImage, this);

	topVisionView = new BMPImageViewer(topVisionImage, this);
	bottomVisionView = new BMPImageViewer(bottomVisionImage, this);

    VisualInfoImage* shapesBottom = new VisualInfoImage(offlineMVision, Camera::BOTTOM);
    VisualInfoImage* shapesTop = new VisualInfoImage(offlineMVision, Camera::TOP);

    MRawImages::const_ptr rawImages = memoryManager->getMemory()->get<MRawImages>();

    FastYUVToBMPImage* rawTopBMP = new FastYUVToBMPImage(rawImages, Camera::TOP, this);
    FastYUVToBMPImage* rawBottomBMP = new FastYUVToBMPImage(rawImages, Camera::BOTTOM, this);

    OverlayedImage* comboBottom = new OverlayedImage(rawBottomBMP, shapesBottom, this);
    OverlayedImage* comboTop = new OverlayedImage(rawTopBMP, shapesTop, this);

    BMPImageViewer *bottomImageViewer = new BMPImageViewer(comboBottom, this);
    BMPImageViewer *topImageViewer = new BMPImageViewer(comboTop, this);

    connect(this, SIGNAL(imagesUpdated()), bottomImageViewer, SLOT(updateView()));
    connect(this, SIGNAL(imagesUpdated()), topImageViewer, SLOT(updateView()));

    memoryManager->connectSlot(bottomImageViewer, SLOT(updateView()), "MRawImages");
    memoryManager->connectSlot(topImageViewer, SLOT(updateView()), "MRawImages");
	memoryManager->connectSlot(this, SLOT(update()), "MRawImages");

	connect(this, SIGNAL(imagesUpdated()),
            bottomVisionView, SLOT(updateView()));

    connect(this, SIGNAL(imagesUpdated()),
            topVisionView, SLOT(updateView()));

	//mouse listeners for all four images
	BMPImageViewerListener* bottomVisionListener
		= new BMPImageViewerListener(bottomVisionImage, this);
	BMPImageViewerListener* topVisionListener
		= new BMPImageViewerListener(topVisionImage, this);
	BMPImageViewerListener* bottomRawListener
		= new BMPImageViewerListener(comboBottom, this);
	BMPImageViewerListener* topRawListener
		= new BMPImageViewerListener(comboTop, this);
    bottomVisionListener->hide();
	topVisionListener->hide();
	bottomRawListener->hide();
	topRawListener->hide();
	connect(topVisionListener, SIGNAL(mouseClicked(int, int, int, bool)),
            this, SLOT(pixelClicked(int, int, int, bool)));
	connect(bottomVisionListener, SIGNAL(mouseClicked(int, int, int, bool)),
            this, SLOT(pixelClicked(int, int, int, bool)));
	connect(topRawListener, SIGNAL(mouseClicked(int, int, int, bool)),
            this, SLOT(pixelClicked(int, int, int, bool)));
	connect(bottomRawListener, SIGNAL(mouseClicked(int, int, int, bool)),
			this, SLOT(pixelClicked(int, int, int, bool)));


	//*** WINDOW LAYOUT STUFF ***//
	QWidget* rawPane = new QWidget(this);
	QWidget* visionPane = new QWidget(this);

    QTabWidget* rawTabs = new QTabWidget(rawPane);
	QHBoxLayout* rawTabLayout = new QHBoxLayout(rawPane);

    rawTabs->addTab(topImageViewer, tr("Top Image"));
    rawTabs->addTab(bottomImageViewer, tr("Bottom Image"));

	rawTabLayout->addWidget(rawTabs);
	rawPane->setLayout(rawTabLayout);

	QTabWidget* visTabs = new QTabWidget(visionPane);
    QHBoxLayout* visTabLayout = new QHBoxLayout(visionPane);

    visTabs->addTab(topVisionView, tr("Top Vision"));
    visTabs->addTab(bottomVisionView, tr("Bottom Vision"));

	visTabLayout->addWidget(visTabs);
    visionPane->setLayout(visTabLayout);

    QTabWidget* outsideTabs = new QTabWidget();
    outsideTabs->addTab(rawPane, tr("Raw Images"));
    outsideTabs->addTab(visionPane, tr("Vision Images"));

    this->setCentralWidget(outsideTabs);

    //corner ownership
    this->setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    Memory::const_ptr memory = memoryManager->getMemory();

    std::vector<QTreeView> messageViewers;

    QDockWidget* dockWidget = new QDockWidget("Offline Vision", this);
	dockWidget->setMinimumWidth(300);
	dockWidget->setMaximumHeight(375);
    offlineVisionView = new MObjectViewer(offlineMVision, this);
	dockWidget->setWidget(offlineVisionView);
	connect(this, SIGNAL(imagesUpdated()), offlineVisionView, SLOT(updateView()));
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    dockWidget = new QDockWidget("Image data", this);
	dockWidget->setMinimumWidth(300);
	dockWidget->setMaximumHeight(300);
    MObjectViewer* imageDataView = new MObjectViewer(
		memoryManager->getMemory()->get<MRawImages>(), this);
    dockWidget->setWidget(imageDataView);
    memoryManager->connectSlot(imageDataView, SLOT(updateView()), "MRawImages");
    this->addDockWidget(Qt::RightDockWidgetArea, dockWidget);

}

void VisionViewer::collectData()
{
    QString path = QFileDialog::getSaveFileName(this, "Choose Output File",
                                                QString(NBITES_DIR) +
                                                "/data");
    if (!path.isEmpty()) {
        output.open(path.toStdString().c_str());
        memoryManager->connectSlot(this, SLOT(iterate()), "MRawImages");
        memoryManager->getNext("MRawImages");
    }
}

void VisionViewer::iterate()
{
    output << "BALL " << offlineMVision->get()->visual_ball().heat() << " "
           << offlineMVision->get()->visual_ball().visual_detection().distance()
           << " " << offlineMVision->get()->visual_ball().visual_detection().bearing()
           << " " << offlineMVision->get()->timestamp() << std::endl;

    output << "RED " << 0 << " "
           << offlineMVision->get()->red1().visual_detection().distance()
           << " " << offlineMVision->get()->red1().visual_detection().bearing()
           <<  " " << offlineMVision->get()->timestamp() << std::endl;

    output << "BLUE " << 0 << " "
           << offlineMVision->get()->navy1().visual_detection().distance() <<
        " " << offlineMVision->get()->navy1().visual_detection().bearing()
           <<  " " << offlineMVision->get()->timestamp() << std::endl;

    // For goalie posts and corners:
    // float leftDistance = offlineMVision->get()->
    //     yglp().visual_detection().distance();
    // float rightDistance = offlineMVision->get()->
    //     ygrp().visual_detection().distance();

    // float leftBearing = offlineMVision->get()->
    //     yglp().visual_detection().bearing();
    // float rightBearing = offlineMVision->get()->
    //     ygrp().visual_detection().bearing();

    // if (leftDistance !=  0.0 && leftDistance < 400.0 && leftBearing > 0.0)
    //     output << "LEFT POST " << leftDistance << " " <<
    //         leftBearing << " 0 0" << std::endl;

    // if (rightDistance !=  0.0 && rightDistance < 400.0 && rightBearing < 0.0)
    //     output << "RIGHT POST " << rightDistance << " " <<
    //         rightBearing << " 0 0" << std::endl;

    // const RepeatedPtrField<PVision::PVisualCorner> cornersData =
    //     offlineMVision->get()->visual_corner();

    // for(int i = 0; i < cornersData.size(); i++)
    // {
    //     const PVision::PVisualDetection corner =
    //         cornersData.Get(i).visual_detection();
    //     const PVision::PVisualCorner cor =
    //         cornersData.Get(i);

    //     if (cor.corner_type() == 1)
    //     {
    //         if (corner.bearing() > 0.0)
    //             output << "LEFT CORNER ";
    //         if (corner.bearing() < 0.0)
    //             output << "RIGHT CORNER ";

    //         output << corner.distance() << " " <<
    //                 corner.bearing() << " " <<
    //                 cor.physical_orientation() << " " <<
    //                 cor.orientation() << std::endl;
    //     }
    //}

    // output << "\n";

    memoryManager->getNext("MRawImages");
}

void VisionViewer::update()
{
    //no useless computation
    if (!this->isVisible())
        return;

    imageTranscribe->acquireNewImage();

    vision->notifyImage(sensors->getImage(Camera::TOP), sensors->getImage(Camera::BOTTOM));
    // Will need to get these to be diffent thresholded images but vision
    // appears to only threhold one at the moment!
    bottomRawImage->mutable_image()->assign(reinterpret_cast<const char *>
                                            (vision->thresh->thresholdedBottom),
                                            AVERAGED_IMAGE_SIZE);
    topRawImage->mutable_image()->assign(reinterpret_cast<const char *>
                                         (vision->thresh->thresholded),
                                         AVERAGED_IMAGE_SIZE);
    emit imagesUpdated();
}

void VisionViewer::pixelClicked(int x, int y, int brushSize, bool leftClick) {

    estimate pixEst = vision->pose->pixEstimate(x, y, 0.0f);
    std::cout << "x: " << x << "   y: " << y << std::endl;
    std::cout << pixEst << std::endl;
}

void VisionViewer::loadColorTable(){
	QString colorTablePath = QFileDialog::getOpenFileName(this, tr("Open Color Table"),
														  "../../data/tables",
														  tr("Table Files (*.mtb)"));
	imageTranscribe->initTable(colorTablePath.toStdString());
	update();
}

#define SET_DEBUG(funcName, buttonName)                             \
    void VisionViewer::set##funcName##Debug(int state) {            \
        vision->thresh->setDebug##funcName(state == Qt::Checked);   \
        update();                                                   \
    }

SET_DEBUG(Horizon, horizon);
SET_DEBUG(HoughTransform, hough);
SET_DEBUG(Shooting, shoot);
SET_DEBUG(EdgeDetection, edgeDetect);
SET_DEBUG(OpenField, openField);
SET_DEBUG(Robots, robots);
SET_DEBUG(VisualLines, visualLines);
SET_DEBUG(VisualCorners, visualCorners);

}
}
