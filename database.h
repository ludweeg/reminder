#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QStringList>

#define DATABASE_HOSTNAME   "DataBase"
#define DATABASE_NAME       "DataBase.db"

#define TASK                "TasksTable"
#define TASK_DATE           "Date"
#define TASK_TIME           "Time"
#define TASK_NAME           "Name"
#define TASK_MESSAGE        "Message"
#define TASK_STATUS         "Status"

#define ACTIVE               "ActiveTable"
#define ACTIVE_NAME          "Name"
#define ACTIVE_TIME          "Time"
#define ACTIVE_STATUS        "Status"
#define ACTIVE_MESSAGE       "Message"

#define A_STATUS  3
#define NA_STATUS 4

class DataBase : public QObject
{
    Q_OBJECT\

public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    void connectToDataBase();
    bool inserIntoDeviceTable(const QVariantList &data);

private:
    QSqlDatabase db;

private:
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTaskTable();
    bool createActiveTaskTable();
};

#endif // DATABASE_H

