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

DBManager::DBManager(QObject *parent) : QObject(parent)
{
}

DBManager::~DBManager()
{
    closeDatabase();
    delete mComboCountryModel;
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
    if(!query.exec(mCmd[SQL_CMD::CREATE_TABLE_COUNTRIES]))
    {
        errorMessage = mDB.lastError().text();
        return false;
    }
    if(!query.exec(mCmd[SQL_CMD::CREATE_TABLE_ACTORS]))
    {
        errorMessage = mDB.lastError().text();
        return false;
    }
    delete mComboCountryModel;
    mComboCountryModel = new ComboBoxSqlModel("name", "country", this);
    return true;
}

bool DBManager::insertActor(const Actor &actor, QString &errMsg)
{
    QSqlQuery cmd;
    cmd.prepare(mCmd[SQL_CMD::INSERT_ACTOR]);
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

bool DBManager::updateActorBioAndImage(int id, const QString &bio,
                                 const QByteArray &image, QString &errorMessage)
{
    QSqlQuery cmd;
    cmd.prepare(mCmd[SQL_CMD::UPDATE_ACTOR_BIO_AND_IMAGE]);
    cmd.bindValue(":bio", bio);
    cmd.bindValue(":image", image);
    cmd.bindValue(":id", id);
    if(!cmd.exec())
    {
        errorMessage = cmd.lastError().text();
        return false;
    }
    return true;
}

std::shared_ptr<Actor> DBManager::getActorById(int id)
{
    QSqlQuery cmd { QString(mCmd[SQL_CMD::GET_ACTOR_BY_ID]).arg(id) };
    QSqlRecord rec { cmd.record() };
    if(cmd.next())
    {
        auto id = cmd.value(rec.indexOf("id")).toInt();
        auto name = cmd.value(rec.indexOf("name")).toString();
        auto dob = cmd.value(rec.indexOf("dob")).toDate();
        auto bio = cmd.value(rec.indexOf("bio")).toString();
        auto idCountry = cmd.value(rec.indexOf("idCountry")).toInt();
        auto image = cmd.value(rec.indexOf("image")).toByteArray();
        return std::make_shared<Actor>(id, name, dob, bio, idCountry, image);
    }
    return nullptr;
}

ComboBoxSqlModel *DBManager::getComboCountryModel() const
{
    return mComboCountryModel;
}

void DBManager::closeDatabase()
{
    mDB.close();
}
