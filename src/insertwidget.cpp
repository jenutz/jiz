#include "insertwidget.h"
#include <QSqlQuery>
#include <QSqlError>

InsertWidget::InsertWidget(QWidget *parent): 
    QWidget(parent), 
    ui(new Ui::InsertWidget)
{
    ui->setupUi(this);
    initStatusBox();
    initDateTime();

    connect(ui->dateButton,&QPushButton::clicked,
            this,&InsertWidget::initDateTime);
    connect(ui->statusButton,&QPushButton::clicked,
            this,&InsertWidget::initStatusBox);
    connect(ui->submitButton,&QPushButton::clicked,
            this,&InsertWidget::submitButton);

    int len = ui->plusLayout->count();
    for (int i = 0; i < len; ++i){
        QWidget *button = ui->plusLayout->itemAt(i)->widget();
        connect(qobject_cast<QPushButton*>(button),&QPushButton::clicked,
                this,&InsertWidget::plusButton);
    }
}

void InsertWidget::plusButton(){
    QPushButton *button = (QPushButton*)sender();
    int offset = button->text().right(2).toInt();

    int SEC_IN_MIN = 60;
    QTime newTime = ui->timeEndEdit->time().addSecs(offset * SEC_IN_MIN);
    ui->timeEndEdit->setTime(newTime);

}

void InsertWidget::initStatusBox(){
    QSqlQuery query(R"(
    SELECT status, SUM(now_diff) from (
        SELECT status,
               ABS(EXTRACT(HOUR from NOW())-hour) as now_diff  
        FROM extended
    ) GROUP BY status ORDER BY sum;)");

    while (query.next()){
        ui->statusBox->addItem(query.value(0).toString());
    }
}


void InsertWidget::initDateTime(){
    QSqlQuery query(R"(
    SELECT
        date,
        time_end as time_start,
        EXTRACT(EPOCH from date) + EXTRACT(EPOCH from time_end) as epoch
    FROM timetable ORDER BY epoch DESC LIMIT 1;)");

    while (query.next()){
         QString date = query.value(0).toString();
         QString time_start = query.value(1).toString();

        ui->dateEdit->setDate(QDate::fromString(date,"yyyy-MM-dd"));

        QString timeFormat = "hh:mm:ss.zzz";
        ui->timeStartEdit->setTime(QTime::fromString(time_start,timeFormat));
        ui->timeEndEdit->setTime(QTime::fromString(time_start,timeFormat));
    }
}

void InsertWidget::submitButton(){
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO timetable(status, date, time_start, time_end)
        VALUES (:status,:date,:time_start, :time_end);
    )");

    QString timeFormat = "hh:mm:ss.zzz";
     
    QString status = ui->statusBox->currentText(); 
    QDate date =ui->dateEdit->date();
    QTime time_start = ui->timeStartEdit->time();
    QTime time_end =ui->timeEndEdit->time(); 

    query.bindValue(":status", status);
    query.bindValue(":date", date);
    query.bindValue(":time_start", time_start); 
    query.bindValue(":time_end", time_end);
    
    query.exec();
}


InsertWidget::~InsertWidget(){
    delete ui;
};

