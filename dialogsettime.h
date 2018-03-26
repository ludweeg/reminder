#ifndef DIALOGSETTIME_H
#define DIALOGSETTIME_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>


class DialogSetTime : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetTime(int row = -1, QWidget *parent = 0);
    ~DialogSetTime();

signals:
    void signalTimeReady(const QVariantList vl);

private slots:
    void okButtonAccepted();

private:
    QLabel      *timeLabel;
    QLineEdit   *timeLineEdit;
    QPushButton *okButton;
    int row;

private:
    void setupLayout();
};

#endif // DIALOGSETTIME_H

