#include "record.h"

Record::Record(const QString &title)
{
    this->title = title;
}

Record::Record(const QString &title, const QString &comment,
               const bool &readOnly, const QVariant::Type &type,
               const QVariant &value)
{
    this->title = title;
    this->comment = comment;
    this->readOnly = readOnly;
    this->type = type;
    if (value.type() == type)
    {
        this->value = value;
    }
    else
    {
        this->value = QVariant(type);
    }
}

QString Record::getTitle() const
{
    return this->title;
}

void Record::setTitle(const QString &title)
{
    this->title = title;
}

QString Record::getComment() const
{
    return this->comment;
}

void Record::setComment(const QString &comment)
{
    this->comment = comment;
}

bool Record::isReadOnly() const
{
    return this->readOnly;
}

void Record::setReadOnly(const bool &readOnly)
{
    this->readOnly = readOnly;
}

QVariant Record::getValue() const
{
    return this->value;
}

void Record::setValue(const QVariant &value)
{
    this->value = value;
}

bool Record::operator ==(const Record &record) const
{
    return this->title == record.getTitle();
}

QVariant::Type Record::getType () const
{
    return this->type;
}
