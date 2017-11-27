#pragma once

#include <QString>
#include <QByteArray>

class Actor
{
public:
    explicit Actor(const QByteArray &image = QByteArray());
    explicit Actor(const QString &name, const QString &date,
                   const QString &bio, int idCountry,
                   const QByteArray &image = QByteArray());
    explicit Actor(int id, const QString &name, const QString &date,
                   const QString &bio, int idCountry,
                   const QByteArray &image = QByteArray());
    int id() const;
    QString name() const;
    QString date() const;
    QString bio() const;
    int idCountry() const;
    QByteArray image() const;

    void setId(int id);
    void setName(const QString &name);
    void setDate(const QString &date);
    void setBio(const QString &bio);
    void setIdCountry(int idCountry);
    void setImage(const QByteArray &image);

private:
    int mId;
    QString mName;
    QString mDate;
    QString mBio;
    int mIdCountry;
    QByteArray mImage;
};
