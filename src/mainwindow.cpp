#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DatabaseWrappper();

    ui->tabWidget->setCurrentIndex(3);

    pixelGridTab = new PixelGrid();
    ui->pixmapLayout->addWidget(pixelGridTab);

  
    insertTab = new InsertWidget();
    ui->insertLayout->addWidget(insertTab);
    
    tableTab = new TableTab();
    ui->tableTabLayout->addWidget(tableTab);
    

    piechart();
}
void MainWindow::piechart(){

    QSqlQuery query;

    QString statusSums = R"(
    SELECT 
        status, 
        SUM((EXTRACT(EPOCH FROM (time_end - time_start)) / 60)::int) AS diff 
    FROM timetable GROUP BY status
        )";

    query.exec(statusSums);

    QPieSeries *series = new QPieSeries();

    while (query.next()) {
        QString status = query.value(0).toString();
        int timeMin = query.value(1).toInt();
        series->append(status, timeMin);

    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    ui->pieChart->setChart(chart);
}

MainWindow::~MainWindow() {
    delete ui;
    delete insertTab;
}

