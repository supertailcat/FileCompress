#include "compress.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Compress w;
    w.show();
    return a.exec();
}
