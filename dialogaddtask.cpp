#include "dialogaddtask.h"
#include <QDebug>


DialogAddTask::DialogAddTask(int row, QWidget *parent) :
    QDialog(parent)
{
    this->setGeometry(QRect(500,200,300,300));
    this->setupModel();

    this->nameLabel = new QLabel("Имя:");
    this->dateLabel = new QLabel();
    this->timeLabel = new QLabel();
    if(row == -1)
    {
        this->dateLabel->setText(QDate::currentDate().toString("dd:MM:yyyy"));
        this->timeLabel->setText(QTime::currentTime().toString());
    }
    else
    {
        this->dateLabel->setText(model->data(model->index(row,2),Qt::DisplayRole).toString());
        this->timeLabel->setText(model->data(model->index(row,3),Qt::DisplayRole).toString());
    }

    this->nameLineEdit    = new QLineEdit();
    this->messageTextEdit = new QTextEdit();
    this->nextButton      = new QPushButton("Вперёд");
    this->prevButton      = new QPushButton("Назад");
    this->okButton        = new QPushButton("Ок");
    this->statusSlider    = new QSlider(Qt::Horizontal);
    this->statusSlider->setRange(0,2);

    this->setupLayout();
    this->setupMapper();

    if(row == -1)
    {
        this->model->insertRow(this->model->rowCount(QModelIndex()));
        this->mapper->toLast();
    }
    else
        this->mapper->setCurrentModelIndex(this->model->index(row,0));

    this->setWindowTitle("Edit");
    connect(this->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonAccepted()));
}

DialogAddTask::~DialogAddTask() {}

void DialogAddTask::setupModel()
{
    this->model = new TableModel(this);
    this->model->setTable(TASK);
    this->model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->model->select();
}

void DialogAddTask::setupLayout()
{
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(this->nameLabel);
    topLayout->addWidget(this->nameLineEdit);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(this->prevButton);
    bottomLayout->addWidget(this->nextButton);
    bottomLayout->addWidget(this->okButton);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(this->messageTextEdit);
    mainLayout->addWidget(this->statusSlider);
    mainLayout->addLayout(bottomLayout);

    this->setLayout(mainLayout);
}

void DialogAddTask::setupMapper()
{
    this->mapper = new QDataWidgetMapper();
    this->mapper->setModel(this->model);
    this->mapper->addMapping(this->statusSlider, 1);
    this->mapper->addMapping(this->dateLabel, 2);
    this->mapper->addMapping(this->timeLabel, 3);
    this->mapper->addMapping(this->nameLineEdit, 4);
    this->mapper->addMapping(this->messageTextEdit, 5, "plainText");
    this->mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    connect(this->prevButton, SIGNAL(clicked()), this->mapper, SLOT(toPrevious()));
    connect(this->nextButton, SIGNAL(clicked()), this->mapper, SLOT(toNext()));
    connect(this->mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons(int)));
}

void DialogAddTask::okButtonAccepted()
{
    this->mapper->submit();
    if(!this->model->submitAll())
        qDebug() << "init err";
    emit signalReady();
    this->close();
}

void DialogAddTask::updateButtons(int row)
{
    this->prevButton->setEnabled(row > 0);
    this->nextButton->setEnabled(row < this->model->rowCount() - 1);
}

