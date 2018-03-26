#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setGeometry(QRect(400,100,700,400));

    this->addTaskButton = new QPushButton("Добавить задание");
    this->taskTable     = new QTableView();
    this->activeLabel   = new QLabel("Активные");
    this->activeTable   = new QTableView();
    this->popUpMessage  = new PopUp();

    this->setupLayout();
    this->setWindowTitle("Reminder");

    this->db = new DataBase();
    this->db->connectToDataBase();

    this->setupTTModel(TASK, QStringList() << trUtf8("id")
                                         << trUtf8("Статус")
                                         << trUtf8("Дата")
                                         << trUtf8("Время")
                                         << trUtf8("Имя")
                                         << trUtf8("Сообщение"));

    this->setupATModel(ACTIVE, QStringList() << trUtf8("id")
                                            << trUtf8("Статус")
                                            << trUtf8("Имя")
                                            << trUtf8("Время")
                                            << trUtf8("Сообщение"));
    this->setupTT();
    this->setupAT();
    this->Timers();
    connect(this->addTaskButton, SIGNAL(clicked(bool)), this, SLOT(addTaskButtonClicked()));
}

MainWindow::~MainWindow() {}

void MainWindow::setupLayout()
{
    QSpacerItem* vItem = new QSpacerItem(5,10);
    QSpacerItem* hItem = new QSpacerItem(5,10);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(this->addTaskButton);
    leftLayout->addWidget(this->taskTable);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(this->activeLabel);
    rightLayout->addItem(hItem);
    rightLayout->addWidget(this->activeTable);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addItem(vItem);
    mainLayout->addLayout(rightLayout);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);
}

void MainWindow::setupTTModel(const QString &tableName, const QStringList &headers)
{
    this->modelTaskTable = new TableModel(this);
    this->modelTaskTable->setTable(tableName);
    this->modelTaskTable->select();
    for(int i = 0; i < this->modelTaskTable->columnCount(); i++)
        this->modelTaskTable->setHeaderData(i,Qt::Horizontal,headers[i]);
}

void MainWindow::setupATModel(const QString &tableName, const QStringList &headers)
{
    this->modelActiveTable = new TableModel(this);
    this->modelActiveTable->setTable(tableName);
    this->modelActiveTable->setEditStrategy(QSqlTableModel::OnFieldChange);
    this->modelActiveTable->select();
    for(int i = 0; i < this->modelActiveTable->columnCount(); i++)
        this->modelActiveTable->setHeaderData(i,Qt::Horizontal,headers[i]);
}

void MainWindow::setupTT()
{
    this->taskTable->setModel(this->modelTaskTable);
    this->taskTable->setColumnHidden(0, true);
    this->taskTable->setColumnHidden(1, true);
    this->taskTable->setColumnHidden(5, true);
    this->taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->taskTable->setSelectionMode(QAbstractItemView::SingleSelection);
    this->taskTable->resizeColumnsToContents();
    this->taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->taskTable->horizontalHeader()->setStretchLastSection(true);

    this->taskTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->taskTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequestedTT(QPoint)));
}

void MainWindow::setupAT()
{
    this->activeTable->setModel(this->modelActiveTable);
    this->activeTable->setColumnHidden(0, true);
    this->activeTable->setColumnHidden(1, true);
    this->activeTable->setColumnHidden(3, true);
    this->activeTable->setColumnHidden(4, true);
    this->activeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->activeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    this->activeTable->resizeColumnsToContents();
    this->activeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->activeTable->horizontalHeader()->setStretchLastSection(true);

    this->activeTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->activeTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequestedAT(QPoint)));
}

void MainWindow::addTaskButtonClicked()
{
    DialogAddTask *addTaskDialog = new DialogAddTask();
    connect(addTaskDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateTTModels()));
    addTaskDialog->exec();
}


void MainWindow::slotCustomMenuRequestedTT(QPoint pos)
{
    QMenu   *menu         = new QMenu(this);
    QAction *editDevice   = new QAction(trUtf8("Редактировать"), this);
    QAction *deleteDevice = new QAction(trUtf8("Удалить"), this);
    QAction *setTime      = new QAction(trUtf8("Задать время"),this);
    connect(editDevice, SIGNAL(triggered()), this, SLOT(slotEditRecordTT()));
    connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecordTT()));
    connect(setTime, SIGNAL(triggered()),this,SLOT(slotSetTimeRecordTT()));
    menu->addAction(editDevice);
    menu->addAction(setTime);
    menu->addAction(deleteDevice);
    menu->popup(this->taskTable->viewport()->mapToGlobal(pos));
}

void MainWindow::slotCustomMenuRequestedAT(QPoint pos)
{

    QMenu   *menu         = new QMenu(this);
    QAction *deleteDevice = new QAction(trUtf8("Удалить"), this);
    connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecordAT()));
    menu->addAction(deleteDevice);
    menu->popup(this->activeTable->viewport()->mapToGlobal(pos));
}

