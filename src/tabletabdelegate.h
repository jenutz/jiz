#ifndef TABLE_TAB_DELEGATE_H
#define TABLE_TAB_DELEGATE_H
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QTableWidget>

class TableTabDelegate : public QStyledItemDelegate{
    Q_OBJECT
    public:
        using QStyledItemDelegate::QStyledItemDelegate;

        void paint(QPainter *painter, 
                const QStyleOptionViewItem &option, 
                const QModelIndex &index) const override;

        QWidget *createEditor(QWidget *parent,
                const QStyleOptionViewItem &option,
                const QModelIndex &index) const override;

        void setModelData(QWidget *editor, 
                QAbstractItemModel *model, 
                const QModelIndex &index) const override;

        void setEditorData(QWidget *editor, 
                const QModelIndex &index) const override;

        void updateEditorGeometry(QWidget *editor, 
                const QStyleOptionViewItem &option, 
                const QModelIndex &index) const override;

    private:

        class InnerTableTabDelegate: public QStyledItemDelegate{
            QWidget *createEditor(QWidget *parent,
                    const QStyleOptionViewItem &option,
                    const QModelIndex &index) const override
            {
                return new QLineEdit(parent);
            }

            void setModelData(QWidget *editor, 
                    QAbstractItemModel *model, 
                    const QModelIndex &index) const override
            {
                auto line = qobject_cast<QLineEdit*>(editor);


                const QString column = model->
                    headerData(index.column(), Qt::Horizontal).toString();

                const QString value = line->text();
                const int id = index.siblingAtColumn(0).data().toInt();

                QMap<QString, QString> map;
                const int cnt = model->columnCount();
                for (int i = 0; i < cnt; ++i){
                    QString key = model->headerData(i, Qt::Horizontal).toString();
                    QString value = index.siblingAtColumn(i).data().toString();
                    map[key] = value;
                }

                map[column] = value;
                QString timeStart = map["hour"] + ':' + map["start"];
                QString timeEnd = map["hour"] + ':' + map["end"];

                QSqlQuery query;
                query.prepare(R"(
                UPDATE timetable 
                SET 
                    status = :status,
                    time_start = :time_start,
                    time_end   = :time_end
                WHERE
                    id = :id;             
            )");
                query.bindValue(":status", map["status"]);
                query.bindValue(":time_start", timeStart);
                query.bindValue(":time_end", timeEnd);
                query.bindValue(":id", map["id"]);
                query.exec();
            }
        };
};
#endif//TABLE_TAB_DELEGATE_H

