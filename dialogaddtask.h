#ifndef DIALOGADDTASK_H
#define DIALOGADDTASK_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QSlider>
#include <QVariant>
#include "database.h"
#include "tablemodel.h"

class DialogAddTask : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddTask(int row = -1, QWidget *parent = 0);
    ~DialogAddTask();

signals:
    void signalReady();

private slots:
    void okButtonAccepted();
    void updateButtons(int row);

private:
    TableModel        *model;
    QDataWidgetMapper *mapper;
    QLabel            *nameLabel;
    QLabel            *dateLabel;
    QLabel            *timeLabel;
    QLineEdit         *nameLineEdit;
    QTextEdit         *messageTextEdit;
    QPushButton       *nextButton;
    QPushButton       *prevButton;
    QPushButton       *okButton;
    QSlider           *statusSlider;

private:
    void setupModel();
    void setupLayout();
    void setupMapper();
};

#endif // DIALOGADDTASK_H

