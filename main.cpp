#include "ttkapplication.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TTKApplication w;
    w.show();

    return a.exec();
}