void MainWindow::slotRemoveRecordTT()
{
    int row = this->taskTable->selectionModel()->currentIndex().row();

    if(row >= 0)
    {
        if (QMessageBox::warning(this,
                                 trUtf8("Удаление записи"),
                                 trUtf8("Вы уверены, что хотите удалить эту запись?"),
                                 QMessageBox::No | QMessageBox::Yes) == QMessageBox::No)
        {
            QSqlDatabase::database().rollback();
            return;
        }
        else
        {
            if(!this->modelTaskTable->removeRow(row))
            {
                QMessageBox::warning(this,trUtf8("Уведомление"),
                                     trUtf8("Не удалось удалить запись\n"
                                            "Возможно она используется другими таблицами\n"
                                            "Проверьте все зависимости и повторите попытку"));
            }
            this->modelTaskTable->select();
            this->taskTable->setCurrentIndex(this->modelTaskTable->index(-1, -1));
        }
    }
}

void MainWindow::slotEditRecordTT()
{
    DialogAddTask *addDeviceDialog = new DialogAddTask(this->taskTable->selectionModel()->currentIndex().row());
    connect(addDeviceDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateTTModels()));
    addDeviceDialog->exec();
}

void MainWindow::slotRemoveRecordAT()
{
    int row = this->activeTable->selectionModel()->currentIndex().row();

    if(row >= 0)
    {
        if (QMessageBox::warning(this,
                                 trUtf8("Удаление записи"),
                                 trUtf8("Вы уверены, что хотите удалить эту запись?"),
                                 QMessageBox::No | QMessageBox::Yes) == QMessageBox::No)
        {
            QSqlDatabase::database().rollback();
            return;
        }
        else
        {
            if(!this->modelActiveTable->removeRow(row))
            {
                QMessageBox::warning(this,trUtf8("Уведомление"),
                                     trUtf8("Не удалось удалить запись\n"
                                            "Возможно она используется другими таблицами\n"
                                            "Проверьте все зависимости и повторите попытку"));
            }
            this->modelActiveTable->select();
            this->activeTable->setCurrentIndex(this->modelActiveTable->index(-1, -1));
        }
    }
}

void MainWindow::slotUpdateTTModels()
{
    this->modelTaskTable->select();
    this->taskTable->resizeColumnsToContents();
}

void MainWindow::slotUpdateATModels(QVariantList stime)
{
    QDateTime dt = QDateTime::currentDateTimeUtc();
    int nowTime = dt.currentMSecsSinceEpoch() / 1000;
    this->db->inserIntoDeviceTable(QVariantList() << this->modelTaskTable->data(modelTaskTable->index(stime[0].toInt(),4),Qt::DisplayRole).toString()
                                            << (stime[1].toInt() + dt.currentMSecsSinceEpoch() / 1000)
                                            << this->modelTaskTable->data(modelTaskTable->index(stime[0].toInt(),5),Qt::DisplayRole).toString());
    this->modelActiveTable->select();
    int i = this->modelActiveTable->rowCount()-1;
    int time = this->modelActiveTable->data(this->modelActiveTable->index(i,3),Qt::DisplayRole).toInt();
    QTimer *t = new QTimer();
    t->start((time - nowTime) * 1000);
    connect(t,&QTimer::timeout, [i,this,t]()
    {
        t->stop();
        this->popUpMessage->setPopupText(this->modelActiveTable->data(this->modelActiveTable->index(i,4),Qt::DisplayRole).toString());
        this->popUpMessage->show();
        this->modelActiveTable->setData(this->modelActiveTable->index(i,1),4);
        this->modelActiveTable->select();
    });
}

void MainWindow::slotSetTimeRecordTT()
{
    DialogSetTime *setTimeDialog = new DialogSetTime(taskTable->selectionModel()->currentIndex().row());
    connect(setTimeDialog, SIGNAL(signalTimeReady(QVariantList)), this, SLOT(slotUpdateATModels(QVariantList)));
    setTimeDialog->exec();
}

void MainWindow::Timers()
{
    for(int i = 0; i < modelActiveTable->rowCount(); i++)
    {
        int time = modelActiveTable->data(modelActiveTable->index(i,3),Qt::DisplayRole).toInt();
        QDateTime dt = QDateTime::currentDateTimeUtc();
        int nowTime = dt.currentMSecsSinceEpoch() / 1000;
        if(nowTime >= time)
        {
           modelActiveTable->setData(modelActiveTable->index(i,1),4);
           modelActiveTable->select();
           continue;
        }
        int status = this->modelActiveTable->data(this->modelActiveTable->index(i,1),Qt::DisplayRole).toInt();
        if(status == 3)
        {
            qDebug() << time - nowTime;
            QTimer *t = new QTimer();
            t->start((time - nowTime) * 1000);
            connect(t,&QTimer::timeout, [i,this,t]()
            {
                t->stop();
                this->popUpMessage->setPopupText(this->modelActiveTable->data(this->modelActiveTable->index(i,4),Qt::DisplayRole).toString());
                this->popUpMessage->show();
                this->modelActiveTable->setData(this->modelActiveTable->index(i,1),4);
                this->modelActiveTable->select();
            });
        }
    }
}
