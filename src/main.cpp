#include <QApplication>
#include <QPushButton>

#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    MainWindow win;
    win.setWindowTitle("jiz");
    win.show();
    return app.exec();
}
