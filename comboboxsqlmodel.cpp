#include "comboboxsqlmodel.hpp"
#include <QSqlQuery>
#ifdef DEBUG
#include <QDebug>
#endif

namespace
{
    enum Columns { ID, DATA };
}

ComboBoxSqlModel::ComboBoxSqlModel(const QString &visibleColumn,
                                   const QString &tableName, QObject *parent):
    QSqlQueryModel(parent),
    mQuery { QString( "SELECT id, %1 FROM %2;" ).arg(visibleColumn).arg(tableName) },
    mTableName { tableName }
{
    this->reload();
}

QVariant ComboBoxSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if(index.row() == 0)
    {
        switch(role)
        {
            case Qt::UserRole:
                result = 0;
                break;
            case Qt::DisplayRole:
                result = "Please select " + mTableName;
                break;
            default:
                break;
        }
    }
    else
    {
        switch(role)
        {
            case Qt::UserRole:
                result = dataFromParent(index, ID);
                break;
            case Qt::DisplayRole:
                result = dataFromParent(index, DATA);
                break;
            default:
                break;
        }
    }

    return result;
}

int ComboBoxSqlModel::rowCount(const QModelIndex &parent) const
{
   return QSqlQueryModel::rowCount(parent) + 1;
   // return QSqlQueryModel::rowCount(parent);
}

void ComboBoxSqlModel::reload()
{
    QSqlQuery query;
    qDebug() << "SQL = " << mQuery;
    query.prepare( mQuery );
    query.exec();
    QSqlQueryModel::setQuery(mQuery);
}

QVariant ComboBoxSqlModel::dataFromParent(QModelIndex index, int column) const
{
    return QSqlQueryModel::data(QSqlQueryModel::index(index.row() - 1, column));
    //return QSqlQueryModel::data(QSqlQueryModel::index(index.row(), column));
}

/*

ui->countryCombo->setModel(  new BaseComboModel( "countryname || ' - ' || countrycode", "country",  this ) );

Таким образом вы можете быстро и легко делать модели для QComboBox-а с помощью BaseComboModel.
Например, у вас есть SQL таблица месяцев в году(«months»).
Где два столбца, «id» и «monthname». Вам можно заполнить комбобокс выбора
месяца следующим образом:

ui->monthsCombo->setModel( new BaseComboModel( «monthname», «months», this ) );
Получить значение «id» выбранного месяца
ui->monthsCombo->itemData( ui->monthsCombo->currentIndex(), Qt::UserRole );.
 Получть значение видимое пользователю ui->monthsCombo->currentText();.
 Этот код гораздо компактнее всех остальных случаев. Большинство разработчиков в
данной ситуации пишут, отдельно запрос к базе (QSqlQuery). А потом, в цикле,
 добавляют полученные записи в комбобокс, через QComboBox::addItem().
 Это конечно рабочее, но не самое красивое решение.
*/
