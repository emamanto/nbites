#include "DataProcessingTool.h"

namespace qtool {

using data::DataLoader;
using viewer::VisionViewer;

DataProcessingTool::DataProcessingTool() : EmptyQTool("Data Tool"),
                             dataLoader(new DataLoader(dataManager)),
                             visionViewer(new VisionViewer(dataManager))
{
    start = new QPushButton(tr("Start"));
    connect(start, SIGNAL(clicked()), visionViewer, SLOT(iterate()));

    toolTabs->addTab(dataLoader, tr("Data Loader"));
    toolTabs->addTab(visionViewer, tr("Vision"));

	scrollArea->setWidget(toolTabs);
	scrollArea->resize(toolTabs->size());
	this->setCentralWidget(scrollArea);

	tabStartSize = new QSize(toolTabs->size());

    toolbar->addWidget(start);
}

}
