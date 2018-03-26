#include "tablemodel.h"
#include <QColor>
#include <QDebug>

TableModel::TableModel(QObject *parent) : QSqlTableModel(parent) {}

QVariant TableModel::data(const QModelIndex &idx, int role) const
{
    if(role == Qt::BackgroundColorRole){
        switch (QSqlTableModel::data(this->index(idx.row(), 1)).toInt()) {
        case 0:
            return QColor(255,204,204,150);
            break;
        case 1:
            return QColor(255,255,153,150);
            break;
        case 2:
            return QColor(204,255,153,150);
            break;
        case 3:
            return QColor(240, 248, 255);
            break;
        case 4:
            return QColor(250, 235, 215);
            break;
        default:
            return QColor(Qt::white);
            break;
        }
    }

    return QSqlTableModel::data(idx,role);
}
