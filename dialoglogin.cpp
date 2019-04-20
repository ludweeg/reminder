#include "dialoglogin.h"

DialogLogin::DialogLogin(QWidget *parent) :
    QDialog(parent)
{
    this->setGeometry(QRect(500,200,300,100));

    this->nameLabel = new QLabel("Введите имя:");
    this->nameLineEdit = new QLineEdit();
    this->passwordLabel = new QLabel("Введите пароль:");
    this->passwordLineEdit = new QLineEdit();
    this->passwordLineEdit->setEchoMode(QLineEdit::Password);
    this->okButton = new QPushButton("Войти");

    this->setupLayout();
    this->setWindowTitle("Login");

    connect(this->okButton, SIGNAL(clicked(bool)), this, SLOT(okButtonAccepted()));
}

DialogLogin::~DialogLogin() {}

void DialogLogin::setupLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(this->nameLabel);
    mainLayout->addWidget(this->nameLineEdit);
    mainLayout->addWidget(this->passwordLabel);
    mainLayout->addWidget(this->passwordLineEdit);
    mainLayout->addWidget(this->okButton);
    this->setLayout(mainLayout);
}

void DialogLogin::okButtonAccepted()
{
    emit signalLoginReady(this->nameLineEdit->text());
    this->close();
}
