#include "actor.hpp"

Actor::Actor(const QByteArray &image):
    mImage { image }
{}

Actor::Actor(int id, const QString &name, const QString &date,
             const QString &bio, int idCountry,
             const QByteArray &image):
    mId { id }, mName{ name }, mDate { date }, mBio { bio },
    mIdCountry{ idCountry }, mImage { image }
{}

Actor::Actor(const QString &name, const QString &date,
             const QString &bio, int idCountry,
             const QByteArray &image):
    Actor(0, name, date, bio, idCountry, image)
{}

int Actor::id() const
{
    return mId;
}

QString Actor::name() const
{
    return mName;
}

QString Actor::date() const
{
    return mDate;
}

QString Actor::bio() const
{
    return mBio;
}

int Actor::idCountry() const
{
    return mIdCountry;
}

QByteArray Actor::image() const
{
    return mImage;
}

void Actor::setId(int id)
{
    mId = id;
}

void Actor::setName(const QString &name)
{
    mName = name;
}

void Actor::setDate(const QString &date)
{
    mDate = date;
}

void Actor::setBio(const QString &bio)
{
    mBio = bio;
}

void Actor::setIdCountry(int idCountry)
{
    mIdCountry = idCountry;
}

void Actor::setImage(const QByteArray &image)
{
    mImage = image;
}
