#ifndef TABLETAB_H
#define TABLETAB_H
#include <QWidget>
#include <ui_tabletab.h>
#include <QSqlTableModel>
#include <QSqlQuery>
namespace Ui {
    class TableTab;
}

class TableTab : public QWidget
{
    Q_OBJECT
    public:
        TableTab(QWidget *parent = nullptr);
        ~TableTab ();
    private:
        Ui::TableTab *ui;
        QString query2;

        void resizeEvent(QResizeEvent *event) override;
        void draw();

        inline void updateView(){
            auto model = qobject_cast<QSqlTableModel*>(ui->tableView->model());
            model->select();
            ui->tableView->reset();
            model->setQuery(query2);
            qDebug()<<"update";
        }

};

#endif//TABLETAB_H
