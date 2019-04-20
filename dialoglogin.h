#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>


class DialogLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLogin(QWidget *parent = 0);
    ~DialogLogin();

private:
    QLabel *nameLabel;
    QLabel *passwordLabel;
    QLineEdit *nameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *okButton;

private slots:
    void okButtonAccepted();

signals:
    void signalLoginReady(QString);

private:
    void setupLayout();
};

#endif // LOGIN_H
