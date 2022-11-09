#include "widget.h"
#include "aibattle.h"
#include "net.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget n;
    n.show();
    return a.exec();
}
