#include "round_image.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RoundImage w;
    w.show();
    return a.exec();
}
