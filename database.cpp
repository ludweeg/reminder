#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent) {}
DataBase::~DataBase() {}

void DataBase::connectToDataBase()
{
    if(QFile(DATABASE_NAME).exists())
        this->openDataBase();
    else
        this->restoreDataBase();
}

bool DataBase::restoreDataBase()
{
    if(this->openDataBase())
        return !this->createActiveTaskTable() || !this->createTaskTable();
    else
    {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
}

bool DataBase::openDataBase()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setHostName(DATABASE_HOSTNAME);
    this->db.setDatabaseName(DATABASE_NAME);
    return this->db.open();
}

void DataBase::closeDataBase()
{
    this->db.close();
}

bool DataBase::createTaskTable()
{
    QSqlQuery query;
    if(!query.exec("CREATE TABLE " TASK " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            TASK_STATUS  " INT          NOT NULL,"
                            TASK_DATE    " VARCHAR(255) NOT NULL,"
                            TASK_TIME    " VARCHAR(255) NOT NULL,"
                            TASK_NAME    " VARCHAR(255) NOT NULL,"
                            TASK_MESSAGE " VARCHAR(255) NOT NULL)"))
    {
        qDebug() << "DataBase: error of create " << TASK;
        qDebug() << query.lastError().text();
        return false;
    }
    else
        return true;
}

bool DataBase::createActiveTaskTable()
{
    QSqlQuery query;
    if(!query.exec("CREATE TABLE " ACTIVE " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            ACTIVE_STATUS  " INT          NOT NULL,"
                            ACTIVE_NAME    " VARCHAR(255) NOT NULL,"
                            ACTIVE_TIME    " INT          NOT NULL,"
                            ACTIVE_MESSAGE " VARCHAR(255) NOT NULL)"))
    {
        qDebug() << "DataBase: error of create " << ACTIVE;
        qDebug() << query.lastError().text();
        return false;
    }
    else
        return true;
}

bool DataBase::inserIntoDeviceTable(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " ACTIVE " ( " ACTIVE_STATUS ", "
                                              ACTIVE_NAME ", "
                                              ACTIVE_TIME ", "
                                              ACTIVE_MESSAGE ") "

                  "VALUES (:Status, :Name, :Time, :Message)");
    query.bindValue(":Status",  A_STATUS);
    query.bindValue(":Name",    data[0]);
    query.bindValue(":Time",    data[1]);
    query.bindValue(":Message", data[2]);

    if(!query.exec())
    {
        qDebug() << "error insert into " << ACTIVE;
        qDebug() << query.lastError().text();
        return false;
    }
    else
        return true;

}
