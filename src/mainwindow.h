#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <ui_mainwindow.h>

#include <QPieSeries>
#include <QSqlQuery>
#include "databasewrapper.h"

#include "pixelgrid.h"
#include "insertwidget.h"
#include "tabletab.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    private:
        void piechart();

        InsertWidget *insertTab = nullptr;
        PixelGrid *pixelGridTab = nullptr;
        TableTab *tableTab = nullptr;

        DatabaseWrappper *db = nullptr;
        Ui::MainWindow *ui;
};

#endif//MAINWINDOW_H
