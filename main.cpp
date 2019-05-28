#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    //w.setWindowTitle("Hex Editor");
    w.setFixedSize(1072, 488);
    w.show();

    return app.exec();
}
