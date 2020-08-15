#include <QtGui/QApplication>
#include "vedio.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    vedio w;
    w.showFullScreen();

    w.run();

    return a.exec();
}
