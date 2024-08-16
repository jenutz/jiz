#ifndef PIXEL_GRID_H
#define PIXEL_GRID_H

#include <QWidget>
#include <QResizeEvent>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QSqlQuery>

class PixelGrid: public QWidget{
    Q_OBJECT

    public:
        explicit PixelGrid(QWidget *parent = nullptr);
    protected:
        void resizeEvent(QResizeEvent *event) override;

    private:
        QLabel *label;
        QPushButton *button;
        QPixmap pixmap;
        QMap<QString,QColor> colorMap;

        void drawPixmap();
        void loadColors();
        QSqlQuery getStatusPerHour();
};


#endif//PIXEL_GRID_H
