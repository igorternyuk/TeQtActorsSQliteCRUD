#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <memory>
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
    enum class SQL_CMD
    {
        CREATE_TABLE_COUNTRIES,
        CREATE_TABLE_ACTORS,
        INSERT_ACTOR,
        UPDATE_ACTOR_BIO_AND_IMAGE,
        GET_ACTOR_BY_ID
    };

    QMap<SQL_CMD, QString> mCmd
    {
        { SQL_CMD::CREATE_TABLE_COUNTRIES, "CREATE TABLE IF NOT EXISTS country " \
                                           "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " \
                                           " name TEXT NOT NULL)"},
        { SQL_CMD::CREATE_TABLE_ACTORS, "CREATE TABLE IF NOT EXISTS actor (" \
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " \
                                        "name TEXT NOT NULL, dob DATE, bio TEXT, " \
                                        "idCountry INTEGER NOT NULL, " \
                                        "image BLOB NOT NULL, " \
                                        "FOREIGN KEY (idCountry) REFERENCES country(id))" },
        { SQL_CMD::INSERT_ACTOR, "INSERT INTO actor (name, dob, bio, idCountry, image) VALUES(?,?,?,?,?)" },
        { SQL_CMD::UPDATE_ACTOR_BIO_AND_IMAGE, "UPDATE actor SET bio=:bio, image=:image WHERE id=:id" },
        { SQL_CMD::GET_ACTOR_BY_ID, "SELECT * FROM actor WHERE id = %1" }
    };
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();
    bool newDatabase(const QString &filename, QString &errorMessage);
    bool configurateDB(QString &errorMessage);
    bool insertActor(const Actor &actor, QString &errMsg);
    bool updateActorBioAndImage(int id, const QString &bio,
                                const QByteArray &image,
                                QString &errorMessage);
    std::shared_ptr<Actor> getActorById(int id);
    ComboBoxSqlModel *getComboCountryModel() const;
    void closeDatabase();

private:
    QSqlDatabase mDB;
    ComboBoxSqlModel *mComboCountryModel { nullptr };
};
