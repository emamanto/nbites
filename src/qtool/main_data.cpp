#include <QtGui/QApplication>
#include "DataProcessingTool.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtool::DataProcessingTool qTool;
    qTool.show();
    return a.exec();
}
