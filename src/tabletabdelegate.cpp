#include "tabletabdelegate.h"
#include <QPainter>
#include <QTableView>
#include <QSqlTableModel>
#include <QLineEdit>
#include <QSizePolicy>
#include <QBoxLayout>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSettings>


void TableTabDelegate::paint(QPainter *painter, 
        const QStyleOptionViewItem &option, 
        const QModelIndex &index) const
{

    QString data = index.data().toString();
    if (data == ""){
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    QSettings settings("jenuz_org", "jiz");
    settings.beginGroup("PixelGridColorMap");
    QVariant val = settings.value(data);

    if  (val == QVariant()){
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    QColor color = val.value<QColor>(); 
    QColor negColor(255-color.red(), 
            255-color.green(), 
            255-color.blue()); 
    painter->fillRect(option.rect, color);
    //painter->drawText(option.rect, index.data().toString());
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt,index);
    opt.palette.setBrush(QPalette::Text, negColor);
    QStyledItemDelegate::paint(painter, opt, index);
    return;


}

QWidget *TableTabDelegate::createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{

    auto subDelegate = new InnerTableTabDelegate();
    auto table = new QTableView(parent);
    table->setItemDelegate(subDelegate);

    return table;
}


void TableTabDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *table = qobject_cast<QTableView*>(editor);
    table->installEventFilter(const_cast<TableTabDelegate*>(this));


    const QAbstractItemModel *model = index.model();

    QString value = model->data(index, Qt::EditRole).toString();
    QString date = index.siblingAtColumn(0).data().toString();
    QString hour = model->headerData(index.column(), Qt::Horizontal).toString();
    hour = hour.replace("H","");
    QSqlQuery query;
    query.prepare(R"(
        SELECT id, status,
            EXTRACT(hour FROM time_start) as hour, 
            EXTRACT(minutes FROM time_start) as start, 
            EXTRACT(minutes FROM time_end) as end
        FROM timetable
        WHERE 
            date = :date
        AND
            EXTRACT(hour FROM time_start) = :hour
        ORDER BY id DESC;)");

    query.bindValue(":date", date); 
    query.bindValue(":hour", hour);
    query.exec();


    QSqlTableModel *editorModel = new QSqlTableModel;
    editorModel->setQuery(std::move(query));
    editorModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    table->setModel(editorModel);

    const int &rowCount = table->model()->rowCount();
    const int &rowHeight = table->rowHeight(0);
    table->resize(rowHeight * 8, rowHeight * (rowCount+1));
    
    const int colCount = 25;
    if (index.column() > colCount /2){
        QPoint pos = table->pos() - QPoint(table->width(),0);
        table->move(pos);
    }

    table->hideColumn(0);
    table->setColumnWidth(1,table->rowHeight(0)*2);
    table->setColumnWidth(2,table->rowHeight(0)*2);
    table->setColumnWidth(3,table->rowHeight(0)*2);
    table->setColumnWidth(4,table->rowHeight(0)*2);
    table->verticalHeader()->hide();
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void TableTabDelegate::setModelData(QWidget *editor, 
        QAbstractItemModel *model, 
        const QModelIndex &index) const
{
    auto tabelModel = qobject_cast<QSqlTableModel*>(model);
    tabelModel->setQuery(tabelModel->query().lastQuery());
}


void TableTabDelegate::updateEditorGeometry(QWidget *editor, 
        const QStyleOptionViewItem &option, 
        const QModelIndex &index) const
{   
//    auto srctable = qobject_cast<QTableWidget*>(editor->parent());
//
//    qDebug()<<"table geom"<< srctable->geometry();
//    qDebug()<<"opt   geom"<< option.rect;



    QStyledItemDelegate::updateEditorGeometry(editor, option, index);}


