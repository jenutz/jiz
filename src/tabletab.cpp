#include "tabletab.h"
#include "tabletabdelegate.h"
#include <QSqlTableModel>
#include <QPushButton>

TableTab::TableTab(QWidget *parent): 
    QWidget(parent), 
    ui(new Ui::TableTab)
{
    ui->setupUi(this);
    ui->tableView->setItemDelegate(new TableTabDelegate) ;

    QString query = R"(
        SELECT * FROM crosstab(
            'SELECT
                date,hour,
                string_agg(CONCAT(status, ''-'', length), '' '') AS concatenated_status
             FROM extended
             GROUP BY date, hour
             ORDER BY date, hour',
            'SELECT DISTINCT hour FROM extended ORDER BY hour'
        ) AS extended(
            date date,
            H00 text, H01 text, H02 text, H03 text, H04 text, H05 text, 
            H06 text, H07 text, H08 text, H09 text, H10 text, H11 text,
            H12 text, H13 text, H14 text, H15 text, H16 text, H17 text,
            H18 text, H19 text, H20 text, H21 text, H22 text, H23 text
        );)";




    query2 = R"(
        SELECT * FROM crosstab(
            'SELECT 
                l2.date,
                l2.hour,
                l2.status
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
            ORDER BY id',
            'SELECT DISTINCT hour FROM extended ORDER BY hour'
        ) AS extended(
            date date,
            H00 text, H01 text, H02 text, H03 text, H04 text, H05 text, 
            H06 text, H07 text, H08 text, H09 text, H10 text, H11 text,
            H12 text, H13 text, H14 text, H15 text, H16 text, H17 text,
            H18 text, H19 text, H20 text, H21 text, H22 text, H23 text
        );)";

    QSqlTableModel *model = new QSqlTableModel;
    model->setEditStrategy(QSqlTableModel::OnFieldChange);


    model->setQuery(query2);

    const int H_IN_DAY = 24;
    const int offset = 1;
    model->setHeaderData(0, Qt::Horizontal,"Date");
    for (int i = 0; i < H_IN_DAY; i++)
    {
        QString header = "H" + QString::number(i).rightJustified(2, '0');
        model->setHeaderData(i + offset, Qt::Horizontal, header);
    }

    ui->tableView->setModel(model);
    draw();
}

void TableTab::draw(){
    const int H_IN_DAY = 24;
    const int offset = 1;
    QTableView *view = ui->tableView;
    view->horizontalHeader()->setMinimumSectionSize(view->rowHeight(0));
    int tableWidth = this->size().width();
    int dateColWidth = view->columnWidth(0);
    int hourColWidth = (tableWidth - dateColWidth) / (H_IN_DAY+offset);

    for (int i = 0; i < H_IN_DAY; i++)
    {
        view->setColumnWidth(i + offset, hourColWidth);
    }
}

void TableTab::resizeEvent(QResizeEvent *event){
    draw();
    QWidget::resizeEvent(event);
}


TableTab::~TableTab(){
    delete ui;
};
