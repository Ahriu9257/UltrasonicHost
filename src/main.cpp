#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 设置应用程序信息
    QApplication::setApplicationName("Ultrasonic Distance Measurement");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("Embedded Systems Lab");

    MainWindow window;
    window.show();

    return app.exec();
}
