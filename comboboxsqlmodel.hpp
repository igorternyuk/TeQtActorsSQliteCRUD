#pragma once

#include <QObject>
#include <QSqlQueryModel>

class ComboBoxSqlModel: public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ComboBoxSqlModel(const QString &visibleColumn, const QString &tableName,
                              QObject *parent = nullptr);
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    void reload();

private:
    QVariant dataFromParent(QModelIndex index, int column) const;
    QString mQuery;
    QString mTableName;
};
