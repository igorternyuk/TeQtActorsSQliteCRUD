#include "db_manager.hpp"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QDate>
#include <QByteArray>
#include "comboboxsqlmodel.hpp"

#ifdef DEBUG
#include <QDebug>
#endif

#define CREATE_TABLE_COUNTRIES "CREATE TABLE IF NOT EXISTS country " \
                               "(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " \
                               " name TEXT NOT NULL)"

#define CREATE_TABLE_ACTORS "CREATE TABLE IF NOT EXISTS actor (" \
                            "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " \
                            "name TEXT NOT NULL, dob DATE, bio TEXT, " \
                            "idCountry INTEGER NOT NULL, " \
                            "image BLOB NOT NULL, " \
                            "FOREIGN KEY (idCountry) REFERENCES country(id))" \

#define INSERT_ACTOR "INSERT INTO actor (name, dob, bio, idCountry, image) VALUES(?,?,?,?,?)"
#define UPDATE_ACTOR_IMAGE "UPDATE actor SET image = ? WHERE id = ?"
#define GET_ACTOR_BY_ID "SELECT * FROM actor WHERE id = %1"

DBManager::DBManager(QObject *parent) : QObject(parent)
{
}

bool DBManager::newDatabase(const QString &filename, QString &errorMessage)
{
    mDB = QSqlDatabase::addDatabase("QSQLITE");
    mDB.setDatabaseName(filename);
    if(!mDB.open())
    {
        errorMessage = mDB.lastError().text();
        return false;
    }
    return true;
}

bool DBManager::configurateDB(QString &errorMessage)
{
    QSqlQuery query;
    if(!query.exec(CREATE_TABLE_COUNTRIES))
    {
        errorMessage = mDB.lastError().text();
        return false;
    }
    if(!query.exec(CREATE_TABLE_ACTORS))
    {
        errorMessage = mDB.lastError().text();
        return false;
    }
    mComboCountryModel = new ComboBoxSqlModel("name", "country", this);
    return true;
}

bool DBManager::insertActor(const Actor &actor, QString &errMsg)
{
    QSqlQuery cmd;
    cmd.prepare(INSERT_ACTOR);
    cmd.addBindValue(actor.name());
    cmd.addBindValue(actor.date());
    cmd.addBindValue(actor.bio());
    cmd.addBindValue(actor.idCountry());
    cmd.addBindValue(actor.image());
    if(!cmd.exec())
    {
        errMsg = cmd.lastError().text();
        return false;
    }
    return true;
}

bool DBManager::updateActorImage(int id, const QByteArray &image,
                                 QString &errorMessage)
{
    QSqlQuery cmd;
    cmd.prepare(UPDATE_ACTOR_IMAGE);
    cmd.addBindValue(image);
    cmd.addBindValue(id);
    if(!cmd.exec())
    {
        errorMessage = cmd.lastError().text();
        return false;
    }
    return true;
}

Actor DBManager::getActorById(int id)
{
    /*QSqlQuery query { cmd};
    QSqlRecord rec { query.record() };
    int nameCol { rec.indexOf("number") };
    if(query.next())
    {
        int numberOfResults { query.value(nameCol).toInt() };*/
    QSqlQuery cmd { QString(GET_ACTOR_BY_ID).arg(id) };
    //cmd.prepare(GET_ACTOR_BY_ID);
    //cmd.addBindValue(id);
    QSqlRecord rec { cmd.record() };
    Actor actor;
    if(cmd.next())
    {
        actor.setId(cmd.value(rec.indexOf("id")).toInt());
        actor.setName(cmd.value(rec.indexOf("name")).toString());
        actor.setDate(cmd.value(rec.indexOf("dob")).toDate().toString(Qt::ISODate));
        actor.setBio(cmd.value(rec.indexOf("bio")).toString());
        actor.setIdCountry(cmd.value(rec.indexOf("idCountry")).toInt());
        actor.setImage(cmd.value(rec.indexOf("image")).toByteArray());
    }
    return actor;
}

ComboBoxSqlModel *DBManager::getComboCountryModel() const
{
    return mComboCountryModel;
}
