#include <QtGui/QApplication>
#include "vediorcv.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    vediorcv w;
    w.show();
    return a.exec();
}
