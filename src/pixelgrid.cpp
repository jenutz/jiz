#include "pixelgrid.h"
#include <QRandomGenerator>
#include <QSettings>

PixelGrid::PixelGrid(QWidget *parent) :
    QWidget(parent)
{
    loadColors();

    label = new QLabel(this);
    label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    pixmap = QPixmap(label->size());
    drawPixmap();
    label->setPixmap(pixmap);
    
    button = new QPushButton(this);
    connect(button,&QPushButton::clicked,this,&PixelGrid::drawPixmap);
}


void PixelGrid::resizeEvent(QResizeEvent *event){
    if (event->size() == QSize(0,0))
        return;
    label->resize(event->size());

    auto getArea = [](QSize size){ return size.width() * size.height();};

    const int AREA_REDRAW_K = 100;
    if (getArea(event->size())/getArea(pixmap.size()) > AREA_REDRAW_K ){
        drawPixmap();
    }
    else{
        label->setPixmap(pixmap.scaled(event->size(),Qt::KeepAspectRatio));
    }
    
    QWidget::resizeEvent(event);
}

void PixelGrid::loadColors(){
    QSettings settings("jenuz_org", "jiz");
    settings.beginGroup("PixelGridColorMap");
    for (QString key: settings.allKeys()){
        colorMap[key] = settings.value(key).value<QColor>();
    }
}

void PixelGrid::drawPixmap(){

    QSize labelSize = label->size();
    int H_PER_DAY = 24;
    int sqLen = labelSize.width()/H_PER_DAY;

    QSqlQuery query = getStatusPerHour();
    
    pixmap = QPixmap(labelSize);
    QPainter painter(&pixmap);
    painter.eraseRect(0, 0, labelSize.width(), labelSize.height());

    while (query.next()){
        QString color = query.value(0).toString();
        int day = query.value(1).toInt()+1; 
        int hour = query.value(2).toInt()+1;

        QRect rect(hour * sqLen, day* sqLen, sqLen, sqLen);
        QBrush brush(colorMap[color],Qt::Dense3Pattern);
        painter.fillRect(rect, brush) ;
    }

   label->setPixmap(pixmap); 
}

QSqlQuery PixelGrid::getStatusPerHour(){
    QSqlQuery query;
    QString maxHours = R"(
        SELECT 
            l2.status,
            EXTRACT(day FROM 
                (l2.date::timestamp - (SELECT MIN(date) FROM extended)::timestamp)) 
                AS days_epoch,
            l2.hour
        FROM
                
        
                (SELECT 
                     s.date, s.hour, MIN(time_start) as time_start
                FROM
                        (SELECT
                            date, hour, MAX(length) as length
                        FROM
                            extended
                        GROUP BY hour, date
                        ) as t
                    JOIN 
                        extended as s
                    ON s.hour = t.hour and t.length = s.length and t.date = s.date
                GROUP BY s.hour, s.date
                ) as l1
        
        
            JOIN
                extended as l2
        
            ON l1.time_start = l2.time_start and l1.date = l2.date
        ORDER BY id;
    )";
    
    query.exec(maxHours);
    return query;
}



//void PixelGrid::drawPixmap(){
//    int H_PER_DAY = 24;
//
//    QSize labelSize = label->size();
//    pixmap = QPixmap(labelSize);
//    
//    int squareLen = labelSize.width()/H_PER_DAY;
//    
//    QPainter painter(&pixmap);
//    
//    painter.eraseRect(0, 0, labelSize.width(), labelSize.height());
//    painter.setPen(Qt::red);
//
//    
//    QRandomGenerator prng1(42);
//    for (int j = 0; j < (int)(labelSize.height()/squareLen); ++j){
//        for (int i = 0; i < H_PER_DAY; ++i){
//            int rand = prng1.generate();
//            QRect rect(i * squareLen, j * squareLen, squareLen, squareLen);
//            if (rand % 2 == 0){
//                painter.setBrush(Qt::NoBrush);
//                painter.drawRect(rect);
//            }
//            else{
//                painter.setBrush(QBrush(Qt::red,Qt::Dense3Pattern));
//                painter.fillRect(rect, painter.brush());
//            }
//
//        }
//    }
//}
