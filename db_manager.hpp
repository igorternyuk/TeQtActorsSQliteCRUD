#pragma once

#include <QObject>
#include <QSqlDatabase>

//#include <memory>
#include "actor.hpp"

class ComboBoxSqlModel;

class DBManager : public QObject
{
    Q_OBJECT
public:
    enum ActorColumns
    {
        ID,
        NAME,
        DOB,
        BIO,
        ID_COUNTRY,
        IMAGE
    };
    explicit DBManager(QObject *parent = nullptr);
    bool newDatabase(const QString &filename, QString &errorMessage);
    bool configurateDB(QString &errorMessage);
    bool insertActor(const Actor &actor, QString &errMsg);
    bool updateActorImage(int id, const QByteArray &image, QString &errorMessage);
    Actor getActorById(int id);
    ComboBoxSqlModel *getComboCountryModel() const;

private:
    QSqlDatabase mDB;
    ComboBoxSqlModel *mComboCountryModel;
};
