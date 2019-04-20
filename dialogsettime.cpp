#include "dialogsettime.h"

DialogSetTime::DialogSetTime(int row, QWidget *parent) :
    QDialog(parent)
{
    this->setGeometry(QRect(500,200,300,100));

    this->timeLabel    = new QLabel("Введите время:");
    this->timeLineEdit = new QLineEdit();
    this->okButton     = new QPushButton("Напомнить");
    this->row = row;

    this->setupLayout();

    connect(this->okButton, SIGNAL(clicked()), this, SLOT(okButtonAccepted()));
}

DialogSetTime::~DialogSetTime() {}

void DialogSetTime::setupLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(this->timeLabel);
    mainLayout->addWidget(this->timeLineEdit);
    mainLayout->addWidget(this->okButton);
    this->setLayout(mainLayout);
}

void DialogSetTime::okButtonAccepted()
{
    emit signalTimeReady(QVariantList() << this->row
                                 << this->timeLineEdit->text().toInt());
    this->close();
}

