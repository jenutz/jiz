#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QDebug>

class DatabaseWrappper{
    private:
        QSqlDatabase db;

    public:
        DatabaseWrappper(){
            QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
            db.setHostName("localhost");
            db.setDatabaseName("jiz");
            db.setUserName("jenuz");
            db.setPassword("");
            bool ok = db.open();
            qDebug()<<ok;
        };
        ~DatabaseWrappper(){
        }

};
