#ifndef INSERTWINDGET_H
#define INSERTWINDGET_H
#include <QWidget>
#include <ui_insertwidget.h>

namespace Ui {
    class InsertWidget;
}

class InsertWidget : public QWidget
{
    Q_OBJECT
    public:
        InsertWidget(QWidget *parent = nullptr);
        ~InsertWidget ();
    private:
        Ui::InsertWidget *ui;

        void initDateTime();
        void initStatusBox();
        void submitButton();
        void plusButton();
};

#endif//INSERTWINDGET_H
