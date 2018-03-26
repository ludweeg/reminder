#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlTableModel>
#include <QHeaderView>
#include <QMenu>
#include <QLabel>
#include "dialogaddtask.h"
#include "dialogsettime.h"
#include "database.h"
#include "tablemodel.h"
#include "popup.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void addTaskButtonClicked();
    void slotUpdateTTModels();
    void slotUpdateATModels(QVariantList stime);
    void slotEditRecordTT();
    void slotRemoveRecordTT();
    void slotSetTimeRecordTT();
    void slotRemoveRecordAT();
    void slotCustomMenuRequestedTT(QPoint pos);
    void slotCustomMenuRequestedAT(QPoint pos);

private:
    QTableView     *taskTable;
    QTableView     *activeTable;
    TableModel     *modelTaskTable;
    TableModel     *modelActiveTable;
    QPushButton    *addTaskButton;
    QLabel         *activeLabel;
    DataBase       *db;
    PopUp          *popUpMessage;

private:
    void setupLayout();
    void setupTTModel(const QString &tableName, const QStringList &headers);
    void setupATModel(const QString &tableName, const QStringList &headers);
    void setupTT();
    void setupAT();
    void Timers();
};

#endif // MAINWINDOW_H
